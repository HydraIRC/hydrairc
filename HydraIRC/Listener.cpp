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

#include "StdAfx.h"
#include "HydraIRC.h"

CListener::CListener(CEventManager *pEventManager)
{
  Start(pEventManager);
}

CListener::CListener(void)
{
  m_pEventManager = NULL;
}

void CListener::Start(CEventManager *pEventManager)
{
  ATLASSERT(pEventManager); // passed a NULL
  m_pEventManager = pEventManager;
  m_pEventManager->AddListener(this);
}

void CListener::Stop(void)
{
  if (m_pEventManager)
    m_pEventManager->RemoveListener(this);
  m_pEventManager = NULL;
}

CListener::~CListener(void)
{
  Stop();
}
