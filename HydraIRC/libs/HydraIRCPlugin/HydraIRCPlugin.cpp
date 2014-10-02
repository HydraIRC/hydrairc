/*

  HydraIRC - plugin link-library

  TODO: Stick LGPL license here, for now we're using the HydraIRC Limited License

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

#include <atlbase.h>
#include <windows.h>
#include <commctrl.h>

#include <CList.h>

#include <HydraIRCCommon.h>
#include <HydraIRCPlugin.h>
#include <API_Basic.h>
#include <API_CommandProcessor.h>

HydraIRCPlugin::HydraIRCPlugin( void )
{
#ifdef DEBUG
  OutputDebugString("HydraIRCPlugin::HydraIRCPlugin() Called");
#endif
  m_Name = NULL;
  m_Active = FALSE;
  m_Resolved = FALSE;  
  m_pDLL = NULL;
  m_FileName = NULL;
/*
#ifdef DEBUG
  // Memory allocation testing
  m_AllocTest1 = NULL;
  m_AllocTest2 = NULL;
#endif
*/
}

HydraIRCPlugin::~HydraIRCPlugin( void )
{
#ifdef DEBUG
  OutputDebugString("HydraIRCPlugin::~HydraIRCPlugin() Called for ");
  OutputDebugString(m_Name ? m_Name : "Uninitialised Plugin");
  OutputDebugString("\n");
#endif

  CNode *pNode;
  APIDescriptor_t *APID;

  while (!m_APIList.IsEmpty())
  {
    pNode = m_APIList.GetFirst();
    APID = (APIDescriptor_t *)pNode->m_Data;

    if (APID->m_Type == PLUGIN_PROVIDES && APID->m_pTable) 
      free(APID->m_pTable);

    free(APID);
    pNode->Remove();
    delete pNode;
  }  

  m_Name = NULL;
}

BOOL HydraIRCPlugin::AddAPI(int API, char *Name, int TableSize, int Type, void *pTable)
{
#ifdef DEBUG
  OutputDebugString("HydraIRCPlugin::AddAPI() Called\n");
#endif

  int CorrectTableSize = 0; // For *known* API's ONLY!!!
  CNode *pNewNode;

  switch(API)
  {
    case PLUGIN_API_CMDPROC:
      CorrectTableSize = sizeof(PluginCmdProcTable);
      break;
    case PLUGIN_API_BASIC:
      CorrectTableSize = sizeof(PluginBasicTable);
      break;

    default:
      // we don't know this API, so we can't check the table size is correct.
      CorrectTableSize = TableSize;
  }

  if (TableSize != CorrectTableSize)
  {
    //sys_Printf(BIC_ERROR,"Plugin table size mismatch for %s (%d != %d)\n",Name,TableSize, CorrectTableSize);
    return FALSE;
  }

  APIDescriptor_t *APID = (APIDescriptor_t *) malloc( sizeof(APIDescriptor_t));
  if (!APID)
    return FALSE;

  APID->m_Name   = Name;
  APID->m_Type   = Type;
  APID->m_API    = API;
  APID->m_Size   = CorrectTableSize;

  if (APID->m_Type == PLUGIN_PROVIDES)
  {
    APID->m_pTable = malloc(CorrectTableSize);
  }
  else
  {
    APID->m_pTable = pTable;
  }

  ZeroMemory(APID->m_pTable,CorrectTableSize); // <- very important!

  APID->m_TableInitDone = FALSE;

  pNewNode = new CNode(APID);
  if (pNewNode)
  {
    m_APIList.AddTail(pNewNode);
    return TRUE;
  }
  return FALSE;
}

void HydraIRCPlugin::Init( void )
{
#ifdef DEBUG
  OutputDebugString("HydraIRCPlugin::Init() Called\n");
#endif

  m_Active = TRUE;

/*
#ifdef DEBUG
  // Testing, makes sure the dll can free memory allocated via the core
  m_AllocTest1 = malloc(100);
#endif
*/

  // Call the plugin's init function if it has one
  if (&HydraIRCPlugin::OnActivate != NULL)
    OnActivate();

/*
#ifdef DEBUG
  // Testing, makes sure the core can free memory allocated via the dll
  free(m_AllocTest2);
#endif
*/
}

