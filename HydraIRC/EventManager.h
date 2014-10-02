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

#pragma once

//ET_* are bitmasks.
#define ET_NORMAL     0 // fire the event NOW, and halt execution until the event has completed.
#define ET_TIMED      1 // fire the event after a delay
#define ET_REPEAT     2 // repeat the event, only valid for timed events
#define ET_BROADCAST  4 // tells all listeners about the event.
#define ET_SKIPMISSED 8 // if a repeat timed event is late, more than 
                        // the duration of repeat time then 
                        // missed events are skipped
//#define ET_         16,32...

typedef struct EventQueueItem_s
{
  CListener *pFrom; // the instance of listener class that sent the message
  int EventID;
  int Seconds;
  time_t StartTime;
  int Flags; // See ET_* defines
  void *pData;
  BOOL Processing;
} EventQueueItem_t;

class CEventManager
{
private:
  // this list can changes a bit and processing of it needs to be fast
  // so use our own simple implementation of CList
  // each CNode's Data member in the list points to a EventQueueItem_t struct.
  CList m_EventList;
  // this list doesn't change much...
  CSimpleArray<CListener *> m_ListenerList;
public:
  CEventManager(void);
  ~CEventManager(void);

  // to be called by the Constructor of a class derived from CListener
  void AddListener(CListener *pListener);
  // to be called by the Destructor of a class derived from CListener
  void RemoveListener(CListener *pListener);
  // creates a timed event and places it on the queue, and returns immediately
  BOOL QueueTimedEvent(CListener *pFrom, BOOL Broadcast, int EventID,int Seconds, BOOL Repeat = FALSE, BOOL SkipMissed = FALSE, void *pData = NULL);
  // create a non-timed event and places it on the queue, and returns immediately
  BOOL QueueEvent(CListener *pFrom, BOOL Broadcast, int EventID, void *pData = NULL);
  // create a non-timed event and waits for event to be processed before returning
  BOOL DoEvent(CListener *pFrom, BOOL Broadcast, int EventID, void *pData = NULL);
  // delete all events with matching listener and EventID, or leave EventID as -1
  // and ALL events from the listener will be deleted.
  BOOL DeleteEvents(CListener *pFrom, int EventID = -1, void *pData = NULL);

  // Call this every 1 second to process the timed events and other
  // non-timed events.
  void ProcessEvents( void );

private:
  // ProcessEvents() and DoEvent() call this to fire events
  // when an event fires it calls the OnEvent() member of the CListener class
  // as specified by the pFrom when the event was created.
  void FireEvent(EventQueueItem_t *pEI);
};
