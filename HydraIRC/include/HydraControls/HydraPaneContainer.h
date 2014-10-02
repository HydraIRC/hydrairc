#pragma once


// TODO: add protected, m_UsingCustomColors, detect systemsetting change and reset the colors of not using custom colors

class CHydraPaneContainer :
  public CPaneContainerImpl<CHydraPaneContainer>
{
protected:
  BOOL m_GradientFilled;
  BOOL m_AlternateStyle;

public:
  typedef CPaneContainerImpl<CHydraPaneContainer> baseClass;

  DECLARE_WND_CLASS_EX(_T("Hydra_PaneContainer"), 0, -1)

  CHydraPaneContainer() : m_GradientFilled(0), m_AlternateStyle(1)
  {
    ResetColors();
  }

  COLORREF m_TitleColor;
  COLORREF m_TitleGradientColor; // not used if m_GradientFilled is not enabled. 
  COLORREF m_TextColor;          // not used if m_AlternateStyle is not enabled.

  void SetGradientFill(BOOL NewValue) { m_GradientFilled = NewValue; ResetColors(); }
  void SetAlternateStyle(BOOL NewValue) { m_AlternateStyle = NewValue; ResetColors();}

  void ResetColors( void )
  {
    m_TextColor = RGB(0xFF,0xFF,0xFF); // WHITE
    m_TitleColor = ::GetSysColor(COLOR_ACTIVECAPTION);
    m_TitleGradientColor = ::GetSysColor(COLOR_GRADIENTACTIVECAPTION);
    if (!m_GradientFilled)
    {
      m_TextColor = ::GetSysColor(COLOR_WINDOWTEXT);
      m_TitleColor = ::GetSysColor(COLOR_3DFACE);      
    }
  }

  // Overrides
  void DrawPaneTitle(CDCHandle dc);
  void DrawButtonImage(CDCHandle dc, RECT& rcImage, HPEN hPen);
};
