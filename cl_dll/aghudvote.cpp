// Martin Webrant (BulliT)

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "aghudvote.h"
#include "aghudglobal.h"

DECLARE_MESSAGE_PTR(m_Vote, Vote)

void AgHudVote::Init()
{
	HOOK_MESSAGE(Vote);

	m_iFlags = 0;
	m_flTurnoff = 0.0;
	m_iVoteStatus = 0;
	m_iFor = 0;
	m_iAgainst = 0;
	m_iUndecided = 0;
	m_szVote[0] = '\0';
	m_szValue[0] = '\0';
	m_szCalled[0] = '\0';
}

void AgHudVote::VidInit()
{
}

void AgHudVote::Reset(void)
{
	m_iFlags &= ~HUD_ACTIVE;
}

void AgHudVote::Draw(float fTime)
{
	if (gHUD.m_flTime > m_flTurnoff || gHUD.m_iIntermission)
	{
		Reset();
		return;
	}

	char szText[128];

	int r, g, b, a;
	a = 255 * gHUD.GetHudTransparency();
	gHUD.GetHudColor(0, 0, r, g, b);
	ScaleColors(r, g, b, a);

	sprintf(szText, "Vote: %s %s", m_szVote, m_szValue);
	gHUD.DrawHudString(ScreenWidth / 20, ScreenHeight / 8, szText, r, g, b);
	sprintf(szText, "Called by: %s", m_szCalled);
	AgDrawHudString(ScreenWidth / 20, ScreenHeight / 8 + gHUD.m_scrinfo.iCharHeight, ScreenWidth, szText, r, g, b);
	if (Called == m_iVoteStatus)
	{
		sprintf(szText, "For: %d", m_iFor);
		gHUD.DrawHudString(ScreenWidth / 20, ScreenHeight / 8 + gHUD.m_scrinfo.iCharHeight * 2, szText, r, g, b);
		sprintf(szText, "Against: %d ", m_iAgainst);
		gHUD.DrawHudString(ScreenWidth / 20, ScreenHeight / 8 + gHUD.m_scrinfo.iCharHeight * 3, szText, r, g, b);
		sprintf(szText, "Undecided: %d", m_iUndecided);
		gHUD.DrawHudString(ScreenWidth / 20, ScreenHeight / 8 + gHUD.m_scrinfo.iCharHeight * 4, szText, r, g, b);
	}
	else if (Accepted == m_iVoteStatus)
	{
		strcpy(szText, "Accepted!");
		gHUD.DrawHudString(ScreenWidth / 20, ScreenHeight / 8 + gHUD.m_scrinfo.iCharHeight * 2, szText, r, g, b);
	}
	else if (Denied == m_iVoteStatus)
	{
		strcpy(szText, "Denied!");
		gHUD.DrawHudString(ScreenWidth / 20, ScreenHeight / 8 + gHUD.m_scrinfo.iCharHeight * 2, szText, r, g, b);
	}
}

int AgHudVote::MsgFunc_Vote(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	m_iVoteStatus = READ_BYTE();
	m_iFor = READ_BYTE();
	m_iAgainst = READ_BYTE();
	m_iUndecided = READ_BYTE();
	strcpy(m_szVote, READ_STRING());
	strcpy(m_szValue, READ_STRING());
	strcpy(m_szCalled, READ_STRING());

	m_flTurnoff = gHUD.m_flTime + 4; // Hold for 4 seconds.

	if (m_iVoteStatus)
		m_iFlags |= HUD_ACTIVE;
	else
		m_iFlags &= ~HUD_ACTIVE;

	return 1;
}
