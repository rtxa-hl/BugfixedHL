//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef VGUI1_LOWEREDBORDER_H
#define VGUI1_LOWEREDBORDER_H

#include<VGUI.h>
#include<VGUI_Border.h>

namespace vgui
{

class Panel;

class VGUIAPI LoweredBorder : public Border
{
public:
	LoweredBorder();
protected:
	virtual void paint(Panel* panel);
};

}

#endif