#include "StdAfx.h"
#include "hydrapanecontainer.h"

void CHydraPaneContainer::DrawPaneTitle ( CDCHandle dc )
{
  HBRUSH hbrBkgnd;
  RECT rect;
  GetClientRect(&rect);

  if ( !m_AlternateStyle || IsVertical() )
  {
    // let the base class do the default drawing.
    baseClass::DrawPaneTitle ( dc );
  }
  else
  {
    // draw title only for horizontal pane container

    rect.bottom = rect.top + m_cxyHeader;

    dc.SaveDC();
    if (m_GradientFilled)
    {
      // for non-flat childs.
      //dc.DrawEdge(&rect, EDGE_SUNKEN, BF_FLAT | BF_RIGHT | BF_BOTTOM | BF_ADJUST);

      // for flat childs
		  dc.DrawEdge(&rect, EDGE_SUNKEN, BF_FLAT | BF_TOP | BF_LEFT | BF_BOTTOM | BF_RIGHT | BF_ADJUST);

      TRIVERTEX tv[] = { { rect.left,  rect.top,               0,0,0},
                        { rect.right - (m_cxImageTB + m_cxyBtnAddTB + 1), rect.bottom, 0,0,0} };
      GRADIENT_RECT gr = { 0, 1 };

      tv[0].Blue   = (COLOR16) ((m_TitleColor & 0x00ff0000) >> 8);
      tv[0].Green  = (COLOR16) ((m_TitleColor & 0x0000ff00));
      tv[0].Red    = (COLOR16) ((m_TitleColor & 0x000000ff) << 8);

      tv[1].Blue   = (COLOR16) ((m_TitleGradientColor & 0x00ff0000) >> 8);
      tv[1].Green  = (COLOR16) ((m_TitleGradientColor & 0x0000ff00));
      tv[1].Red    = (COLOR16) ((m_TitleGradientColor & 0x000000ff) << 8);

      dc.GradientFill ( tv, 2, &gr, 1, GRADIENT_FILL_RECT_H );
      RECT rightrect = rect;
      rightrect.left = rect.right - (m_cxImageTB + m_cxyBtnAddTB + 1);
      hbrBkgnd = CreateSolidBrush(m_TitleGradientColor);
      dc.FillRect(&rightrect, hbrBkgnd);//COLOR_GRADIENTACTIVECAPTION);      
      DeleteObject(hbrBkgnd);
      dc.SetTextColor(RGB(0xFF,0xFF,0xFF));
    }
    else
    {
      // for non-flat childs.
		  //dc.DrawEdge(&rect, EDGE_SUNKEN, BF_FLAT | BF_TOP | BF_LEFT | BF_BOTTOM | BF_RIGHT | BF_ADJUST);

      // for flat childs
		  dc.DrawEdge(&rect, EDGE_SUNKEN, BF_FLAT | BF_TOP | BF_LEFT | BF_BOTTOM | BF_RIGHT | BF_ADJUST);

      // original
		  //dc.DrawEdge(&rect, EDGE_RAISED, BF_LEFT | BF_TOP | BF_RIGHT | BF_BOTTOM | BF_ADJUST);
      hbrBkgnd = CreateSolidBrush(m_TitleColor);
      dc.FillRect(&rect, hbrBkgnd);//COLOR_3DFACE);      
      DeleteObject(hbrBkgnd);
      dc.SetTextColor(m_TextColor);//::GetSysColor(COLOR_WINDOWTEXT));
    }

		dc.SetBkMode(TRANSPARENT);
		//T* pT = static_cast<T*>(this);
		HFONT hFontOld = dc.SelectFont(/*pT->*/GetTitleFont());
		rect.left += m_cxyTextOffset;
		rect.right -= m_cxyTextOffset;
		if(m_tb.m_hWnd != NULL)
			rect.right -= m_cxToolBar;;
#ifndef _WIN32_WCE
		dc.DrawText(m_szTitle, -1, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
#else // CE specific
		dc.DrawText(m_szTitle, -1, &rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
#endif //_WIN32_WCE
		dc.SelectFont(hFontOld);

    dc.RestoreDC(-1);
  }
}

void CHydraPaneContainer::DrawButtonImage(CDCHandle dc, RECT& rcImage, HPEN hPen)
{
  HBRUSH hbrBkgnd;
  RECT rect;
  GetClientRect(&rect);
  if (m_AlternateStyle)
  { 
    if (m_GradientFilled)
      hbrBkgnd = CreateSolidBrush(m_TitleGradientColor);
    else
      hbrBkgnd = CreateSolidBrush(m_TitleColor);

    dc.FillRect(&rect, hbrBkgnd);
    DeleteObject(hbrBkgnd);
  }
  else
  {
    dc.FillRect(&rect, COLOR_3DFACE);
  }
  baseClass::DrawButtonImage(dc, rcImage, hPen);
}
