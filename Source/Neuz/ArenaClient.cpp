#include "StdAfx.h"
#ifdef __ARENA_PARADISE
#include "ArenaClient.h"

void CArenaClient::PaintFullType( C2DRender* p2DRender )
{
	if( NULL == p2DRender ) //idwacinpos
		return;

	int x = g_Option.m_nResWidth;
	int y = g_Option.m_nResHeight;

	signed int middleWidth = x / 2;
	signed int middleHeight = y / 2;

	signed int top		= ( ( y / 2 ) / 2 ) - 30;
	signed int bottom	= ( y / 2 ) + ( ( y / 2 ) / 2 ) + 30;
	signed int left		= ( ( x / 2 ) / 2 ) - ( x / 2 ) / 4;
	signed int right	=  x - left; //=x + ( ( x / 2 ) / 4 );// / 4;

	signed int write = top + 15;

	


	CString strArena = "Arena Scoreboard";
	CD3DFont* m_pFont = p2DRender->m_pFont;
	if( m_pFont == NULL )
		return;

	p2DRender->TextOut( middleWidth-( m_pFont->GetTextExtent( strArena ).cx / 2 ), top+12, "Arena Scoreboard", 0xFFFBB917, 0xFF000000 );

	//15 85 102 160 195 270 355 404 445 486

	int nTotal = 486;//15 + 85 + 102 + 160 + 195 + 270 + 355 + 404 + 445 + 486;
	int nTotalMiddle = nTotal / 2;
	int nTotalStart = middleWidth - nTotalMiddle;

	CRect rectFill;
	rectFill.top = top;
	rectFill.left = middleWidth - nTotalMiddle - 35;
	rectFill.right = middleWidth + nTotalMiddle + 39;
	rectFill.bottom = bottom;

	p2DRender->RenderFillRect( rectFill, D3DCOLOR_ARGB( 82, 0, 0, 0 ) );

	//215
	//left = 200
	// left(200)-(400)

	//left + 15

	p2DRender->TextOut( nTotalStart, top+32, "Name", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+85, top+32, "Kill", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+110, top+32, "Death", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+160, top+32, "Row", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+195, top+32, "Job", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+270, top+32, "Arena Point", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+355, top+32, "Duel/W", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+404, top+32, "Duel/L", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+445, top+32, "DM/W", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( nTotalStart+486, top+32, "DM/L", 0xFFFFFFFF, 0xFF000000 );
	



	int i = 0;
	int liney = top + 50;
	vector<ARENAPLAYER>::iterator end = m_Arena.size() >= 15 ? m_Arena.begin()+15 : m_Arena.end();
	for( vector<ARENAPLAYER>::iterator it = m_Arena.begin(); it != end; ++it )
	{
		D3DCOLOR md3Color = 0xFF157DEC;
		if( i == 0 ) md3Color = 0xFFFBB917;
		else if( i == 1 ) md3Color = 0xFFC0C0C0;
		else if( i == 2 ) md3Color = 0xFF7E3817;
		
		CString strRender[10];
		strRender[0] = CString( (*it).szName, 6 );
		strRender[0] += "...";
		strRender[1].Format( "%u", (*it).nKill );
		strRender[2].Format( "%u", (*it).nDeath );
		strRender[3].Format( "%u", (*it).nRow );
		strRender[4].Format( "%s", prj.m_aJob[(*it).nJob].szName );
		strRender[5].Format( "%I64u", (*it).n64ArenaPoint );
		strRender[6].Format( "%u", (*it).nDuelWin );
		strRender[7].Format( "%u", (*it).nDuelLose );
		strRender[8].Format( "%u", (*it).nDeathMatchWin );
		strRender[9].Format( "%u", (*it).nDeathMatchLose );


		p2DRender->TextOut( nTotalStart, liney, strRender[0], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+85, liney, strRender[1], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+120, liney, strRender[2], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+160, liney, strRender[3], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+195, liney, strRender[4], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+270, liney, strRender[5], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+355, liney, strRender[6], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+404, liney, strRender[7], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+445, liney, strRender[8], md3Color, 0xFF000000 );
		p2DRender->TextOut( nTotalStart+486, liney, strRender[9], md3Color, 0xFF000000 );

		i++;
		liney += 16;
	}

}


void CArenaClient::Paint( C2DRender* p2DRender )
{
	if( NULL == p2DRender )
		return;

//#define __DEBUG_TYPE
#ifdef __DEBUG_TYPE
	PaintFullType( p2DRender );
	return;
#endif

	if( GetAsyncKeyState( VK_TAB ) & 0x8000 )
	{
		if( m_dwArenaTick < GetTickCount() )
		{
			ToggleType();
			m_dwArenaTick = GetTickCount() + 1000;
		}
	}

	if( IsFullType() )
	{
		PaintFullType( p2DRender );
		return;
	}

	int x = g_Option.m_nResWidth;
	int y = g_Option.m_nResHeight;

	int left = x - 200;
	int top = ( ( y / 2 ) / 2 );

	CRect rectFill;
	rectFill.right = x;
	rectFill.left = x - 200;
	rectFill.top = ( ( y / 2 ) / 2 );
	rectFill.bottom = ( ( y / 2 ) ) + ( ( y / 2 ) / 2 );

	p2DRender->RenderFillRect( rectFill, D3DCOLOR_ARGB( 40, 0, 0, 0 ) );

	p2DRender->TextOut( left+15, top+4, "Name", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( left+75, top+4, "Kill", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( left+108, top+4, "Death", 0xFFFFFFFF, 0xFF000000 );
	p2DRender->TextOut( left+160, top+4, "Row", 0xFFFFFFFF, 0xFF000000 );

	
	int i = 0;
	int liney = top + 36;
	vector<ARENAPLAYER>::iterator end = m_Arena.size() >= 15 ? m_Arena.begin()+15 : m_Arena.end();
	for( vector<ARENAPLAYER>::iterator it = m_Arena.begin(); it != end; ++it )
	{
		D3DCOLOR md3Color = 0xFF157DEC;
		if( i == 0 ) md3Color = 0xFFFBB917;
		else if( i == 1 ) md3Color = 0xFFC0C0C0;
		else if( i == 2 ) md3Color = 0xFF7E3817;
		
		CString strRender[4];
		strRender[0] = CString( (*it).szName, 6 );
		strRender[0] += "...";
		strRender[1].Format( "%u", (*it).nKill );
		strRender[2].Format( "%u", (*it).nDeath );
		strRender[3].Format( "%u", (*it).nRow );

		p2DRender->TextOut( left+15, liney, strRender[0], md3Color, 0xFF000000 );
		p2DRender->TextOut( left+75, liney, strRender[1], md3Color, 0xFF000000 );
		p2DRender->TextOut( left+108, liney, strRender[2], md3Color, 0xFF000000 );
		p2DRender->TextOut( left+160, liney, strRender[3], md3Color, 0xFF000000 );

		i++;
		liney += 16;
	}

}

void CArenaClient::SetScore( vector<ARENAPLAYER>& vArena )
{
	vector<u_long>vOrdered;
	vector<CString>vNames;

	for( vector<ARENAPLAYER>::iterator start = vArena.begin(); start != vArena.end(); ++start )
	{
		vOrdered.push_back( (*start).nKill );
	}

	std::sort( vOrdered.begin(), vOrdered.end() );

	m_Arena.clear();
	for( vector<u_long>::iterator itor = vOrdered.end()-1; itor != vOrdered.begin()-1; --itor )
	{
		for( vector<ARENAPLAYER>::iterator iter = vArena.begin(); iter != vArena.end(); ++iter )
		{
			if( (*iter).nKill == (*itor) )
			{
				if( std::find( vNames.begin(), vNames.end(), (*iter).szName ) == vNames.end() )
				{
					vNames.push_back( (*iter).szName );
					m_Arena.push_back( (*iter) );
				}
			}
		}
	}
}

#endif