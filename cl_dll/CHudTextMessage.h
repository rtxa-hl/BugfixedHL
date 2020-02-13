#ifndef CHUDTEXTMESSAGE_H
#define CHUDTEXTMESSAGE_H

#include "CHudBase.h"

class CHudTextMessage : public CHudBase
{
public:
	void Init();
	static char *LocaliseTextString(const char *msg, char *dst_buffer, int buffer_size);
	static char *BufferedLocaliseTextString(const char *msg);
	char *LookupString(const char *msg_name, int *msg_dest = NULL);
	int MsgFunc_TextMsg(const char *pszName, int iSize, void *pbuf);
};

#endif