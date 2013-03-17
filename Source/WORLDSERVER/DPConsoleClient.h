#ifndef __DPCONSOLECLIENT_H__
#define __DPCONSOLECLIENT_H__

#include "DPMng.h"
#include "MsgHdr.h"
#include "WorldServer.h"
#include "misc.h"

#undef	theClass
#define theClass	CDPConsoleClient
#undef theParameters
#define theParameters CAr & ar, DPID, DPID, OBJID objid

class CDPConsoleClient : public CDPMng
{
private:
	WSAEVENT	m_hWait;
	u_long		m_uRecharge;
	BOOL		m_bAlive;

public:
	CDPConsoleClient();
	virtual	~CDPConsoleClient();

	virtual	void	SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );
	virtual	void	UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );

	BOOL	Run( LPSTR lpszAddr, USHORT uPort );

protected:
	USES_PFNENTRIES;
};

#endif