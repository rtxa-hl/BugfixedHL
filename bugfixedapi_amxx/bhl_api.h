#ifndef BHL_API_H
#define BHL_API_H
#include <interface.h>
#include <IBugfixedServer.h>
#include <CGameVersion.h>

enum class E_ApiInitResult
{
	Ok = 0,
	ModuleNotFound,		// Sys_LoadModule returned nullptr
	FactoryNotFound,	// Sys_GetFactory returned nullptr
	InterfaceNotFound,	// Factory returned nullptr
	VersionMismatch		// GetInterfaceVersion returned incompatible version
};

/**
 * @returns pointer to server API or nullptr
 */
bhl::IBugfixedServer *serverapi();

/**
 * Return true if API is available.
 */
bool IsServerApiReady();

/**
 * Initializes server API.
 * Uses functions from interface.h to get API pointer and check the version.
 *
 * If result is VersionMismatch, use GetServerApiVersion to get the server version.
 */
E_ApiInitResult InitServerApi();

/**
 * Deinitializes server API.
 */
void DeinitServerApi();

/**
 * Sets major and minor to the oversion of the server.
 * Only works if IsServerApiReady() == true or InitServerApi() returned VersionMismatch.
 * Othervise variables are not changed.
 */
void GetServerApiVersion(int &major, int &minor);

#endif