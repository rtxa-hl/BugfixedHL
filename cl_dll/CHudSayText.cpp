/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// saytext.cpp
//
// implementation of CHudSayText class
//

#include <string.h>
#include <stdio.h>
#include <time.h>

#include "CHudSayText.h"
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "results.h"
#include "vgui_TeamFortressViewport.h"
#include "aghudlocation.h"
#include "dllexport.h"

#ifdef USE_VGUI2
#include "vgui2/CHudChat.h"
#endif

#undef PlaySound

// FIXME: Windows only
#ifndef _WIN32
#define ResultsAddLog(...)
#endif

#define MAX_LINES	5
#define MAX_CHARS_PER_LINE	256  /* it can be less than this, depending on char size */

// allow 20 pixels on either side of the text
#define MAX_LINE_WIDTH  ( ScreenWidth - 40 )
#define LINE_START  10
static float SCROLL_SPEED = 5;

static char g_szLineBuffer[ MAX_LINES + 1 ][ MAX_CHARS_PER_LINE ];
static float *g_pflNameColors[ MAX_LINES + 1 ];
static int g_iNameLengths[ MAX_LINES + 1 ];
static float flScrollTime = 0;  // the time at which the lines next scroll up

static int Y_START = 0;
static int line_height = 0;

DECLARE_MESSAGE_PTR( m_SayText, SayText );

extern "C" void DLLEXPORT ChatInputPosition(int *x, int *y)
{
	if (!gHUD.m_SayText->m_pCvarOldInputPos || !gHUD.m_SayText->m_pCvarOldInputPos->value)
	{
		*x = LINE_START;
		*y = Y_START + line_height * MAX_LINES;
	}
	else
	{
		// Default position
	}
}

void CHudSayText :: Init( void )
{
	HOOK_MESSAGE( SayText );

	InitHUDData();

	m_HUD_saytext = CVAR_CREATE( "hud_saytext", "1", 0 );
	m_HUD_saytext_time = CVAR_CREATE( "hud_saytext_time", "5", 0 );
	m_pCvarConSayColor = CVAR_CREATE( "con_say_color", "30 230 50", FCVAR_BHL_ARCHIVE );
	m_pCvarOldInputPos = CVAR_CREATE("hud_saytext_oldpos", "0", FCVAR_BHL_ARCHIVE);
	m_pCvarSound = CVAR_CREATE("hud_saytext_sound", "1", FCVAR_BHL_ARCHIVE);
#ifdef USE_VGUI2
	m_pCvarOldChat = CVAR_CREATE("hud_saytext_oldchat", "0", FCVAR_BHL_ARCHIVE);
#endif

	m_iFlags |= HUD_INTERMISSION; // is always drawn during an intermission
}


void CHudSayText :: InitHUDData( void )
{
	memset( g_szLineBuffer, 0, sizeof g_szLineBuffer );
	memset( g_pflNameColors, 0, sizeof g_pflNameColors );
	memset( g_iNameLengths, 0, sizeof g_iNameLengths );
}

void CHudSayText :: VidInit( void )
{
	if (ScreenHeight >= 480)
		Y_START = ScreenHeight - 60;
	else
		Y_START = ScreenHeight - 45;
}


int ScrollTextUp( void )
{
	g_szLineBuffer[MAX_LINES][0] = 0;
	memmove( g_szLineBuffer[0], g_szLineBuffer[1], sizeof(g_szLineBuffer) - sizeof(g_szLineBuffer[0]) ); // overwrite the first line
	memmove( &g_pflNameColors[0], &g_pflNameColors[1], sizeof(g_pflNameColors) - sizeof(g_pflNameColors[0]) );
	memmove( &g_iNameLengths[0], &g_iNameLengths[1], sizeof(g_iNameLengths) - sizeof(g_iNameLengths[0]) );
	g_szLineBuffer[MAX_LINES-1][0] = 0;

	if ( g_szLineBuffer[0][0] == ' ' ) // also scroll up following lines
	{
		g_szLineBuffer[0][0] = 2;
		return 1 + ScrollTextUp();
	}

	return 1;
}

void CHudSayText :: Draw( float flTime )
{
	int y = Y_START;

	if ( ( gViewPort && gViewPort->AllowedToPrintText() == FALSE) || !m_HUD_saytext->value )
		return;

	// make sure the scrolltime is within reasonable bounds,  to guard against the clock being reset
	flScrollTime = min( flScrollTime, flTime + m_HUD_saytext_time->value );

	if ( flScrollTime <= flTime )
	{
		if ( *g_szLineBuffer[0] )
		{
			flScrollTime = flTime + m_HUD_saytext_time->value;
			// push the console up
			ScrollTextUp();
		}
		else
		{ // buffer is empty,  just disable drawing of this section
			m_iFlags &= ~HUD_ACTIVE;
		}
	}

	for ( int i = 0; i < MAX_LINES; i++ )
	{
		if ( *g_szLineBuffer[i] )
		{
			if ( *g_szLineBuffer[i] == 2 && g_pflNameColors[i] )
			{
				// it's a saytext string
				static char buf[MAX_PLAYER_NAME+32];

				// draw the first x characters in the player color
				strncpy( buf, g_szLineBuffer[i], min(g_iNameLengths[i], MAX_PLAYER_NAME+32) );
				buf[ min(g_iNameLengths[i], MAX_PLAYER_NAME+31) ] = 0;
				int x = DrawConsoleString( LINE_START, y, buf, g_pflNameColors[i] );

				DrawConsoleString( x, y, g_szLineBuffer[i] + g_iNameLengths[i], NULL );
			}
			else
			{
				// normal draw
				DrawConsoleString( LINE_START, y, g_szLineBuffer[i], NULL );
			}
		}

		y += line_height;
	}
}

int CHudSayText :: MsgFunc_SayText( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	int client_index = READ_BYTE();		// the client who spoke the message
	SayTextPrint( READ_STRING(), iSize - 1,  client_index );

	return 1;
}

void CHudSayText :: SayTextPrint( const char *pszBuf, int iBufSize, int clientIndex )
{
	// Print to the console
	if (pszBuf[0] == 2 && clientIndex > 0 || pszBuf[0] == 1 && clientIndex == 0)
	{
		RGBA color;
		bool colored = ParseColor(m_pCvarConSayColor->string, color);

		// Prepend time for say messages from players and the server
		time_t now;
		time(&now);
		if (now)
		{
			struct tm *current = localtime(&now);
			char time_buf[32];
			sprintf(time_buf, "[%02i:%02i:%02i] ", current->tm_hour, current->tm_min, current->tm_sec);
			if (colored)
				ConsolePrintColor(time_buf, color);
			else
				ConsolePrint(time_buf);
			sprintf(time_buf, "[%04i-%02i-%02i %02i:%02i:%02i] ", current->tm_year + 1900, current->tm_mon + 1, current->tm_mday, current->tm_hour, current->tm_min, current->tm_sec);
			ResultsAddLog(time_buf, true);
		}

		// Cut indicator in first byte
		if (colored)
			ConsolePrintColor(pszBuf + 1, color);
		else
			ConsolePrint(pszBuf + 1);

		int strLen = strlen(pszBuf + 1);
		if (pszBuf[strLen] != '\n')
			ConsolePrint("\n");
		ResultsAddLog(pszBuf + 1, true);
	}
	else
	{
		ConsolePrint(pszBuf);
		ResultsAddLog(pszBuf, false);
	}

	if ( gViewPort && gViewPort->AllowedToPrintText() == FALSE )
	{
		return;
	}

#ifdef USE_VGUI2
	if (m_pCvarOldChat->value)
#endif
	{
		// find an empty string slot
		int i;
		for (i = 0; i < MAX_LINES; i++)
		{
			if (!*g_szLineBuffer[i])
				break;
		}
		if (i == MAX_LINES)
		{
			// force scroll buffer up
			ScrollTextUp();
			i = MAX_LINES - 1;
		}

		g_iNameLengths[i] = 0;
		g_pflNameColors[i] = NULL;

		// if it's a say message, search for the players name in the string
		if (*pszBuf == 2 && clientIndex > 0)
		{
			GetPlayerInfo(clientIndex, &g_PlayerInfoList[clientIndex]);
			const char* pName = g_PlayerInfoList[clientIndex].name;
			if (pName)
			{
				const char* nameInString = strstr(pszBuf, pName);
				if (nameInString)
				{
					g_iNameLengths[i] = strlen(pName) + (nameInString - pszBuf);
					g_pflNameColors[i] = GetClientTeamColor(clientIndex);
				}
			}
		}

		strncpy(g_szLineBuffer[i], pszBuf, max(iBufSize - 1, MAX_CHARS_PER_LINE - 1));

		// Substitute location
		gHUD.m_Location->ParseAndEditSayString(clientIndex, g_szLineBuffer[i], HLARRAYSIZE(g_szLineBuffer[i]));

		// make sure the text fits in one line
		EnsureTextFitsInOneLineAndWrapIfHaveTo(i);

		// Set scroll time
		if (i == 0)
		{
			flScrollTime = gHUD.m_flTime + m_HUD_saytext_time->value;
		}

		m_iFlags |= HUD_ACTIVE;

		if (ScreenHeight >= 480)
			Y_START = ScreenHeight - 60;
		else
			Y_START = ScreenHeight - 45;
		Y_START -= (line_height * (MAX_LINES + 1));
	}
#ifdef USE_VGUI2
	else
	{
		char szBuf[1024];
		strncpy(szBuf, pszBuf, sizeof(szBuf));
		szBuf[sizeof(szBuf) - 1] = '\0';
		gHUD.m_Location->ParseAndEditSayString(clientIndex, szBuf, sizeof(szBuf));	// Substitute location
		gHUD.m_Chat->ChatPrintf(clientIndex, CHAT_FILTER_NONE, "%s", szBuf);
	}
#endif

	if (m_pCvarSound->value)
		PlaySound("misc/talk.wav", 1);
}

void CHudSayText :: EnsureTextFitsInOneLineAndWrapIfHaveTo( int line )
{
	int line_width = 0;
	GetConsoleStringSize( g_szLineBuffer[line], &line_width, &line_height );

	if ( (line_width + LINE_START) > MAX_LINE_WIDTH )
	{ // string is too long to fit on line
		// scan the string until we find what word is too long,  and wrap the end of the sentence after the word
		int length = LINE_START;
		int tmp_len = 0;
		char *last_break = NULL;
		for ( char *x = g_szLineBuffer[line]; *x != 0; x++ )
		{
			// check for a color change, if so skip past it
			if ( x[0] == '/' && x[1] == '(' )
			{
				x += 2;
				// skip forward until past mode specifier
				while ( *x != 0 && *x != ')' )
					x++;

				if ( *x != 0 )
					x++;

				if ( *x == 0 )
					break;
			}

			char buf[2];
			buf[1] = 0;

			if ( *x == ' ' && x != g_szLineBuffer[line] )  // store each line break,  except for the very first character
				last_break = x;

			buf[0] = *x;  // get the length of the current character
			GetConsoleStringSize( buf, &tmp_len, &line_height );
			length += tmp_len;

			if ( length > MAX_LINE_WIDTH )
			{  // needs to be broken up
				if ( !last_break )
					last_break = x-1;

				x = last_break;

				// find an empty string slot
				int j;
				do 
				{
					for ( j = 0; j < MAX_LINES; j++ )
					{
						if ( ! *g_szLineBuffer[j] )
							break;
					}
					if ( j == MAX_LINES )
					{
						// need to make more room to display text, scroll stuff up then fix the pointers
						int linesmoved = ScrollTextUp();
						line -= linesmoved;
						last_break = last_break - (sizeof(g_szLineBuffer[0]) * linesmoved);
					}
				}
				while ( j == MAX_LINES );

				// copy remaining string into next buffer,  making sure it starts with a space character
				if ( (char)*last_break == (char)' ' )
				{
					int linelen = strlen(g_szLineBuffer[j]);
					int remaininglen = strlen(last_break);

					if ( (linelen - remaininglen) <= MAX_CHARS_PER_LINE )
						strcat( g_szLineBuffer[j], last_break );
				}
				else
				{
					if ( (strlen(g_szLineBuffer[j]) - strlen(last_break) - 2) < MAX_CHARS_PER_LINE )
					{
						strcat( g_szLineBuffer[j], " " );
						strcat( g_szLineBuffer[j], last_break );
					}
				}

				*last_break = 0; // cut off the last string

				EnsureTextFitsInOneLineAndWrapIfHaveTo( j );
				break;
			}
		}
	}
}
