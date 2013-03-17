#include "StdAfx.h"
#include "DPConsoleClient.h"

CDPConsoleClient::CDPConsoleClient()
{
	BEGIN_MSG;
	this->ConnectToServer("127.0.0.1", 25652);
}

CDPConsoleClient::~CDPConsoleClient()
{
	CLOSE_HANDLE( m_hWait );
}

void CDPConsoleClient::SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{

}

void CDPConsoleClient::UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom )
{
	CAr ar( (LPBYTE)lpMsg + sizeof(DPID) + sizeof(DPID), dwMsgSize - ( sizeof(DPID) + sizeof(DPID) ) );
	GETTYPE( ar );

	static map<DWORD, CString> mapstrProfile;
	map<DWORD, CString>::iterator it = mapstrProfile.find( dw );
	if( it == mapstrProfile.end() )
	{
		CString strTemp;
		strTemp.Format("CDPConsoleClient::UserMessageHandler(0x%08x)", dw );
		it = mapstrProfile.insert( make_pair( dw, strTemp ) ).first;
	}
	_PROFILE( it->second );

	void ( theClass::*pfn )( theParameters )	=	GetHandler( dw );
	
	if( pfn ) {
		( this->*( pfn ) )( ar, *(UNALIGNED LPDPID)lpMsg, *(UNALIGNED LPDPID)( (LPBYTE)lpMsg + sizeof(DPID) ), NULL_ID );
	}
	else {
		switch( dw )
		{
			case PACKETTYPE_PASSAGE:
			{
				DWORD objid, dwtmp;
				ar >> objid >> dwtmp;

				pfn		= GetHandler( dwtmp );
				ASSERT( pfn != NULL );
				( this->*( pfn ) )( ar, *(UNALIGNED LPDPID)lpMsg, *(UNALIGNED LPDPID)( (LPBYTE)lpMsg + sizeof(DPID) ), (OBJID)objid );
				break;
			}
			case PACKETTYPE_BROADCAST:
				{
					DWORD dwtmp;
					ar >> dwtmp;

					pfn		= GetHandler( dwtmp );
					ASSERT( pfn != NULL );
					( this->*( pfn ) )( ar, *(UNALIGNED LPDPID)lpMsg, *(UNALIGNED LPDPID)( (LPBYTE)lpMsg + sizeof(DPID) ), NULL_ID );
					break;
				}
			default:
				Error( "Handler not found(%08x)\n",dw );
				break;
		}
	}


}

BOOL CDPConsoleClient::Run( LPSTR lpszAddr, USHORT uPort )
{
	::Error("Connecting to %s", lpszAddr);
	if( ConnectToServer( lpszAddr, uPort, TRUE ) )
	{
		return ( WaitForSingleObject( m_hWait, INFINITE ) != WAIT_TIMEOUT );
	}
	return FALSE;
}