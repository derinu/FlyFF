#include "StdAfx.h"
#ifdef __DDOM
#include "User.h"
#include "Mover.h"
#include "World.h"
#include "DDom.h"
extern CUserMng g_UserMng;

#include "DPCoreClient.h"
extern	CDPCoreClient		g_DPCoreClient;

#include "dpdatabaseclient.h"
extern	CDPDatabaseClient	g_dpDBClient;


inline unsigned __int64 NanoTime();
static unsigned __int64 g_Prev = NanoTime();
__int64 xxRandom( void );

CDDomTeam::CDDomTeam()
{
	Init();
}
void CDDomTeam::Init( void )
{
	nKill = nDeath = nTouch = nCaptureS = 0;
	m_TeamPlayer.clear();
}
CDDomTeam::~CDDomTeam()
{
}

CDDom::CDDom()
{
	m_dwRound = 0;
	m_dwRoundTime = 0;
	m_idCapUser = 0;
	m_splitSize = 0;
	m_wTime = 0 ;
	m_RunTime = RUN_CLOSED;
}

BOOL CDDomTeam::Add( CUser* pUser, DDOM_TEAM team )
{
	if( IsValidObj( pUser ) == FALSE )
		return FALSE;

	vector<DOMPLAYER>::iterator it = find( m_TeamPlayer.begin(), m_TeamPlayer.end(), pUser->m_idPlayer );
	if( it == m_TeamPlayer.end() )
	{
		DOMPLAYER dom;
		dom.nJob = pUser->GetJob();
		dom.idObj = pUser->m_idPlayer;
		dom.strName = pUser->GetName();
		dom.nTeam = static_cast<int>( team );
		m_TeamPlayer.push_back( dom );

			return TRUE;
	}
	return FALSE;
}

BOOL CDDomTeam::Remove( CUser* pUser )
{
	if( IsValidObj( pUser ) == FALSE )
		return FALSE;

	pUser->m_ddomTeam = MAX_TEAM;
	pUser->SendAdditionalMover( pUser );
	vector<DOMPLAYER>::iterator it = find( m_TeamPlayer.begin(), m_TeamPlayer.end(), pUser->m_idPlayer );
	if( it != m_TeamPlayer.end() )
	{
		m_TeamPlayer.erase( it );
		return TRUE;
	}
	return FALSE;
}

BOOL IsValidBase( DDOM_BASE base )
{
	switch( base )
	{
		case BASE_A:
		case BASE_B:
		{
			return TRUE;
		}break;
	}
	return FALSE;
}

BOOL IsValidTeamOne( DDOM_TEAM team )
{
	switch( team )
	{
		case TEAM_A:
		case TEAM_B:
		{
			return TRUE;
		}break;
	}
	return FALSE;
}

BOOL IsValidTeam( DDOM_TEAM t1, DDOM_TEAM t2 )
{
	BOOL bRet = FALSE;
	switch( t1 )
	{
		case TEAM_A:
		case TEAM_B:
		{
			bRet = TRUE;
		}break;
	}
	switch( t2 )
	{
		case TEAM_A:
		case TEAM_B:
		{
			bRet = TRUE;
		}break;
		default:
		{
			bRet = FALSE;
		}
	}
	return bRet;
}

void CDDomTeam::Increase( u_long idPlayer, DDOM_INC inc )
{
	if( idPlayer == 0 )
		return;
	
	vector<DOMPLAYER>::iterator it = find( m_TeamPlayer.begin(), m_TeamPlayer.end(), idPlayer );
	if( it !=  m_TeamPlayer.end() )
	{
		switch( inc )
		{
			case INC_KILL:
				{
					it->nPoint += DoubleDom::Point::nKillPoint;
					it->nKill++; 
					this->nKill++;
				}break;
			case INC_DEATH:
				{
					it->nDeath++; 
					this->nDeath++;
				}break;
			case INC_CAPTURE:  
				{
					it->nPoint += DoubleDom::Point::nCapturePoint;
					it->nCaptureS++;
					this->nCaptureS++;
				} break;	
			case INC_TOUCH:
				{
					it->nPoint += DoubleDom::Point::nTouchPoint;
					it->nTouch++;
					this->nTouch++;
				}break;
		}
		CDDom::GetInstance().SendScore(); 
	}
}


void CDDom::SendTouch( DDOM_TEAM team, DDOM_BASE base, BOOL bCaptured )
{
	if( GetRunTime() == RUN_CLOSED )
		return;

	for( vector<u_long>::iterator it = m_User.begin(); it != m_User.end(); ++it )
	{
		CUser*	pUser		= static_cast<CUser*>( prj.GetUserByID( (*it ) ) );
		if( IsValidObj( pUser ) )
		{
			pUser->SendDDomTouch( team, base, bCaptured );
		}
	}
}

void CDDomTeam::Serialize( CAr & ar )
{
	ar << m_TeamPlayer.size();
	for( vector<DOMPLAYER>::iterator it = m_TeamPlayer.begin(); it != m_TeamPlayer.end(); ++it )
	{
		ar << it->nKill;
		ar << it->nDeath;
		ar << it->nTouch;
		ar << it->nCaptureS;
		ar << it->nTeam;
		ar << it->nJob;
		ar << it->nPoint;
		ar.WriteString( LPCTSTR( it->strName ) );
	}
	
	ar << this->nKill;
	ar << this->nDeath;
	ar << this->nTouch;
	ar << this->nCaptureS;
}


void CDDom::Serialize( CAr & ar )
{
	m_DomTeam[TEAM_A].Serialize( ar );
	m_DomTeam[TEAM_B].Serialize( ar );
}

void CDDom::SendScore( void )
{
	if( GetRunTime() == RUN_CLOSED )
		return;

	for( vector<u_long>::iterator it = m_User.begin(); it != m_User.end(); ++it )
	{
		CUser*	pUser		= static_cast<CUser*>( prj.GetUserByID( (*it ) ) );
		if( IsValidObj( pUser ) )
		{
			pUser->SendDDomScore();
		}
	}
}

void CDDom::Send( CString strMessage )
{
	if( GetRunTime() == RUN_CLOSED )
		return;

	for( vector<u_long>::iterator it = m_User.begin(); it != m_User.end(); ++it )
	{
		CUser*	pUser		= static_cast<CUser*>( prj.GetUserByID( (*it ) ) );
		if( IsValidObj( pUser ) )
		{
			pUser->AddTextD3D( strMessage, DoubleDom::Color::nBlueCol );
		}
	}
}

void CDDom::Send( CString strMessage, FONT_MSG font, D3DCOLOR d3dcolor )
{
	if( GetRunTime() == RUN_CLOSED )
		return;
	for( vector<u_long>::iterator it = m_User.begin(); it != m_User.end(); ++it )
	{
		CUser*	pUser		= static_cast<CUser*>( prj.GetUserByID( (*it ) ) );
		if( IsValidObj( pUser ) )
		{
#ifdef __NEW_FONT
			//pUser->SendTextFont( strMessage, static_cast<u_int>( font ), d3dcolor );
			pUser->AddTextD3D( strMessage, d3dcolor );//
#else
			pUser->AddTextD3D( strMessage, d3dcolor );
#endif
		}
	}
}



BOOL CDDom::Start( vector<u_long>& playerList  )
{
	if( GetRunTime() != RUN_CLOSED )
		return FALSE;
	SetRunTime( RUN_FIGHT );

	g_DPCoreClient.SendSystem("Double Domination has started!");

#ifdef __NEW_FONT
	Send( "Double Domination Start", FONT_MIDMED, DoubleDom::Color::nBlueCol );
	//g_UserMng.AddTextFontType(  );
#endif

	m_splitSize = playerList.size() / 2;
	for( vector<u_long>::iterator it = playerList.begin(); it != playerList.end(); ++it )
	{
		CUser*	pUser	= static_cast<CUser*>( prj.GetUserByID( (*it ) ) );
		if( IsValidObj( pUser ) )
		{
			Join( pUser );
		}
	}
	m_splitSize = 0;
	return TRUE;
}

void CDDom::Touch( CUser* pUser, DDOM_BASE base )
{
	if( IsValidObj( pUser ) == FALSE )
		return;
	if( IsValidBase( base ) == FALSE )
		return;
	if( GetRunTime() != RUN_FIGHT )
		return; 
	if( pUser->IsDie() )
		return; 

	const DDOM_TEAM teamUser = pUser->m_ddomTeam;
	if( IsValidTeamOne( teamUser ) )
	{
		DDOM_TEAM team = m_Base[base].GetTouchTeam();
		if( team != MAX_TEAM )
		{
			if( team == pUser->m_ddomTeam )
			{
				pUser->AddTextD3D( "This base is already captured by your team.", 0xFFFF0000 );
				return;
			}
		}

		CString strTouch;
		strTouch.Format( "[%s] %s captured %s base", GetName( teamUser ), pUser->GetName(), GetBaseName( base ) );
		//SendBaseTouch( strTouch, base, pUser->m_ddomTeam ); //ATTENTION
		Send( strTouch );

		m_Base[base].SetTouchedBy( teamUser );
		const DDOM_BASE opBase = GetOponentBase( base );

		
		m_idCapUser = pUser->m_idPlayer;
		BOOL bTouchTime = FALSE;
		DDOM_TEAM opTeam = m_Base[opBase].GetTouchTeam(); 
		if( opTeam == pUser->m_ddomTeam )
		{
			bTouchTime = TRUE;
			CString strFontText; 
			strFontText.Format( "Both bases have been dominated by %s team. Countdown start", GetName( teamUser ) );
			Send( strFontText, FONT_MIDMED, DoubleDom::Color::nBlueCol );
			SetTime();
		}
		else
		{
			if( GetTime() == 1 || GetTime() == 2 )
			{
				ResetTime();
				Send( "Last second save", FONT_MIDMED, DoubleDom::Color::nBlueCol );
			}
		}
		m_DomTeam[ teamUser ].Increase( pUser->m_idPlayer, INC_TOUCH );
		SendTouch( pUser->m_ddomTeam, base, bTouchTime ); 
	}
}

void CDDom::ReturnAll( void )
{
	//CItemElem item;
	//item.m_dwItemId = II_CHR_FOO_COO_REMANTIS;
	//item.m_nItemNum = 5;

	for( vector<u_long>::iterator it = m_User.begin(); it != m_User.end(); ++it )
	{
		CUser* pUser		= static_cast<CUser*>( prj.GetUserByID( (*it) ) );
		if( IsValidObj( pUser ) )
		{
			pUser->m_ddomTeam = MAX_TEAM;
			pUser->SendAdditionalMover( pUser );
			pUser->AddTextD3D( "Thank you for participating", DoubleDom::Color::nBlueCol );
			pUser->Replace( g_uIdofMulti, WI_WORLD_MADRIGAL, D3DXVECTOR3( 6971.0F, 100.0F, 3327.0F ), REPLACE_FORCE, nDefaultLayer );
			//item.SetSerialNumber();
			//g_dpDBClient.SendQueryPostMail( pUser->m_idPlayer, 0, item, 10000, "Double Domination", "Thank you for participating" );
		}
	}
}			
			
void CDDomTeam::SendPrize( void )
{
	for( vector<DOMPLAYER>::iterator it = m_TeamPlayer.begin(); it != m_TeamPlayer.end(); ++it )
	{
		CUser*	pUser		= static_cast<CUser*>( prj.GetUserByID( it->idObj ) );
		if(	IsValidObj( pUser ) )
		{
			pUser->AddTextD3D( "You have won the Double Domination match!", DoubleDom::Color::nBlueCol );
			//item.SetSerialNumber();
	//		if( pUser->CreateItem( item ) == FALSE )
			//{
			//	g_dpDBClient.SendQueryPostMail( pUser->m_idPlayer, 0, item, 10000, "Double Domination", "You have won the match." );
			//}
		}
		else if( it->idObj != 0 )
		{
			//item.SetSerialNumber();
			//g_dpDBClient.SendQueryPostMail( pUser->m_idPlayer, 0, item, 10000, "Double Domination", "You have won the match." );
		}
	}
}


void CDDom::End( void )
{
	if( GetRunTime() == RUN_CLOSED )
		return;

	SendTouch( MAX_TEAM, BASE_A );
	SendTouch( MAX_TEAM, BASE_B ); 

	CDDomTeam& team = m_DomTeam[TEAM_A].GetCaptures() > m_DomTeam[TEAM_B].GetCaptures() ? m_DomTeam[TEAM_A] : m_DomTeam[TEAM_B];
	
	if( m_DomTeam[TEAM_A].GetAmount() == 0 )
		team = m_DomTeam[TEAM_B];
	else if( m_DomTeam[TEAM_B].GetAmount() == 0 )
		team = m_DomTeam[TEAM_A];

	DDOM_TEAM dteam = m_DomTeam[TEAM_A].GetCaptures() > m_DomTeam[TEAM_B].GetCaptures() ? TEAM_A : TEAM_B; 

	team.SendPrize();
	CString strTeam;
	strTeam.Format( "Team %s has won the Double Domination game! You can now join at Is!", GetName( dteam ) );
	//this->Send( strTeam );
	//this->Send( "The game has ended!" );
	//g_DPCoreClient.SendSystem( strTeam );
	g_DPCoreClient.SendSystem( strTeam );
	
	ReturnAll();
	
	m_dwRound = 0;
	m_dwRoundTime = 0;
	m_idCapUser = 0;
	m_splitSize = 0;
	m_wTime = 0 ;
	m_RunTime = RUN_CLOSED;
	m_User.clear();

	m_Base[BASE_A].SetTouchedBy( MAX_TEAM );
	m_Base[BASE_B].SetTouchedBy( MAX_TEAM );

	m_DomTeam[TEAM_A].Init();
	m_DomTeam[TEAM_B].Init();
}



void CDDom::Capped( DDOM_TEAM team )
{
	if( GetRunTime() == RUN_CLOSED )
		return;

	m_DomTeam[team].Increase( m_idCapUser, INC_CAPTURE );
	m_idCapUser = 0;

	if( m_DomTeam[team].GetCaptures() >= DoubleDom::Queue::nRounds )
	{
		End();
		return;
	} 

/*#ifdef __LEA_MONDE
	g_UserMng.AddDominationWireframe();
#endif*/

	CString strCaptured;
	strCaptured.Format( "Team %s has increased the score, new round in 9 seconds", GetName( team ) );
	Send( strCaptured );
	SetRunTime( RUN_COOLDOWN );
	m_dwRoundTime = GetTickCount() + 9000;
	m_dwRound++;
}

void CDDom::StartRound( void )
{
	if( GetRunTime() != RUN_COOLDOWN )
		return;

	CString strRound;
	strRound.Format( "The round: %u has begun", m_dwRound +1 );
	for( vector<u_long>::iterator it = m_User.begin(); it != m_User.end(); ++it )
	{
		CUser*	pUser	= static_cast<CUser*>( prj.GetUserByID( (*it ) ) );
		if( IsValidObj( pUser ) )
		{
			pUser->AddTextD3D( strRound, DoubleDom::Color::nBlueCol );
			Safe( pUser );
		} 
	}

	SendTouch( MAX_TEAM, BASE_A );
	SendTouch( MAX_TEAM, BASE_B ); 
	SetRunTime( RUN_FIGHT );
}

void CDDom::Process( void )
{
	if( GetRunTime() == RUN_CLOSED )
		return;

	if( GetRunTime() == RUN_COOLDOWN )
	{
		if( m_dwRoundTime != 0 && m_dwRoundTime < GetTickCount() )
		{
			m_dwRoundTime = 0;
			StartRound();
		}
	}
	else if( GetRunTime() == RUN_FIGHT )
	{
		if( GetTime() > 0 )
		{
			DecreaseTime();
		}
		else
		{
			const DDOM_TEAM tA = m_Base[BASE_A].GetTouchTeam();
			const DDOM_TEAM tB = m_Base[BASE_B].GetTouchTeam();
			if( tA != MAX_TEAM && tB != MAX_TEAM )
			{
				if( tA == tB )
				{
					Capped( tA ); //ATTENTION
					ResetCapTeam(); 
				} 
			}	
		}
	}
}

void CDDom::ResetCapTeam( void )
{
	ResetTime();
	m_Base[TEAM_A].SetTouchedBy( MAX_TEAM );
	m_Base[TEAM_B].SetTouchedBy( MAX_TEAM );
}

void CDDom::Kill( CUser* pAttack, CUser* pTarget )
{
	if( GetRunTime() != RUN_FIGHT )
		return;
	if( IsInvalidObj( pAttack ) || IsInvalidObj( pTarget ) )
		return;
	if( IsValidTeam( pAttack->m_ddomTeam, pTarget->m_ddomTeam ) )
	{
		CDDomTeam* pDomAttack = &m_DomTeam[ pAttack->m_ddomTeam ];
		CDDomTeam* pDomDefend = &m_DomTeam[ pTarget->m_ddomTeam ];
		if( pDomAttack == NULL || pDomDefend == NULL )
			return;

		//Increase( pAttack->m_ddomTeam, INC_KILL );
		//Increase( pTarget->m_ddomTeam, INC_DEATH );

		pDomAttack->Increase( pAttack->m_idPlayer, INC_KILL );
		pDomDefend->Increase( pTarget->m_idPlayer, INC_DEATH );

		CString strKill;
		strKill.Format( "[%s] %s has killed [%s] %s", GetName( pAttack->m_ddomTeam ), pAttack->GetName(), 
			GetName( pTarget->m_ddomTeam ), pTarget->GetName() );
		Send( strKill, FONT_MIDMED, DoubleDom::Color::nGoldCol );
	}
}

BOOL CDDom::Join( CUser* pUser )
{
	if( GetRunTime() != RUN_FIGHT )
	{
		if( IsValidObj( pUser ) )
		{
			pUser->AddTextD3D( "No match available", 0xFFFF0000 );
		}
		return FALSE;
	}

	__int64 domRandom = xxRandom();
	DDOM_TEAM domTeam =  static_cast<DDOM_TEAM>( domRandom & 0x01 );
	if( IsValidTeamOne( domTeam ) == FALSE )
		return FALSE;
	if( m_DomTeam[domTeam].GetAmount() >= m_splitSize )
		domTeam = GetOponentTeam( domTeam );
	CString strTeam = GetName( domTeam );
	if( strTeam.IsEmpty() == true )
		return FALSE;
	if( m_DomTeam[domTeam].Add( pUser, domTeam ) != TRUE )
		return FALSE;

	m_User.push_back( pUser->m_idPlayer );

	pUser->m_ddomTeam = domTeam;
	Safe( pUser );
	pUser->SendAdditionalMover( static_cast<CMover*>( pUser ) ); //hey!

	CString strJoin;
	strJoin.Format( "You have been set to %s team", strTeam );
	pUser->AddTextD3D( "You have joined the double domination", DoubleDom::Color::nBlueCol );
	pUser->AddTextD3D( strJoin, DoubleDom::Color::nBlueCol );
	
	SendScore();

	return TRUE;
}

BOOL CDDom::Kick( CUser* pUser )
{
	if( IsValidObj( pUser ) == FALSE )
		return FALSE;

	vector<u_long>::iterator it = find( m_User.begin(), m_User.end(), pUser->m_idPlayer );
	if( it != m_User.end() )
	{
		m_User.erase( it );
	}

	if( m_DomTeam[TEAM_A].GetAmount() == 0 && m_DomTeam[TEAM_B].GetAmount() == 0 )
		return FALSE;
	if( m_DomTeam[TEAM_A].Remove( pUser ) == FALSE )
	{
		if( m_DomTeam[TEAM_B].Remove( pUser ) == FALSE )
		{
			return FALSE;
		}
	} 

	if( GetRunTime() != RUN_CLOSED )
	{
		if( m_DomTeam[TEAM_A].GetAmount() == 0 || m_DomTeam[TEAM_B].GetAmount() == 0 )
			End();
	} 
	
	return TRUE;
}

CString CDDom::GetBaseName( DDOM_BASE base )
{
	switch( base )
	{
		case BASE_A: return _T( "A" ); break;
		case BASE_B: return _T( "B" ); break;
	}
	return _T("");
}

DDOM_TEAM CDDom::GetOponentTeam( DDOM_TEAM team )
{
	switch( team )
	{
		case TEAM_A: return TEAM_B; break;
		case TEAM_B: return TEAM_A; break;
	}
	return TEAM_A;
}

DDOM_BASE CDDom::GetOponentBase( DDOM_BASE base )
{
	switch( base )
	{
		case BASE_A: return BASE_B; break;
		case BASE_B: return BASE_A; break;
	}
	return BASE_A;
}

CString CDDom::GetName( DDOM_TEAM team )
{
	switch( team )
	{
		case TEAM_A: return DoubleDom::Name::strTeamA; break;
		case TEAM_B: return DoubleDom::Name::strTeamB; break;
	}
	return _T("");
}

void CDDom::Safe( CUser* pUser )
{
	if( IsValidObj( pUser ) == FALSE )
		return;
	if( pUser->IsDie() ) 
		return;

	switch( pUser->m_ddomTeam )
	{
		case TEAM_A: pUser->Replace( g_uIdofMulti, WI_WORLD_DOMINATION, D3DXVECTOR3( 1457.259521f, 108.749542f, 1270.984619f ), REPLACE_FORCE, nDefaultLayer ); break;
		case TEAM_B: pUser->Replace( g_uIdofMulti, WI_WORLD_DOMINATION, D3DXVECTOR3( 1374.718506f, 108.618896f, 1002.483582f ), REPLACE_FORCE, nDefaultLayer ); break;
	default:
		{
#ifdef _DEBUG
			pUser->Replace( g_uIdofMulti, WI_WORLD_DOMINATION, D3DXVECTOR3( 1359.0F, 81.0F, 998.0F ), REPLACE_FORCE, nDefaultLayer );
#endif
			break;
		}
	}
}
inline unsigned __int64 NanoTime()
{
    struct { int low, high; } Nano;

    __asm push EAX
    __asm push EDX
    __asm __emit 0fh __asm __emit 031h
    __asm mov Nano.low, EAX
    __asm mov Nano.high, EDX
	__asm pop EDX
    __asm pop EAX

    return *(__int64 *)( &Nano );

}
__int64 xxRandom( void )
{
	__int64 Now = NanoTime() - g_Prev / 2;
	CString strRand, strLast;
	strRand.Format( "%I64d", Now );
	if( strLast.GetLength() < 2 )
		return 0;

	strLast = strRand.GetAt( strRand.GetLength()-1 );
	strLast += strRand.GetAt( strRand.GetLength()-2 );
	return( static_cast<__int64>( _ttol( strLast ) ) );
}
#endif