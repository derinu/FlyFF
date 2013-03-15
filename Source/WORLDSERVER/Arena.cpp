#include "StdAfx.h"
#ifdef __ARENA_PARADISE
#include "CQuery.h"
#include "Arena.h"
#include "Mover.h"
#include "User.h"

BOOL LoadArena( void )
{
	return(  CArena::GetInstance().Load() );
}

ARENA_ADD_TYPE CArena::Add( CUser* pUser )
{
	if( IsValidObj( pUser ) )
	{
		const u_long idPlayer = pUser->m_idPlayer;
		if( idPlayer == 0 )
			return ARENA_ADD_FAILURE;
		if( m_mArenaMap.find( idPlayer ) != m_mArenaMap.end() )
			return ARENA_ADD_ALREADY_EXIST;

		ARENAPLAYER addPlayer;
		addPlayer.szName = pUser->GetName();
		addPlayer.nJob = pUser->GetJob();
		
		m_mArenaMap.insert( make_pair( idPlayer, addPlayer ) );

		if( FALSE == AddToDatabase( idPlayer, pUser->GetName(), pUser->GetJob() ) )
		{
			Error( "CArena::Add --- AddToDatabase return FALSE" );
		}

		return ARENA_ADD_SUCCESS;
	}
	return ARENA_ADD_FAILURE;
}

ARENAPLAYER* CArena::GetPlayer( u_long idPlayer )
{
	map<u_long, ARENAPLAYER>::iterator it = m_mArenaMap.find( idPlayer );
	if( it != m_mArenaMap.end() )
		return &it->second;
	return NULL;
}

void CArena::Serialize( CAr & ar )
{
	ar << m_Online.size();

	for( map<u_long, nomember>::iterator itor = m_Online.begin(); itor != m_Online.end(); ++itor )
	{
		map<u_long, ARENAPLAYER>::iterator it = m_mArenaMap.find( itor->first );
		if( it != m_mArenaMap.end() )
		{
			ar << it->second.nKill;
			ar << it->second.nDeath;
			ar << it->second.nRow;
			ar << it->second.nJob;
			ar << it->second.nDeathMatchWin;
			ar << it->second.nDeathMatchLose;
			ar << it->second.nDuelWin;
			ar << it->second.nDuelLose;
			ar << (unsigned long)it->second.n64ArenaPoint;
			ar.WriteString( LPCTSTR( it->second.szName ) );
		}
		else //map.size() == ar >> size ( --> nomatch = crash ).
		{
			ar << 0;
			ar << 0;
			ar << 0;
			ar << 0;
			ar << 0;
			ar << 0;
			ar << 0;
			ar << 0;
			ar << 0;
			ar.WriteString( "Deleted" );
		}

	}
}
void CArena::Process( void )
{
	if( GetTime() > 0 )
	{
		DecreaseTime();
	}
	else if( GetTime() == 0 )
	{
		SaveToDatabase();
		ResetTime();
	}
}
BOOL CArena::SaveToDatabase( void )
{
	CQuery* pQuery = new CQuery;
	if( pQuery->Connect( 3, "Character01", "", "" ) == FALSE )
	{
		pQuery->DisConnect();
		SAFE_DELETE( pQuery );
		return FALSE;
	}
	
	for( map<u_long, ARENAPLAYER>::iterator it = m_mArenaMap.begin(); it != m_mArenaMap.end(); ++it )
	{
		if( it->second.bUpdate == FALSE ) //nothing has changed, please don't Query me!
			continue;

		it->second.bUpdate = FALSE;

		CString strExecUpdate;
		strExecUpdate.Format( "UPDATE dbo.ARENA SET "
			"m_idPlayer = %u, "
			"m_nKill = %u, "
			"m_nDeath = %u, "
			"m_nRow = %u, "
			"m_nJob = %u, "
			"m_nDeathMatchWin = %u, "
			"m_nDeathMatchLose = %u, "
			"m_nDuelWin = %u, "
			"m_nDuelLose = %u, "
			"m_nArenaPoint = %I64u, "
			"m_szName = '%s' "
			"WHERE m_idPlayer = %u",
			it->first,
			it->second.nKill,
			it->second.nDeath,
			it->second.nRow,
			it->second.nJob,
			it->second.nDeathMatchWin,
			it->second.nDeathMatchLose,
			it->second.nDuelWin,
			it->second.nDuelLose,
			it->second.n64ArenaPoint,
			it->second.szName,
			it->first );

			if( pQuery->Exec( strExecUpdate ) == FALSE )
			{
				Error( "---------- Start of Arena Save Error --------" );
				Error( "CArena --> Save Database Player." );
				Error( "User ID:%u --> Kills:%u --> Death:%u ---> Row:%u",
					it->first, it->second.nKill, it->second.nDeath, it->second.nRow );
				Error( "Query: --> %s", strExecUpdate );
				Error( "---------- End of Arena Save Error --------" );
			}
			pQuery->Clear();
	}

	pQuery->DisConnect();
	SAFE_DELETE( pQuery );
	return TRUE;
}
void CArena::Kill( CUser* pUser, CUser* pDeath )
{
	if( IsValidObj( pUser ) && IsValidObj( pDeath ) )
	{
		map<u_long, ARENAPLAYER>::iterator it = m_mArenaMap.find( pUser->m_idPlayer );
		map<u_long, ARENAPLAYER>::iterator itDead = m_mArenaMap.find( pDeath->m_idPlayer );
		if( it != m_mArenaMap.end() && itDead != m_mArenaMap.end() )
		{
			if( it->second.bUpdate != TRUE ) //do not perform a write multiple times.
				it->second.bUpdate = TRUE;
			if( itDead->second.bUpdate != TRUE )
				itDead->second.bUpdate = TRUE;

			it->second.nKill++;
			it->second.nRow++;

			itDead->second.nDeath++;
			itDead->second.nRow = 0;

			it->second.n64ArenaPoint += GetPointMultiplierFactor();

			CString strFormat;
			strFormat.Format( "[%s] killed [%s]", pUser->GetName(), pDeath->GetName() );
			Send( strFormat );
		//	Send();
		}
	}
}
//--------------------Database Operations----------------------
BOOL CArena::Load( void )
{
	CQuery* pQuery = new CQuery;
	if( pQuery->Connect( 3, "Character01", "", "" ) == FALSE )
	{
		pQuery->DisConnect();
		SAFE_DELETE( pQuery );
		return FALSE;
	}

	if( pQuery->Exec( "SELECT * FROM dbo.ARENA" ) == FALSE )
	{
		pQuery->DisConnect();
		SAFE_DELETE( pQuery );
		return FALSE;
	}

	while( pQuery->Fetch() )
	{
		char szQryName[MAX_NAME] = { 0, };
		ARENAPLAYER dbPlayer;

		u_long idPlayer				= static_cast<u_long>( pQuery->GetInt64( "m_idPlayer" ) );

		dbPlayer.nKill				= static_cast<u_int>( pQuery->GetInt64( "m_nKill" ) );
		dbPlayer.nDeath				= static_cast<u_int>( pQuery->GetInt64( "m_nDeath" ) );
		dbPlayer.nRow				= static_cast<u_int>( pQuery->GetInt64( "m_nRow" ) );
		dbPlayer.nJob				= static_cast<u_int>( pQuery->GetInt64( "m_nJob" ) );
		dbPlayer.nDeathMatchWin		= static_cast<u_int>( pQuery->GetInt64( "m_nDeathMatchWin" ) );
		dbPlayer.nDeathMatchLose	= static_cast<u_int>( pQuery->GetInt64( "m_nDeathMatchLose" ) );
		dbPlayer.nDuelWin			= static_cast<u_int>( pQuery->GetInt64( "m_nDuelWin" ) );
		dbPlayer.nDuelLose			= static_cast<u_int>( pQuery->GetInt64( "m_nDuelLose" ) );
		dbPlayer.n64ArenaPoint		=					  pQuery->GetInt64( "m_nArenaPoint" );
						
					pQuery->GetStr( "m_szName", szQryName );
		dbPlayer.szName				=  CString( szQryName );

		m_mArenaMap.insert( make_pair( idPlayer, dbPlayer ) );
	}

	pQuery->DisConnect();
	SAFE_DELETE( pQuery );
	return TRUE;
}
BOOL CArena::AddToDatabase( u_long idPlayer, CString szName, u_int nJob )
{
	CQuery* pQuery = new CQuery;
	if( pQuery->Connect( 3, "Character01", "", "" ) == FALSE )
	{
		pQuery->DisConnect();
		SAFE_DELETE( pQuery );
		return FALSE;
	}

	CString szDatabaseAdd;
	szDatabaseAdd.Format( "INSERT INTO dbo.ARENA values(%u," //idPlayer
		"0,"//nKill
		"0,"//nDeath
		"0,"//nRow
		"%u,"//nJob
		"0,"//nDeathmatchWin
		"0,"//nDeathMatchLose
		"0,"//nDuelWin
		"0,"//nDuelLose
		"0,"//n64ArenaPoint
		"'%s')", /*szName*/
		idPlayer, nJob, szName );
	

	//Error( szDatabaseAdd );

		/*

			ar << it->second.nKill;
		ar << it->second.nDeath;
		ar << it->second.nRow;
		ar << it->second.nJob;
		ar << it->second.nDeathMatchWin;
		ar << it->second.nDeathMatchLose;
		ar << it->second.nDuelWin;
		ar << it->second.nDuelLose;
		ar << it->second.n64ArenaPoint;

		*/

	if( pQuery->Exec( szDatabaseAdd ) == FALSE )
	{
		Error( "------- Start of CArena::AddToDatabase -------" );
		Error( "Error Inserting User. Query ---> %s", szDatabaseAdd );
		Error( "--------- End of CArena::AddToDatabase -------" );
		pQuery->DisConnect();
		SAFE_DELETE( pQuery );
		return FALSE;
	}

	pQuery->DisConnect();
	SAFE_DELETE( pQuery );
	return TRUE;
}


void CArena::AddOnline( CUser* pUser )
{
	if( IsValidObj( pUser ) )
	{
		if( m_Online.find( pUser->m_idPlayer ) != m_Online.end() )
			return;

		//if( m_mArenaMap.size() != 0 )
		{
			if( m_mArenaMap.find( pUser->m_idPlayer ) == m_mArenaMap.end() )
			{
				this->Add( pUser );
			}
		}

		m_Online.insert( make_pair( pUser->m_idPlayer, 0 ) );

		CString strFormat;
		strFormat.Format( "%s has entered the Arena", pUser->GetName() );
		Send( strFormat );

		//Send();
		//pUser->AddTextD3D( "You are entering the arena", 0xFFFFFFFF );
	}
}

void CArena::RemoveOnline( CUser* pUser )
{
	if( IsValidObj( pUser ) )
	{
		map<u_long, nomember>::iterator it = m_Online.find( pUser->m_idPlayer );
		if( it != m_Online.end() )
		{
			m_Online.erase( it );
			CString strFormat;
			strFormat.Format( "%s has left the Arena", pUser->GetName() );
			Send( strFormat );
		}
		//Send();
	}
}
			
void CArena::Send( void )
{
	for( map<u_long, nomember>::iterator it = m_Online.begin(); it != m_Online.end(); ++it )
	{
		CUser* pUser =			static_cast<CUser*>( prj.GetUserByID( it->first ) );
		if( IsValidObj( pUser ) )
		{
			pUser->SendArena();
		}
	}
}

void CArena::Send( CString strMessage )
{
	for( map<u_long, nomember>::iterator it = m_Online.begin(); it != m_Online.end(); ++it )
	{
		CUser* pUser =			static_cast<CUser*>( prj.GetUserByID( it->first ) );
		if( IsValidObj( pUser ) )
		{
			pUser->SendArena();
			pUser->AddText( strMessage );
//			pUser->AddTextD3D( strMessage, 0xFF3BBFBF );
		}
	}
}

u_long CArena::GetPointMultiplierFactor( void )
{
	return( xRandom( Arena::Point::nMultiplierFirst, Arena::Point::nMultiplierSecond ) );
}
#endif
