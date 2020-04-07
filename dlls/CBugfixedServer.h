#ifndef CBUGFIXEDSERVER_H
#define CBUGFIXEDSERVER_H

#include <CGameVersion.h>
#include <IBugfixedServer.h>
#include "eiface.h"
#include "progdefs.h"

#ifndef BUGFIXEDAPI_REQUEST_BASE
#define BUGFIXEDAPI_REQUEST_BASE 1
#endif

/**
 * Implementation of IBugfixedServer
 * @see IBugfixedServer
 */
class CBugfixedServer : public bhl::IBugfixedServer
{
public:
	CBugfixedServer();
	void Init();
	void ClientConnect(edict_t *pEntity);
	void PlayerPostThink(edict_t *pEntity);
	void CvarValueCallback(const edict_t *pEnt, int requestID, const char *cvarName, const char *value);

private:
	cvar_t *m_pLanCvar = nullptr;
	cvar_t m_NoQueryCvar = { "sv_disable_cvar_query", "0", 0 };

	enum E_RequestType
	{
		REQUEST_VERSION = 0,
		REQUEST_SUPPORTS,
		REQUEST_COLOR
	};

	enum E_RequestId
	{
		REQUESTID_VERSION = 0,
		REQUESTID_SUPPORTS,
		REQUESTID_COLOR,
		REQUESTID_END
	};

	struct bhl_client_info_t
	{
		CGameVersion version;
		bhl::E_ClientSupports supports = bhl::E_ClientSupports::None;
		bool isColorEnabled = false;

		bool isAuthed = false;
		float nextAuthCheck = 0;
	};

	bhl_client_info_t m_pClientInfo[MAX_PLAYERS + 1];
	CGameVersion m_ServerVersion;
	bhl::E_MotdType m_nMotdType = bhl::E_MotdType::All;
	
	void ResetPlayerData(int idx);
	void QueryClientCvars(edict_t *pEntity);

public:
	//----------------------------------------------------------------------------------------------------
	// IBugfixedServer methods
	//----------------------------------------------------------------------------------------------------
	virtual void GetInterfaceVersion(int &major, int &minor);
	virtual CGameRules **GetGameRulesPtr();
	virtual const IGameVersion *GetServerVersion();
	virtual bhl::E_ClientSupports GetClientSupports(int idx);
	virtual bool GetColorSupport(int idx);
	virtual bool IsClientVersionValid(int idx);
	virtual const IGameVersion *GetClientVersion(int idx);
	virtual bool GetAutomaticMotd(bhl::E_MotdType type);
	virtual void SetAutomaticMotd(bhl::E_MotdType type, bool state);
	virtual void ShowMotdFromString(bhl::E_MotdType type, int idx, const char *str);
	virtual void ShowMotdFromFile(bhl::E_MotdType type, int idx, const char *file);
};

CBugfixedServer *serverapi();

#endif
