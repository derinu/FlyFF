#include "StdAfx.h"
#ifdef __DDOM
#include "User.h"
#include "DDom.h"
#include "DDomQueue.h"

#include "DPCoreClient.h"
extern	CDPCoreClient		g_DPCoreClient;

void CDDomQueue::Add( CUser* pUser )
{
	if( IsValidObj( pUser ) )
	{
		if( CDDom::GetInstance().GetRunTime() != RUN_CLOSED )
		{
			pUser->AddTextD3D( "An instance is already running.", 0xFFFF0000 );
			return;
		}
		if( m_PlayerQueue.size() == DoubleDom::Queue::nAmount )
		{
			pUser->AddTextD3D( "The queue is full.", 0xFFFF0000 );
			return;
		}
		vector<DOMQPL>::iterator it = find( m_PlayerQueue.begin(), m_PlayerQueue.end(), pUser->m_idPlayer );
		if( it != m_PlayerQueue.end() )
		{
			pUser->AddTextD3D( "You're already queued.", 0xFFFF0000 );
			return;
		}

		//wtf?
		//CDDom::GetInstance().Safe( pUser ); 

		DOMQPL domPlayer;
		domPlayer.idPlayer = pUser->m_idPlayer;
		domPlayer.nJob = pUser->GetJob();
		domPlayer.nLevel = pUser->GetLevel();
		//domPlayer.nRebirth = pUser->m_nCampusPoint;
		domPlayer.szName = pUser->GetName();
		//domPlayer.WorldId =  pUser->GetWorld() != NULL ? pUser->GetWorld()->GetID() : WI_WORLD_MADRIGAL;
		m_PlayerQueue.push_back( domPlayer );

		pUser->AddTextD3D( "You have been queued to a Double Domination match!", 0xFF8CFA05 );
		if( m_PlayerQueue.size() == DoubleDom::Queue::nAmount && CDDom::GetInstance().GetRunTime() == RUN_CLOSED )
		{
			vector<u_long> playerList;
			for( vector<DOMQPL>::iterator it = m_PlayerQueue.begin(); it != m_PlayerQueue.end(); ++it )
				playerList.push_back( it->idPlayer );

			CDDom::GetInstance().Start( playerList );
			m_PlayerQueue.clear();

		}
		else
		{
			Send();
		}

		if(m_PlayerQueue.size() == (DoubleDom::Queue::nAmount/2))
			g_DPCoreClient.SendSystem("Double Domination is filling up! Go join in!");
	}
}

void CDDomQueue::Send( void )
{
	for( vector<DOMQPL>::iterator it = m_PlayerQueue.begin(); it != m_PlayerQueue.end(); ++it )
	{
		CUser* pUser		= static_cast<CUser*>( prj.GetUserByID( it->idPlayer ) );
		if( IsValidObj( pUser ) )
		{
			pUser->SendDDomQueue();
		}
	}
}

void CDDomQueue::Serialize( CAr & ar )
{
	ar << m_PlayerQueue.size();
	for( vector<DOMQPL>::iterator it = m_PlayerQueue.begin(); it != m_PlayerQueue.end(); ++it )
	{
		CUser* pUser		= static_cast<CUser*>( prj.GetUserByID( it->idPlayer ) );
		if( IsValidObj( pUser ) )
		{
			u_long nWorldId = WI_WORLD_MADRIGAL;
			CWorld* pWorld = pUser->GetWorld();
			if( pWorld )
			{
				nWorldId = pWorld->GetID();
			}

			ar << it->nJob;
			ar << it->nLevel;
			//ar << it->nRebirth;
			ar << nWorldId;
			ar.WriteString( LPCTSTR( it->szName ) );
		}
		else
		{
			ar << 0 << 0 << WI_WORLD_MADRIGAL;
			ar.WriteString( "Unknown" );
		}
	}
}

			
 
void CDDomQueue::Remove( CUser* pUser )
{
	if( IsValidObj( pUser ) )
	{
		vector<DOMQPL>::iterator it = find( m_PlayerQueue.begin(), m_PlayerQueue.end(), pUser->m_idPlayer );
		if( it != m_PlayerQueue.end() )
		{
			m_PlayerQueue.erase( it );
			Send();
		}
	}
}

#endif

		