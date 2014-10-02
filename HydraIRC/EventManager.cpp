/*

  HydraIRC
  Copyright (C) 2002-2006 Dominic Clifton aka Hydra

  HydraIRC limited-use source license

  1) You can:
  1.1) Use the source to create improvements and bug-fixes to send to the
       author to be incorporated in the main program.
  1.2) Use it for review/educational purposes.

  2) You can NOT:
  2.1) Use the source to create derivative works. (That is, you can't release
       your own version of HydraIRC with your changes in it)
  2.2) Compile your own version and sell it.
  2.3) Distribute unmodified, modified source or compiled versions of HydraIRC
       without first obtaining permission from the author. (I want one place
       for people to come to get HydraIRC from)
  2.4) Use any of the code or other part of HydraIRC in anything other than 
       HydraIRC.
       
  3) All code submitted to the project:
  3.1) Must not be covered by any license that conflicts with this license 
       (e.g. GPL code)
  3.2) Will become the property of the author.

*/

/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "HydraIRC.h"

//Define this to enable a event queue dump each time ProcessEvents() is called
//#define EVENTMANAGER_DEBUG

#ifdef DEBUG
char *g_EventIDNames[EV_LAST] = {
  "EV_SV_RECONNECT",
  "EV_SV_KEEPALIVE",
  "EV_PREFSCHANGED",
  "EV_CH_RETRYJOIN",
  "EV_TICK",
  "EV_DIEDIEDIE"
};
#endif

CEventManager::CEventManager(void)
{
}

CEventManager::~CEventManager(void)
{
  // wtf? we have listeners left...
  ATLASSERT(m_ListenerList.GetSize() == 0);
  // Delete remaining events
  DeleteEvents(NULL);
}

void CEventManager::AddListener(CListener *pListener)
{
  m_ListenerList.Add(pListener);
}

void CEventManager::RemoveListener(CListener *pListener)
{
  // remove the listener
  m_ListenerList.Remove(pListener);

  // remove all outstanding events from this source.
  DeleteEvents(pListener);
}

/** Delete events from the event mananger's queue
 *
 * Specify just the listener (pFrom) to remove all events
 * from the queue from that source, or specify
 * both to have all events from the source matching the EventID
 * removed from the queue
 * Note: NULL Listeners's *ARE* valid.  an event can be created by
 * somethinbg that's not a listener.
 */
BOOL CEventManager::DeleteEvents(CListener *pFrom, int EventID, void *pData)
{
  CNode *pNode,*pTempNode;
  EventQueueItem_t *pEI;
  int EventQueueItemNum;
  BOOL Found = FALSE;

  for (EventQueueItemNum = 0,pNode = m_EventList.GetFirst(); pNode != NULL && pNode->m_Next != NULL; pNode = pNode->m_Next,EventQueueItemNum ++)
  {
    pEI = (EventQueueItem_t *)pNode->m_Data;

    if (pEI->pFrom == pFrom && (pEI->EventID == EventID || EventID == -1) && (pEI->pData == pData || pData == NULL) && pEI->Processing == FALSE)
    {
#ifdef DEBUG
      if (EventID == -1)
        sys_Printf(BIC_INFO,"Removing unprocessed event, Event Queue Number: %d, ID: %s (%d)\n",EventQueueItemNum,pEI->EventID < EV_LAST ? g_EventIDNames[pEI->EventID] : "CUSTOMID",pEI->EventID);
      else
        sys_Printf(BIC_INFO,"Removing event, Event Queue Number: %d, ID: %s (%d)\n",EventQueueItemNum,pEI->EventID < EV_LAST ? g_EventIDNames[pEI->EventID] : "CUSTOMID",pEI->EventID);
#endif
      pTempNode = pNode->m_Next;
      free(pEI);
      pNode->Remove();
      delete pNode;
      pNode = pTempNode;
      Found = TRUE;
    }
  }
  return Found;
}

BOOL CEventManager::QueueTimedEvent(CListener *pFrom, BOOL Broadcast, int EventID,int Seconds, BOOL Repeat, BOOL SkipMissed, void *pData)
{
  // can't create non-broadcast events without a listener
  if (!pFrom && !Broadcast)
    return FALSE;

  CNode *pNode = new CNode;
  if (!pNode)
    return FALSE;

  EventQueueItem_t *pNewEI = (EventQueueItem_t *)malloc(sizeof(EventQueueItem_t));
  if (!pNewEI)
  {
    delete pNode;
    return FALSE;
  }

  // set who it's from
  pNewEI->pFrom = pFrom;
  // set the timed stuff and flags
  pNewEI->Flags = ET_TIMED;
  time(&pNewEI->StartTime); // store NOW.
  pNewEI->Seconds = Seconds;
  if (Repeat)
    pNewEI->Flags |= ET_REPEAT;
  if (Broadcast)
    pNewEI->Flags |= ET_BROADCAST;
  if (SkipMissed)
    pNewEI->Flags |= ET_SKIPMISSED;
  // set the event data
  pNewEI->EventID = EventID;
  pNewEI->pData = pData;
  // set the status
  pNewEI->Processing = FALSE;
  // add to queue
  pNode->m_Data = pNewEI;
  m_EventList.AddTail(pNode);

  return TRUE;
}

BOOL CEventManager::QueueEvent(CListener *pFrom, BOOL Broadcast, int EventID, void *pData)
{
  // can't create non-broadcast events without a listener
  if (!pFrom && !Broadcast)
    return FALSE;

  CNode *pNode = new CNode;
  if (!pNode)
    return FALSE;

  EventQueueItem_t *pNewEI = (EventQueueItem_t *)malloc(sizeof(EventQueueItem_t));
  if (!pNewEI)
  {
    delete pNode;
    return FALSE;
  }
  // set who it's from
  pNewEI->pFrom = pFrom;
  // set flags
  if (Broadcast)
    pNewEI->Flags |= ET_BROADCAST;
  // set the event data
  pNewEI->EventID = EventID;
  pNewEI->pData = pData;
  // set the status
  pNewEI->Processing = FALSE;
  // add to queue
  pNode->m_Data = pNewEI;
  m_EventList.AddTail(pNode);

  return TRUE;
}

/** Create an event and fire it, without adding to the queue.
 *	
 */
BOOL CEventManager::DoEvent(CListener *pFrom, BOOL Broadcast, int EventID, void *pData)
{
  // can't create non-broadcast events without a listener
  if (!pFrom && !Broadcast)
    return FALSE;

  EventQueueItem_t *pNewEI = (EventQueueItem_t *)malloc(sizeof(EventQueueItem_t));

  if (!pNewEI)
    return FALSE;

  // set who it's from
  pNewEI->pFrom = pFrom;
  // set flags
  if (Broadcast)
    pNewEI->Flags |= ET_BROADCAST;
  // set the event data
  pNewEI->EventID = EventID;
  pNewEI->pData = pData;
  // set the status
  pNewEI->Processing = FALSE;
  FireEvent(pNewEI);

  free(pNewEI);

  return TRUE;
}

void CEventManager::FireEvent(EventQueueItem_t *pEI)
{
  CListener *pListener;

  if (pEI->Flags & ET_BROADCAST)
  {
    int i;
    CSimpleArray<CListener *> FireOrder;

    // firing an event can cause a listener to be removed from the list
    // so we make a copy of the list, and then fire each one.

    for (i = 0 ; i < m_ListenerList.GetSize() ; i++)
      FireOrder.Add(m_ListenerList[i]);

    for (i = 0 ; i < FireOrder.GetSize() ; i++)
    {
      pListener = FireOrder[i];
      // only fire the event if it's still in the list! (if it's not then pListener is invalid..)
      if (m_ListenerList.Find(pListener) >= 0)
        pListener->OnEvent(pEI->EventID,pEI->pData);
    }
  }
  else
  {
    pEI->pFrom->OnEvent(pEI->EventID,pEI->pData);
  }
}
void CEventManager::ProcessEvents( void )
{
  CNode *pNode,*pTempNode;
  BOOL remove;
  EventQueueItem_t *pEI;
  double diff;
  time_t now;
  time(&now);
  int EventQueueItemNum;

//#ifdef EVENTMANAGER_DEBUG
#ifdef DEBUG
  if (g_DebugFlags & DBGFLG_EVENTDUMP1 || g_DebugFlags & DBGFLG_EVENTDUMPALL)
  {
    // dump event queue  
    tm *t;
    for (EventQueueItemNum = 0,pNode = m_EventList.GetFirst(); pNode->m_Next != NULL; pNode = pNode->m_Next,EventQueueItemNum ++)
    {
      pEI = (EventQueueItem_t *)pNode->m_Data;
      t = localtime(&pEI->StartTime);
      sys_Printf(BIC_INFO,"Event Item %d: %s %s (%d) %s-%d (%s) %s %s\n",
        EventQueueItemNum,
        pEI->Processing ? "Processing" : "Ready",
        pEI->EventID < EV_LAST ? g_EventIDNames[pEI->EventID] : "CUSTOMID",pEI->EventID,
        pEI->Flags & ET_TIMED ? "Timed" : "Not Timed",
        pEI->Flags & ET_TIMED ? pEI->Seconds : 0,
        pEI->Flags & ET_TIMED ? stripcrlf(asctime(t)) : "N/A",
        pEI->Flags & ET_REPEAT ? "Repeat" : "No Repeat",
        pEI->Flags & ET_BROADCAST ? "Broadcast" : "Private");

      if (g_DebugFlags & DBGFLG_EVENTDUMP1) // just dump the first event (usually the EV_TICK one)
        break;
    }
  }
#endif

  for (EventQueueItemNum = 0 , pNode = m_EventList.GetFirst(); 
       pNode != NULL && pNode->m_Next != NULL;
       pNode = pNode->m_Next,EventQueueItemNum ++)
  {
    pEI = (EventQueueItem_t *)pNode->m_Data;

    // don't keep processing the same event
    // some events could trigger other events...
    // if we didn't do this we'd end up in an endless loop
    if (pEI->Processing)
      continue;
    // set our flag.
    pEI->Processing = TRUE;
    
    // we remove all events after processing, unless they're repat timed events.
    remove = TRUE; 


    if (pEI->Flags & ET_TIMED)
    {
      // time event due yet ?
      diff = difftime(now,pEI->StartTime);

#ifdef DEBUG
      // if it's late, tell us (but ignore short timed events)
      if (pEI->Seconds > 2 && diff > pEI->Seconds)
      {
        sys_Printf(BIC_WARNING,"Event Queue Item %d is %d seconds late\n",EventQueueItemNum,(int)(diff - pEI->Seconds));
      }
#endif
      if (diff >= pEI->Seconds)
      {
        // yes!

        // repeat timer ?
        if (pEI->Flags & ET_REPEAT)
        {
          // add the seconds to the start time
          // so we repeat exactly x seconds after the start time
          // if we used "now" we'd introduce a bit of fluctuation
          // and could skep events if we're running late
          // as calls to ProcessEvents() are not evenly spaced in time.
          pEI->StartTime += pEI->Seconds;

          double diff2 = difftime(now,pEI->StartTime); //Note: process new start time...
          if (diff2 > pEI->Seconds)
          {
            // if an event was queued for repeat every 5 seconds at
            // 00:00:00 and then 22 seconds passed before ProcessEvents()
            // was called again (stalled process or something...)
            // we'll print out how many times it should have run
            // in this case 4 times.
            // note that all the four events are still triggered
            // unless the ET_SKIPMISSED flag is set
            // but the timing of them will depend on the frequency of
            // calls to ProcessEvents() (normally every 1 second)
#ifdef DEBUG
            sys_Printf(BIC_WARNING,"Event Queue Item %d has overrun %d times\n",EventQueueItemNum,(int)(diff2 / pEI->Seconds));
#endif
            if (pEI->Flags & ET_SKIPMISSED)
            {
              // skip missed events if we're supposed to.
#ifdef DEBUG
              sys_Printf(BIC_WARNING,"Event Queue Item %d - Skipping missed events\n",EventQueueItemNum);
#endif
              do 
              {
                pEI->StartTime += pEI->Seconds;
                diff2 = difftime(now,pEI->StartTime);
              } while(diff2 > pEI->Seconds);
            }
          }

          remove = FALSE; // don't free it!
        }

        // fire the event
        FireEvent(pEI);        
      } 
      else
      {
        // not processed it yet..  need to wait more
        remove = FALSE;
      }
    }
    else
    {
      // not a timed event, so fire it.
      FireEvent(pEI);
    }

    if (remove)
    {
      // remove from list and reset loop pointer
      pTempNode = pNode->m_Next;
      free(pEI);
      pNode->Remove();
      delete pNode;
      pNode = pTempNode;
    }
    else
    {
      // Finished processing this event now.
      pEI->Processing = FALSE;
    }
    ATLASSERT(pNode);
  }
}
