#pragma once
#include "atlframe.h"

class CHidingMDIWindow :
  public CMDIWindow
{
public:
	void MDIActivate(HWND hWndChildToActivate);
};
