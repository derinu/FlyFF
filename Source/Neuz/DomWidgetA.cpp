#include "StdAfx.h"
#ifdef __DDOM
#include "2DRender.h"
#include "DomWidgetA.h"



DOMINATING_TEAM DomTeamToDominatingTeam( DDOM_TEAM team )
{
	switch( team )
	{
		case TEAM_A: return DTEAM_A; break;
		case TEAM_B: return DTEAM_B; break;
	}
	return DTEAM_NONE;
}

DDOM_TEAM DominatingTeamToDomTeam( DOMINATING_TEAM team )
{
	switch( team )
	{
		case DTEAM_A: return TEAM_A; break;
		case DTEAM_B: return TEAM_B; break;
	}
	return MAX_TEAM;
}


CDDomWidgetA::CDDomWidgetA()
{
	m_dwTime = 0;
	m_ToBeDominated = DTEAM_NONE;
	m_BaseA = m_BaseB = DTEAM_NONE;
}

DWORD CDDomWidgetA::GetAlpha( void )
{
	DWORD dwTime = GetTimeNumber();
	if( dwTime <= 0 || dwTime > 8 )
	{
		dwTime = 0;
		return 0xFF;
	}

	return( 255 - ( ( m_dwTime - GetTickCount() ) / 32 ) );
}

void CDDomWidgetA::MakeColor( D3DCOLOR& d3dcolor, DOMINATING_TEAM team )
{
	if( m_ToBeDominated != DTEAM_NONE )
	{
		switch( team )
		{
		case DTEAM_A: d3dcolor = D3DCOLOR_ARGB( 0xFF, GetAlpha(), 0x00, 0x00 ); break;
		case DTEAM_B: d3dcolor = D3DCOLOR_ARGB( 0xFF, 0x00, 0x00, GetAlpha() ); break;
			default:
				{
					d3dcolor = 0xDAD5DBBA;  
				}break;
		}
	}
	else
	{
		switch( team )
		{
			case DTEAM_A: d3dcolor = 0xFFE01B1B; break;
			case DTEAM_B: d3dcolor = 0xFF3C47DE; break;
			default:
				{
					d3dcolor = 0xDAD5DBBA;  
				}break;
		}
	}
}

DOMINATING_TEAM CDDomWidgetA::GetDominatingTeam( DDOM_BASE base )
{
	switch( base )
	{
	case BASE_A: return m_BaseA; break;
	case BASE_B: return m_BaseB; break;
	default:
		{
			return DTEAM_NONE;
		}break;
	}
}

void CDDomWidgetA::SetDominatingTeam( DDOM_BASE base, DOMINATING_TEAM team )
{
	switch( base )
	{
		case BASE_A: m_BaseA = team; break;
		case BASE_B: m_BaseB = team; break;
	}
	m_dwTime = 0;
	m_ToBeDominated = DTEAM_NONE;
}

void CDDomWidgetA::SetCapTeam( DOMINATING_TEAM team )
{
	m_dwTime = GetTickCount() + 8159;
	m_ToBeDominated = team;
}

DWORD CDDomWidgetA::GetTimeNumber( void )
{
	return( ( m_dwTime - GetTickCount() ) / 1000 );
}

void CDDomWidgetA::MakeColor( D3DCOLOR& d3dcolor, DWORD nTime ) 
{
	switch( nTime )
	{
	case 8:
	case 7:
	case 6:
		{
			d3dcolor = 0xFF2BC91C; 
		}break;
	case 5:
	case 4:
	case 3:
		{
			d3dcolor = 0xFFC9C41C; 
		}break;
	default:
		{
			d3dcolor = 0xFFD61A1A;
		}
	}
}


void CDDomWidgetA::PaintMiddleNumber( C2DRender* p2DRender, int x, int y )
{
	if( NULL == p2DRender )
		return;
	if( m_ToBeDominated == DTEAM_NONE )
		return;
	
	DWORD dwTime = GetTimeNumber();
	if( dwTime <= 0 || dwTime > 8 )
	{
		dwTime = 0;
		m_ToBeDominated = DTEAM_NONE;
	}


	CString strTime;
	strTime.Format( "%u", dwTime );
	D3DCOLOR color;
	MakeColor( color, dwTime );
	p2DRender->TextOut( x-11 , y+4, strTime, color, 0xEA000000 );	  
}

	

void CDDomWidgetA::Paint( C2DRender* p2DRender )
{
	if( NULL == p2DRender )
		return;

	int x = g_Option.m_nResWidth;
	int y = g_Option.m_nResHeight;

	CRect rect;
	rect.top = 25;
	rect.left = x / 2 / 2;
	rect.right = x / 2 + rect.left;
	rect.bottom = rect.top + 110;

	// p2DRender->RenderFillRect( rect, D3DCOLOR_ARGB( 30, 0, 0, 0 ) );

	CD3DFont* pOldFont = p2DRender->GetFont();
#ifdef __NEWFONT
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontDisko ); 
#endif

	D3DCOLOR colorA, colorB;
	MakeColor( colorA, m_BaseA );
	MakeColor( colorB, m_BaseB );

	/*p2DRender->TextOut( rect.left+15, rect.top, "A", colorA, 0xDA000000 ); 
	p2DRender->TextOut( rect.right-55, rect.top, "B", colorB, 0xDA000000 ); */ 
	
	PaintMiddleNumber( p2DRender, x / 2 - 11, rect.top-5 );

	p2DRender->TextOut( rect.left+15, rect.top, "A", colorA, 0xDA000000 ); 
	p2DRender->TextOut( rect.right-41 , rect.top, "B", colorB, 0xDA000000 ); 

	p2DRender->SetFont( pOldFont );



}


#endif