#include <vgui/IInputInternal.h>
#include "vgui2/CClientMOTD.h"
#include "CScorePanel.h"
#include "CHudViewport.h"
#include <vgui/ISurface.h>
#include "hud.h"
#include "CHudScoreBoard.h"
#include "gameui/GameUIPanelNames.h"
#include "IEngineVgui.h"

void CHudViewport::ApplySchemeSettings(vgui2::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);

	SetPaintBackgroundEnabled(false);

	extern vgui2::HFont g_HudTextVgui_TextFont;
	g_HudTextVgui_TextFont = pScheme->GetFont("Default");
}

void CHudViewport::Start()
{
	BaseClass::Start();
}

void CHudViewport::HideScoreBoard()
{
	BaseClass::HideScoreBoard();
	gHUD.m_ScoreBoard->HideScoreBoard(true);
}

void CHudViewport::ActivateClientUI()
{
	BaseClass::ActivateClientUI();
	if (gHUD.m_iIntermission)
		gHUD.m_ScoreBoard->ShowScoreBoard();
}

void CHudViewport::HideClientUI()
{
	BaseClass::HideClientUI();
}

void CHudViewport::CreateDefaultPanels()
{
	AddNewPanel( CreatePanelByName( VIEWPORT_PANEL_MOTD ) );
	AddNewPanel( CreatePanelByName( VIEWPORT_PANEL_SCORE ) );
}

IViewportPanel* CHudViewport::CreatePanelByName( const char* pszName )
{
	IViewportPanel* pPanel = nullptr;

	if (Q_strcmp( VIEWPORT_PANEL_MOTD, pszName ) == 0)
	{
		pPanel = new CClientMOTD( this );
	}
	else if (Q_strcmp(VIEWPORT_PANEL_SCORE, pszName) == 0)
	{
		pPanel = new CScorePanel(this);
	}

	return pPanel;
}
