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

// simple CNode wrapper that stores an ObjectID, so we just use m_Data and m_ObjectID
class ObjectItem : public CNode
{
public:
  int m_ObjectID;
};

// ObjectManager is a simple class to return objects given an object id.
// so it functions like an array, but without a linear index
// object ID's are just ints starting from 0.
// if an object is freed then so is it's id. so make sure you only store the
// id of an object for as long as the object exists.

// you also might liken this class to a managed linked list with each node having
// an ID.

// returns objects given a specific ID.
class ObjectManager
{
private:
  IDManager m_IDManager;
  CList m_ObjectList;

public:

  ~ObjectManager( void )
  {
    ATLASSERT(m_ObjectList.IsEmpty()); // you didn't remove your objects..

#ifdef DEBUG
    // you shouldn't need this bit code...

    // remove ojects..
    ObjectItem *pObjectItem;
    while (!m_ObjectList.IsEmpty())
    {
      pObjectItem = (ObjectItem *)m_ObjectList.GetFirst();
      pObjectItem->Remove();
      m_IDManager.RelinquishID(pObjectItem->m_ObjectID);
    }
#endif
  }

  // return TRUE if we're managing some objects
  BOOL IsEmpty( void ) 
  {
    // just a wrapper
    return m_ObjectList.IsEmpty();
  }

  // adds an object to a managed list of objects and returns the object id.
  int AddObject(void *pObject)
  {
    ObjectItem *pObjectItem = new ObjectItem;

    if (!pObjectItem)
      return -1; // failed!

    pObjectItem->m_ObjectID = m_IDManager.ObtainID();
    pObjectItem->m_Data = pObject;
    m_ObjectList.AddTail((CNode *)pObjectItem);

    return pObjectItem->m_ObjectID;
  }

  // removes the first object matching pObject from the list and returns true, or false if it can't find it.
  BOOL RemoveObject(void *pObject)
  {
    ObjectItem *pObjectItem;

    for (pObjectItem = (ObjectItem *)m_ObjectList.GetFirst(); pObjectItem->m_Next != NULL; pObjectItem = (ObjectItem *)pObjectItem->m_Next)
    {
      if (pObjectItem->m_Data != pObject)
        continue;

      pObjectItem->Remove();
      m_IDManager.RelinquishID(pObjectItem->m_ObjectID);
      delete pObjectItem;
      return TRUE;
    }
    return FALSE;
  }

  // returns amount of object matching pObject that were removed from the list.
  int RemoveMatchingObjects(void *pObject)
  {
    int Count = 0;
    ObjectItem *pObjectItem;

    for (pObjectItem = (ObjectItem *)m_ObjectList.GetFirst(); pObjectItem->m_Next != NULL; pObjectItem = (ObjectItem *)pObjectItem->m_Next)
    {
      if (pObjectItem->m_Data != pObject)
        continue;

      pObjectItem->Remove();
      m_IDManager.RelinquishID(pObjectItem->m_ObjectID);
      delete pObjectItem;
      Count ++;
    }
    return Count;
  }

  // removed an object with the ID of ObjectID and returns true if it found it, false otherwise.
  BOOL RemoveObjectByID(int ObjectID)
  {
    ObjectItem *pObjectItem;

    for (pObjectItem = (ObjectItem *)m_ObjectList.GetFirst(); pObjectItem->m_Next != NULL; pObjectItem = (ObjectItem *)pObjectItem->m_Next)
    {
      if (pObjectItem->m_ObjectID != ObjectID)
        continue;

      pObjectItem->Remove();
      m_IDManager.RelinquishID(pObjectItem->m_ObjectID);
      delete pObjectItem;
      return TRUE;
    }
    return FALSE;
  }

  // returns the object ID for a given object, or -1 if it's not found.
  int GetObjectID(void *pObject)
  {
    ObjectItem *pObjectItem;

    for (pObjectItem = (ObjectItem *)m_ObjectList.GetFirst(); pObjectItem->m_Next != NULL; pObjectItem = (ObjectItem *)pObjectItem->m_Next)
    {
      if (pObjectItem->m_Data != pObject)
        continue;

      return pObjectItem->m_ObjectID;
    }
    return -1;
  }

  // returns the object, given the ID, or NULL if the object is not found.
  void *GetObjectByID(int ObjectID)
  {
    ObjectItem *pObjectItem;

    for (pObjectItem = (ObjectItem *)m_ObjectList.GetFirst(); pObjectItem->m_Next != NULL; pObjectItem = (ObjectItem *)pObjectItem->m_Next)
    {
      if (pObjectItem->m_ObjectID != ObjectID)
        continue;

      return pObjectItem->m_Data;
    }
    return NULL;
  }

  // returns the object first NULL if there are no objects
  void *GetFirstObject()
  {
    ObjectItem *pObjectItem = (ObjectItem *)m_ObjectList.GetFirst();
    if (pObjectItem->m_Next != NULL) // empty list ?
      return pObjectItem->m_Data; //no
    else
      return NULL;                // yes
  }

};

