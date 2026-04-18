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

// CList.h : implementation of the CList and CNode classes
//
// CList implements a simple doubly-linked list class.
//
/////////////////////////////////////////////////////////////////////////////

// Example Usage:
/*
  CList MyList;
  CNode *pNode,*pNewNode;
  int num;

  // Add to nodes, to the end of the list
  pNewNode = new CNode((void *)1);
  MyList.AddTail(pNewNode);
  pNewNode = new CNode((void *)2);
  MyList.AddTail(pNewNode);

  // get the second item
  pNode = MyList.GetAt(1); // pNode->data = 2

  // add a node to the head of the list
  pNewNode = new CNode((void *)3);
  MyList.AddHead(pNewNode);

  // get the second item
  pNode = MyList.GetAt(1); // pNode->data = 1

  // insert a new node after the head of the list
  pNewNode = new CNode((void *)4);
  MyList.InsertAfter(MyList.GetFirst(),pNewNode);

  // insert a new node before (at) the head of the list
  pNewNode = new CNode((void *)5);
  MyList.InsertBefore(MyList.GetFirst(),pNewNode);

  pNewNode = new CNode((void *)6);
  MyList.InsertAfter(MyList.GetLast(),pNewNode);

  pNewNode = new CNode((void *)7);
  MyList.InsertBefore(MyList.GetAt(2),pNewNode);

  sys_Printf("Forwards\n");
  for (pNode = MyList.GetFirst(); pNode->m_Next ; pNode = pNode->m_Next)
  {
    num = (int)pNode->m_Data;
    sys_Printf("Num = %d\n",num);
  }

  (MyList.GetAt(3))->Remove(); // remove the 4th node.

  sys_Printf("Forwards\n");
  for (pNode = MyList.GetFirst(); pNode->m_Next ; pNode = pNode->m_Next)
  {
    num = (int)pNode->m_Data;
    sys_Printf("Num = %d\n",num);
  }

  sys_Printf("Backwards\n");
  for (pNode = MyList.GetLast(); pNode->m_Prev ; pNode = pNode->m_Prev)
  {
    num = (int)pNode->m_Data;
    sys_Printf("Num = %d\n",num);
  }

  pNode = MyList.GetAt(MyList.GetCount()); // pNode == NULL; // 6 is out of bounds, index is 0 based remember!

  // Remove all items
  MyList.RemoveAll();

  // Nothing in the list now!

  sys_Printf("Forwards\n");
  for (pNode = MyList.GetFirst(); pNode->m_Next ; pNode = pNode->m_Next)
  {
    // We never get here
    num = (int)pNode->m_Data;
    sys_Printf("Num = %d\n",num);
  }

  sys_Printf("Backwards\n");
  for (pNode = MyList.GetLast(); pNode->m_Prev ; pNode = pNode->m_Prev)
  {
    // We never get here
    num = (int)pNode->m_Data;
    sys_Printf("Num = %d\n",num);
  }

*/

// you can derive from this class, or you can just
// use it as-is.

class CNode
{
public:
  CNode *m_Prev;
  CNode *m_Next;
  void *m_Data;

  CNode( void )
  {
    m_Prev = NULL;
    m_Next = NULL;
    m_Data = NULL;
  }

  CNode( void *Data )
  {
    m_Prev = NULL;
    m_Next = NULL;
    m_Data = Data;
  }

  BOOL InList( void )
  {
    return (m_Prev != NULL && m_Next != NULL);
  }

  void Remove( void )
  {
    if (InList())
    {
      m_Prev->m_Next = m_Next;
      m_Next->m_Prev = m_Prev;
    }
  }

  void Delete( void )
  {
    Remove();
    delete this;
  }
};

class CList
{
private:
  CNode m_Head;
  CNode m_Tail;
  CNode *m_pHead;
  CNode *m_pTail;
public:

  CList( void )
  {
    m_pHead = &m_Head;
    m_pTail = &m_Tail;
    m_pHead->m_Next = m_pTail;
    m_pTail->m_Prev = m_pHead;
  }

  void InsertAfter(CNode *pNode,CNode *pNewNode)
  {
    CNode *pNext = pNode->m_Next;

    pNode->m_Next = pNewNode;
    pNewNode->m_Prev = pNode;
    pNewNode->m_Next = pNext;
    pNext->m_Prev = pNewNode;
  }

  void InsertBefore(CNode *pNode,CNode *pNewNode)
  {
    CNode *pPrev = pNode->m_Prev;

    pNode->m_Prev = pNewNode;
    pNewNode->m_Next = pNode;
    pNewNode->m_Prev = pPrev;
    pPrev->m_Next = pNewNode;
  }

  void AddTail(CNode *pNode)
  {
    InsertBefore(m_pTail,pNode);
    /*
    CNode *pOldTail = m_pTail->Prev;
    pOldTail->Next = pNode;
    pNode->Prev = pOldTail;
    pNode->Next = m_pTail;
    m_pTail->Prev = pNode;
    */
  }

  void AddHead(CNode *pNode)
  {
    InsertAfter(m_pHead,pNode);
    /*
    CNode *pOldHead = m_pHead->Next;
    pOldHead->Prev = pNode;
    pNode->Next = pOldHead;
    pNode->Prev = m_pHead;
    m_pHead->Next = pNode;
    */
  }

  CNode *GetFirst( void )
  {
    return m_pHead->m_Next;
  }

  CNode *GetLast( void )
  {
    return m_pTail->m_Prev;
  }

  CNode *GetAt( int Index ) // 0 based index
  {
    CNode *pNode = m_pHead->m_Next;
    while (Index-- > 0 && pNode)
    {
      pNode = pNode->m_Next;
    } 

    // reached the end of the list ?
    if (pNode == m_pTail)
      return NULL;

    return pNode;
  }

  int GetCount( void )
  {
    int Count;
    CNode *pNode;
    for (Count = 0, pNode = GetFirst(); pNode->m_Next ; pNode = pNode->m_Next, Count ++);    
    return Count;
  }

  BOOL IsEmpty( void )
  {
    return ((BOOL)(m_pHead->m_Next == m_pTail));
  }

  /** Remove the nodes from the list
   *
   *  This resets the list, but leaves the nodes all linked together.
   */

  void RemoveAll( void )
  {
    // unlink the head node from the list
    m_pHead->m_Next->m_Prev = NULL;
    // unlink the tail node from the list
    m_pTail->m_Prev->m_Next = NULL;

    m_pHead->m_Next = m_pTail;
    m_pTail->m_Prev = m_pHead;
  }

};
