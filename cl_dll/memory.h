/***
*
*	Copyright (c) 2012, AGHL.RU. All rights reserved.
*
****/
//
// Memory.h
//
// Runtime memory searching/patching
//

#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>
#include <stdint.h>

#define MAX_PATTERN 128

typedef struct cl_enginemessages_s
{
	void	(*pfnSvcBad)				(void);
	void	(*pfnSvcNop)				(void);
	void	(*pfnSvcDisconnect)			(void);
	void	(*pfnSvcEvent)				(void);
	void	(*pfnSvcVersion)			(void);
	void	(*pfnSvcSetView)			(void);
	void	(*pfnSvcSound)				(void);
	void	(*pfnSvcTime)				(void);
	void	(*pfnSvcPrint)				(void);
	void	(*pfnSvcStuffText)			(void);
	void	(*pfnSvcSetAngle)			(void);
	void	(*pfnSvcServerInfo)			(void);
	void	(*pfnSvcLightStyle)			(void);
	void	(*pfnSvcUpdateUserInfo)		(void);
	void	(*pfnSvcDeltaDescription)	(void);
	void	(*pfnSvcClientData)			(void);
	void	(*pfnSvcStopSound)			(void);
	void	(*pfnSvcPings)				(void);
	void	(*pfnSvcParticle)			(void);
	void	(*pfnSvcDamage)				(void);
	void	(*pfnSvcSpawnStatic)		(void);
	void	(*pfnSvcEventReliable)		(void);
	void	(*pfnSvcSpawnBaseline)		(void);
	void	(*pfnSvcTempEntity)			(void);
	void	(*pfnSvcSetPause)			(void);
	void	(*pfnSvcSignonNum)			(void);
	void	(*pfnSvcCenterPrint)		(void);
	void	(*pfnSvcKilledMonster)		(void);
	void	(*pfnSvcFoundSecret)		(void);
	void	(*pfnSvcSpawnStaticSound)	(void);
	void	(*pfnSvcIntermission)		(void);
	void	(*pfnSvcFinale)				(void);
	void	(*pfnSvcCdTrack)			(void);
	void	(*pfnSvcRestore)			(void);
	void	(*pfnSvcCutscene)			(void);
	void	(*pfnSvcWeaponAnim)			(void);
	void	(*pfnSvcDecalName)			(void);
	void	(*pfnSvcRoomType)			(void);
	void	(*pfnSvcAddAngle)			(void);
	void	(*pfnSvcNewUserMsg)			(void);
	void	(*pfnSvcPacketEntites)		(void);
	void	(*pfnSvcDeltaPacketEntites)	(void);
	void	(*pfnSvcChoke)				(void);
	void	(*pfnSvcResourceList)		(void);
	void	(*pfnSvcNewMoveVars)		(void);
	void	(*pfnSvcResourceRequest)	(void);
	void	(*pfnSvcCustomization)		(void);
	void	(*pfnSvcCrosshairAngle)		(void);
	void	(*pfnSvcSoundFade)			(void);
	void	(*pfnSvcFileTxferFailed)	(void);
	void	(*pfnSvcHltv)				(void);
	void	(*pfnSvcDirector)			(void);
	void	(*pfnSvcVoiceInit)			(void);
	void	(*pfnSvcVoiceData)			(void);
	void	(*pfnSvcSendExtraInfo)		(void);
	void	(*pfnSvcTimeScale)			(void);
	void	(*pfnSvcResourceLocation)	(void);
	void	(*pfnSvcSendCvarValue)		(void);
	void	(*pfnSvcSendCvarValue2)		(void);
} cl_enginemessages_t;

struct UserMessage
{
	int messageId;
	int messageLen;
	char messageName[16];
	UserMessage *nextMessage;
};

struct CGameConsole003
{
	int v_table;
	int activated;
	void *panel;
};

namespace Memory
{
	// Common
	// Converts HEX string containing pairs of symbols 0-9, A-F, a-f with possible space splitting into byte array
	size_t ConvertHexString(const char *srcHexString, unsigned char *outBuffer, size_t bufferSize);
	size_t MemoryFindForward(size_t start, size_t end, const unsigned char *pattern, const unsigned char *mask, size_t pattern_len);
	// Signed char versions assume pattern and mask are in HEX string format and perform conversions
	size_t MemoryFindForward(size_t start, size_t end, const char *pattern, const char *mask);
	size_t MemoryFindBackward(size_t start, size_t end, const unsigned char *pattern, const unsigned char *mask, size_t pattern_len);
	// Signed char versions assume pattern and mask are in HEX string format and perform conversions
	size_t MemoryFindBackward(size_t start, size_t end, const char *pattern, const char *mask);

	// Platform
	void OnHudInit();
	void OnFrame();
	void HookSvcMessages(cl_enginemessages_t *pEngineMessages);
	void UnHookSvcMessages(cl_enginemessages_t *pEngineMessages);
}
/*
Other functions defined in memory_???.cpp:
	RGBA SetConsoleColor(RGBA color);
*/

extern void **g_EngineBuf;
extern int *g_EngineBufSize;
extern int *g_EngineReadPos;
extern UserMessage **g_pUserMessages;
extern bool g_bNewerBuild;

#endif MEMORY_H
