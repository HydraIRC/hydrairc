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

// maintains a list of in-use unique id's
// set EndID to -1 when not in use.
// obtainid returns -1 if no id's are left.
class IDManager
{
private:
  CSimpleArray<int> m_InUseIDs;
  int m_StartID;
  int m_EndID;

public:
  IDManager( void )
  {
    m_StartID = 0;
    m_EndID = -1;
  }

  IDManager( int StartID, int EndID = -1 )
  {
    m_StartID = StartID;
    m_EndID = EndID;
  }
  
  void SetIDRange( int StartID, int EndID = -1 )
  {
    ATLASSERT(m_InUseIDs.GetSize() == 0); // should only be done before the first ObtainID call.
    m_StartID = StartID;
    m_EndID = EndID;
  }

  ~IDManager( void )
  {
    RelinquishAll(); 
  }

  int ObtainID(void)
  {
    int ID = m_StartID;
    while (m_InUseIDs.Find(ID) != -1) 
      ID++;

    if (m_EndID != -1 && ID > m_EndID)
      return -1;

    m_InUseIDs.Add(ID);
    return ID;
  }

  BOOL IsValid(int ID)
  {
    return (m_InUseIDs.Find(ID) >= 0);
  }

  void RelinquishAll( void )
  {
    m_InUseIDs.RemoveAll();
  }

  void RelinquishID(int ID)
  {
#ifdef DEBUG
    int num = m_InUseIDs.Find(ID);
    ATLASSERT(num >= 0); // remove attempted with invalid id.
    if (num >= 0)
    {
      m_InUseIDs.RemoveAt(num);
    }
#else
    m_InUseIDs.Remove(ID);
#endif
  }
};

