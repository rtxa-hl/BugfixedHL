#ifndef MULTIMODE_GAMERULES_H
#define MULTIMODE_GAMERULES_H
#include "cdll_dll.h"
#include "skill.h"
#include "multimode.h"

class CHalfLifeMultimode : public CHalfLifeMultiplay
{
public:
	using BaseClass = CHalfLifeMultiplay;

	enum class State
	{
		Invalid = 0,
		Waiting,
		Warmup,
		FreezeTime,
		Game,
		Endgame,
		Intermission
	};

	CHalfLifeMultimode();
	virtual ~CHalfLifeMultimode();

	void SwitchToWaiting();
	void SwitchToWarmup();
	void SwitchToNextMode();
	void SwitchToEndgame();

	void BeginCurMode(bool bEnableFreezeTime, bool bShowModeInfo);
	void StartCurMode();
	void FinishCurMode();

	bool IsSpectator(CBasePlayer *pPlayer);

	virtual const char *GetGameDescription();

	void Think();
	virtual void ClientDisconnected(edict_t *pClient);
	virtual float FlPlayerFallDamage(CBasePlayer *pPlayer);
	virtual BOOL  FPlayerCanTakeDamage(CBasePlayer *pPlayer, CBaseEntity *pAttacker);
	virtual void PlayerSpawn(CBasePlayer *pPlayer);
	virtual void PlayerThink(CBasePlayer *pPlayer);
	virtual BOOL FPlayerCanRespawn(CBasePlayer *pPlayer);
	virtual float FlPlayerSpawnTime(CBasePlayer *pPlayer);
	virtual edict_t *GetPlayerSpawnSpot(CBasePlayer *pPlayer);
	virtual int IPointsForKill(CBasePlayer *pAttacker, CBasePlayer *pKilled);
	virtual void PlayerKilled(CBasePlayer *pVictim, entvars_t *pKiller, entvars_t *pInflictor);

	// Weapon retrieval
	virtual BOOL CanHavePlayerItem(CBasePlayer *pPlayer, CBasePlayerItem *pWeapon);// The player is touching an CBasePlayerItem, do I give it to him?
	virtual void PlayerGotWeapon(CBasePlayer *pPlayer, CBasePlayerItem *pWeapon);// Called each time a player picks up a weapon from the ground

	// Weapon spawn/respawn control
	virtual int WeaponShouldRespawn(CBasePlayerItem *pWeapon);// should this weapon respawn?
	virtual float FlWeaponRespawnTime(CBasePlayerItem *pWeapon);// when may this weapon respawn?
	virtual float FlWeaponTryRespawn(CBasePlayerItem *pWeapon); // can i respawn now,  and if not, when should i try again?
	virtual Vector VecWeaponRespawnSpot(CBasePlayerItem *pWeapon);// where in the world should this weapon respawn?
	
	// Item retrieval
	virtual BOOL CanHaveItem(CBasePlayer *pPlayer, CItem *pItem);// is this player allowed to take this item?
	virtual void PlayerGotItem(CBasePlayer *pPlayer, CItem *pItem);// call each time a player picks up an item (battery, healthkit, longjump)

	// Item spawn/respawn control
	virtual int ItemShouldRespawn(CItem *pItem);// Should this item respawn?
	virtual float FlItemRespawnTime(CItem *pItem);// when may this item respawn?
	virtual Vector VecItemRespawnSpot(CItem *pItem);// where in the world should this item respawn?

	// Ammo retrieval
	virtual BOOL CanHaveAmmo(CBasePlayer *pPlayer, const char *pszAmmoName, int iMaxCarry);// can this player take more of this ammo?
	virtual void PlayerGotAmmo(CBasePlayer *pPlayer, char *szName, int iCount);// called each time a player picks up some ammo in the world

	// Ammo spawn/respawn control
	virtual int AmmoShouldRespawn(CBasePlayerAmmo *pAmmo);// should this ammo item respawn?
	virtual float FlAmmoRespawnTime(CBasePlayerAmmo *pAmmo);// when should this ammo item respawn?
	virtual Vector VecAmmoRespawnSpot(CBasePlayerAmmo *pAmmo);// where in the world should this ammo item respawn?
																			// by default, everything spawns

	// Healthcharger respawn control
	virtual float FlHealthChargerRechargeTime();// how long until a depleted HealthCharger recharges itself?
	virtual float FlHEVChargerRechargeTime();// how long until a depleted HealthCharger recharges itself?

	// What happens to a dead player's weapons
	virtual int DeadPlayerWeapons(CBasePlayer *pPlayer);// what do I do with a player's weapons when he's killed?

	// What happens to a dead player's ammo	
	virtual int DeadPlayerAmmo(CBasePlayer *pPlayer);// Do I drop ammo when the player dies? How much?

	// After primary attack
	void OnPrimaryAttack(CBasePlayer *pPlayer, CBasePlayerItem *pWeapon);

private:
	CBaseMode *m_pModes[(int)ModeID::ModeCount] = {};
	skilldata_t m_DefSkillData;
	hudtextparms_t m_WarmupTextParams;
	hudtextparms_t m_TimerTextParams;
	hudtextparms_t m_ModeTitleTextParams;
	hudtextparms_t m_ModeInfoTextParams;

	State m_State = State::Invalid;
	float m_flNextTimerUpdate = 0;

	// Warmup
	CBaseMode *m_pWarmupMode = nullptr;
	float m_flWarmupEndTime = 0;

	// Freeze time
	float m_flFreezeEndTime = 0;
	int m_iFreezeNextSec = 0;
	bool m_bFreezeOnSpawn = false;

	// Game time
	CBaseMode *m_pCurMode = nullptr;
	ModeID m_CurModeId = ModeID::None;
	float m_flEndTime = 0;

	CHalfLifeMultimode(const CHalfLifeMultimode &&) = delete;
	CHalfLifeMultimode &operator=(const CHalfLifeMultimode &&) = delete;

	void ResetTimerUpdate();

	friend bool IsRunningMultimode(ModeID mode);
};

#endif
