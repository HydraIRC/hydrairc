/*

  HydraIRC
  Copyright (C) 2002-2026 Dominic Clifton aka Hydra

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

