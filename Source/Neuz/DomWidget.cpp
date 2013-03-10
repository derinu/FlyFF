#include "StdAfx.h"
#ifdef __DDOM
#include "2DRender.h"
#include "DomWidget.h"

#define DOMCOL_A 0xEF7AA2D6
#define DOMCOL_B 0xEF74CF51 

//ATTENTION DO NUMBERS 8-7-6-5-4-3-2-1 using font saved on desktop

void CDDomWidget::SetData( vector<DOMQPL>& vecDomPlayer )
{
	m_vecDomPlayer = vecDomPlayer;
}

void CDDomWidget::SetFont( CD3DFont* pFont )
{
	m_pFont = pFont;
}

void CDDomWidget::PaintWidget( C2DRender* p2DRender )
{
	if( NULL == p2DRender )
		return;

	int x = g_Option.m_nResWidth;
	int y = g_Option.m_nResHeight;
	CRect rect;
	rect.left = x - 96;
	rect.right = x;
	rect.top =  y / 2 - 30;
	rect.bottom = y / 2 - 10;
	p2DRender->RenderFillRect( rect, D3DCOLOR_ARGB( 30, 0, 0, 0 ) );
	CString strList;
	strList.Format( "Queued: %u/12", m_vecDomPlayer.size() );
	p2DRender->TextOut( rect.left + 4, rect.top + 2, strList, 0xFFFFFFFF, 0xFF000000 );

/*	int x = g_Option.m_nResWidth;
	int y = g_Option.m_nResHeight;
 
	CRect rect2;
	rect2.top = 0;
	rect2.left = x / 2 - 413;
	rect2.right = x / 2 + 413;
	rect2.bottom = 575;

	CRect rect3;
	rect3.top = rect2.bottom;
	rect3.left = x / 2 - 50;
	rect3.right = x / 2 + 50;
	rect3.bottom = rect3.top + 20;
	p2DRender->RenderFillRect( rect3, D3DCOLOR_ARGB( 23, 0, 0, 0 ) ); 
	
	p2DRender->TextOut( rect3.left + 20, rect3.top + 4, "Close", 0xFCC4A233, 0xAA000000 );
	CPoint mPoint = g_WndMng.GetMousePoint();//PtInRect( point )
	if( PtInRect( &rect3, mPoint ) )
	{ 
		if( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 )
		{
			m_RenderMode = RENDER_SMALL; 
		}
	}

	
	if( m_RenderMode == RENDER_SMALL )
		return;

	p2DRender->RenderFillRect( rect2, D3DCOLOR_ARGB( 17, 0, 0, 0 ) );  */

}


void CDDomWidget::SetColorTransition( void )
{
	/*for( int i =0; i<5; i++ )
			m_d3dTransColor[i] = DOMCOL_A; */

	int nNoTransform = 5;
	if( m_dwTransition < GetTickCount() )
	{
		if( m_d3dTransColor[0] != 0xAF74CF53 ) //Q
		{
			m_d3dTransColor[0] = 0xAF74CF53;
			m_dwTransition = GetTickCount() + 95; 
		}
		else if( m_d3dTransColor[1] != 0xAF74CF53 )//u
		{
			m_d3dTransColor[1] = 0xAF74CF53;
			m_dwTransition = GetTickCount() + 95;
		}
		else if( m_d3dTransColor[2] != 0xAF74CF53 )//e
		{
			m_d3dTransColor[2] = 0xAF74CF53;
			m_dwTransition = GetTickCount() + 95;
		}
		else if( m_d3dTransColor[3] != 0xAF74CF53 )//u
		{
			m_d3dTransColor[3] = 0xAF74CF53;
			m_dwTransition = GetTickCount() + 95;
		}
		else if( m_d3dTransColor[4] != 0xAF74CF53 )//e
		{
			m_d3dTransColor[4] = 0xAF74CF53 ; 
			m_dwTransition = GetTickCount() + 95;
		} 
		else
		{
			for( int i =0; i<5; i++ )
			{
				if( i != nNoTransform )
					m_d3dTransColor[i] = 0xAF7AA2D6;// DOMCOL_A; 
			}
			
			m_dwTransition = GetTickCount() + 7000;
		}
	}
}

void CDDomWidget::PaintFont( C2DRender* p2DRender )
{
	if( NULL == p2DRender )
		return;

	CD3DFont* pOldFont = p2DRender->GetFont();

	if( m_RenderMode == RENDER_SMALL )
		return;

	//if( m_pFont )
	//	p2DRender->SetFont( m_pFont );

	
 
	int x = g_Option.m_nResWidth;
	int y = g_Option.m_nResHeight;  

//	p2DRender->RenderRoundRect( rect 

	/*p2DRender->SetFont( CWndBase::m_Theme.m_pFontDrawRibbons );
	p2DRender->TextOut( 150, 150, "O", d3dcolor, 0 );*/
	#ifdef __NEWFONT
	 p2DRender->SetFont( CWndBase::m_Theme.m_pFontDisko );  
#endif
	SetColorTransition();
	#ifdef __NEWFONT
	long int nLess = CWndBase::m_Theme.m_pFontDisko->GetTextExtent( "Queue.cx" ).cx / 2 - 32;
	p2DRender->TextOut( x / 2 - 
		( nLess ), 38,
		"Q", m_d3dTransColor[0], 0x7E000000 );  
	p2DRender->TextOut( x / 2 - 
		( nLess ) + 65, 38,
		"u", m_d3dTransColor[1], 0x7E000000 );
	 p2DRender->TextOut( x / 2 - 
		( nLess ) + 109, 38,
		"e", m_d3dTransColor[2], 0x7E000000 );
	p2DRender->TextOut( x / 2 - 
		( nLess ) + 150, 38,
		"u", m_d3dTransColor[3], 0x7E000000 );
	p2DRender->TextOut( x / 2 -   
		( nLess ) + 192   , 38,
		"e", m_d3dTransColor[4], 0x7E000000 ); 
#endif

	//;
#ifdef __NEWFONT
	p2DRender->SetFont( CWndBase::m_Theme.m_pFontEverson );

	int wY = 151;
	int wX = x / 2 - 413;

	p2DRender->TextOut( x/2-( CWndBase::m_Theme.m_pFontEverson->GetTextExtent( "Double Domination" ).cx / 2 )
		, 36, "Double Domination", 0xAF7AA2D6, 0x4A000000 ); 

	p2DRender->TextOut( wX+130, wY, "Name", 0xFCC4A233, 0xAA000000 );
	p2DRender->TextOut( wX+250, wY, "Job", 0xFCC4A233, 0xAA000000 ); 
	p2DRender->TextOut( wX+370, wY, "Level", 0xFCC4A233, 0xAA000000 );
	p2DRender->TextOut( wX+490, wY, "Rebirth", 0xFCC4A233, 0xAA000000 );
	p2DRender->TextOut( wX+610, wY, "World", 0xFCC4A233, 0xAA000000 );  

	int nTop = wY + 33; 
	for( vector<DOMQPL>::iterator it = m_vecDomPlayer.begin(); it != m_vecDomPlayer.end(); ++it )
	{
		CString strRender[5];
		strRender[0] = CString( (*it).szName, 6 );
		strRender[0] += "...";
		strRender[1].Format( "%s", prj.m_aJob[(*it).nJob].szName );
		strRender[2].Format( "%u", (*it).nLevel );
		strRender[3].Format( "%u", (*it).nRebirth ); 
	//	CWorld* pWorld = g_WorldMng.GetWorld( (*it).WorldId )->m_szWorldName;
		strRender[4].Format( "%s", "Madrigal" );
		int nLeft =wX+130; 
		for( int i =0; i<5; i++ )
		{
			p2DRender->TextOut( nLeft, nTop, strRender[i], 0xFCC4A233, 0xAA000000 );
			nLeft+=120;
		}
		nTop+=22;
	}  
#endif
	p2DRender->SetFont( pOldFont ); 
}
#endif



	
