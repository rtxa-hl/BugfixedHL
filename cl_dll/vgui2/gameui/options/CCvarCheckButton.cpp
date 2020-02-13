#include <KeyValues.h>
#include "CCvarCheckButton.h"
#include "hud.h"

CCvarCheckButton::CCvarCheckButton(vgui2::Panel *parent, const char *panelName, const char *text, const char *cvarName, bool inverse) :
	vgui2::CheckButton(parent, panelName, text)
{
	m_pCvar = gEngfuncs.pfnGetCvarPointer(cvarName);
	m_bInverse = inverse;
	if (!m_pCvar)
	{
		Msg("%s [CCvarCheckButton]: cvar '%s' not found.\n", panelName, cvarName);
	}
	ResetData();
}

void CCvarCheckButton::ResetData()
{
	if (m_pCvar)
	{
		bool val = !!m_pCvar->value;
		if (m_bInverse)
			val = !val;
		BaseClass::SetSelected(val);
	}
}

void CCvarCheckButton::ApplyChanges()
{
	if (m_pCvar)
	{
		char buf[256];
		bool val = IsSelected();
		if (m_bInverse)
			val = !val;

		snprintf(buf, sizeof(buf), "%s \"%s\"", m_pCvar->name, (val ? "1" : "0"));
		gEngfuncs.pfnClientCmd(buf);
	}
}
