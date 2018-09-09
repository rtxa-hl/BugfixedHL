#ifndef CHUDSCORES_H
#define CHUDSCORES_H

#include "CHudBase.h"

class CScorePanel;

struct HudScoresData
{
	char szScore[64];
	int r, g, b;
};

class CHudScores : public CHudBase
{
public:
	CScorePanel * m_pScorePanel = nullptr;

	int Init(void);
	int VidInit(void);
	int Draw(float flTime);
	void ShowScoreBoard();
	void HideScoreBoard();

private:
	HudScoresData m_ScoresData[MAX_PLAYERS] = {};
	int m_iLines = 0;
	int m_iOverLay = 0;
	float m_flScoreBoardLastUpdated = 0;
	cvar_t* m_pCvarHudScores = nullptr;
	cvar_t* m_pCvarHudScoresPos = nullptr;
};

#endif