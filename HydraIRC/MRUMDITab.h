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

template< class TTabCtrl >

class CHydraMDITabOwner :
public CMDITabOwner<TTabCtrl>
{
  public:
    // Expose the type of tab control
    typedef TTabCtrl TTabCtrl;

  protected:
    typedef CMDITabOwner<TTabCtrl> baseClass;
    typedef CHydraMDITabOwner<TTabCtrl> thisClass;

    // Message Handling
  public:
    DECLARE_WND_CLASS(_T("HydraMdiTabOwner"))

    BEGIN_MSG_MAP(CMDITabOwner)
      NOTIFY_CODE_HANDLER(NM_CLICK, OnClick)
      CHAIN_MSG_MAP(baseClass)
    END_MSG_MAP()

  LRESULT OnClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
  {
    bHandled = TRUE;

    NMCTCITEM* pnmTC = (NMCTCITEM*)pnmh;
    if(pnmTC)
    {
      if(pnmTC->iItem >= 0)
      {
        if(pnmTC->iItem != 0)
        {
          m_TabCtrl.MoveItem(pnmTC->iItem, 0U, false, false);
        }

        m_TabCtrl.SetFocus();
        m_TabCtrl.SetCurSel(0U);
      }
    }

    // Return TRUE so that default handling doesn't occur
    // (we've already taken care of it)
    return TRUE;
  }
};

