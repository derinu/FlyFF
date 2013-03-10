#include "StdAfx.h"
#ifdef __DDOM
#include "2DRender.h"
#include "DomScore.h"

CDDomScore::CDDomScore()
{
}
//The one million dollar function! :)

void CDDomScore::SetData( vector<DOMPLAYER>& dataPlayer,  vector<DOMTEAM>& dataTeam )
{
	m_DomTeam = dataTeam;
	vector<u_long>vOrdered;
	vector<CString>vNames;

	for( vector<DOMPLAYER>::iterator start = dataPlayer.begin(); start != dataPlayer.end(); ++start )
	{
		vOrdered.push_back( (*start).nKill );
	}

	std::sort( vOrdered.begin(), vOrdered.end() );

	m_DomPlayer.clear();
	for( vector<u_long>::iterator itor = vOrdered.end()-1; itor != vOrdered.begin()-1; --itor )
	{
		for( vector<DOMPLAYER>::iterator iter = dataPlayer.begin(); iter != dataPlayer.end(); ++iter )
		{
			if( (*iter).nKill == (*itor) )
			{
				if( std::find( vNames.begin(), vNames.end(), (*iter).strName ) == vNames.end() )
				{
					vNames.push_back( (*iter).strName );
					m_DomPlayer.push_back( (*iter) );
				}
			}
		}
	}
}

void CDDomScore::Render( C2DRender* p2DRender, const int& xPos, const int& yPos )
{
	if( NULL == p2DRender )
		return;
	if( NULL == p2DRender->m_pFont )
		return;

	int x = g_Option.m_nResWidth;
	int y = g_Option.m_nResHeight;

 	int nBlueX = xPos;
	int nBlueY = yPos + 52;
 
	int nRedX = xPos + 407 + 3;
	int nRedY = yPos + 52;

	int nLetterFontY = p2DRender->m_pFont->GetTextExtent( "letter" ).cy + 2;

	int nTeamY = yPos + 7; //serialize JOB -> TEAM SCORE -> make points
	/*
	team.nKill = 430;
	team.nDeath = 109;
	team.nTouch = 44;
	team.nCaptureS = 120;
	*/
	
	//img size = 815x435 // imgx/2 = 407,5

	const int nEnhance[7] = { 12, 53, 108, 182, 234, 288, 348 };
	const int nEnhanceName[7] = { 18, 53, 119, 182, 239, 288, 348 };
	const int nEnhanceTeam[5] = { 12, 102, 172, 252, 332 };

	p2DRender->TextOut( nBlueX+nEnhanceTeam[0], nTeamY, "Score", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nBlueX+nEnhanceTeam[1], nTeamY, "Kill", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nBlueX+nEnhanceTeam[2], nTeamY, "Death", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nBlueX+nEnhanceTeam[3], nTeamY, "Touch", 0xEA8DABC4, 0xED000000 );
	//p2DRender->TextOut( nBlueX+nEnhanceTeam[4], nTeamY, "Capture", 0xEA8DABC4, 0xED000000 );

	p2DRender->TextOut( nRedX+nEnhanceTeam[0], nTeamY, "Score", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nRedX+nEnhanceTeam[1], nTeamY, "Kill", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nRedX+nEnhanceTeam[2], nTeamY, "Death", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nRedX+nEnhanceTeam[3], nTeamY, "Touch", 0xEA8DABC4, 0xED000000 );
	//p2DRender->TextOut( nRedX+nEnhanceTeam[4], nTeamY, "Capture", 0xEA8DABC4, 0xED000000 );

	int nTeamXR = nRedX, nTCountI = 0;
	for( vector<DOMTEAM>::iterator it = m_DomTeam.begin(); it != m_DomTeam.end(); ++it )
	{
		const DOMTEAM& team = (*it);
		CString strTeam[5];
		strTeam[0].Format( "%u", team.nCaptureS ); //score
		strTeam[1].Format( "%u", team.nKill );
		strTeam[2].Format( "%u", team.nDeath );
		strTeam[3].Format( "%u", team.nTouch );
		strTeam[4].Format( "%u", 1 );

		if( nTCountI > 0 )
			nTeamXR = nBlueX;
			
		for( int i=0; i<4; i++ )
			p2DRender->TextOut( nTeamXR + nEnhanceTeam[i], nTeamY+21, strTeam[i], 0xAFFFFFFF, 0 );

		nTCountI++;
	}


	p2DRender->TextOut( nBlueX+nEnhance[0], nBlueY, "Rank", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nBlueX+nEnhance[1], nBlueY, "Name", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nBlueX+nEnhance[2], nBlueY, "Kill/Death", 0xEA8DABC4, 0xED000000 );   
	p2DRender->TextOut( nBlueX+nEnhance[3], nBlueY, "Job", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nBlueX+nEnhance[4], nBlueY, "Touch", 0xEA8DABC4, 0xED000000 ); 
	p2DRender->TextOut( nBlueX+nEnhance[5], nBlueY, "Capture", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nBlueX+nEnhance[6], nBlueY, "Point", 0xEA8DABC4, 0xED000000 );	  

	p2DRender->TextOut( nRedX+nEnhance[0], nRedY, "Rank", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nRedX+nEnhance[1], nRedY, "Name", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nRedX+nEnhance[2], nRedY, "Kill/Death", 0xEA8DABC4, 0xED000000 );   
	p2DRender->TextOut( nRedX+nEnhance[3], nRedY, "Job", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nRedX+nEnhance[4], nRedY, "Touch", 0xEA8DABC4, 0xED000000 ); 
	p2DRender->TextOut( nRedX+nEnhance[5], nRedY, "Capture", 0xEA8DABC4, 0xED000000 );
	p2DRender->TextOut( nRedX+nEnhance[6], nRedY, "Point", 0xEA8DABC4, 0xED000000 );

	CRect crLinePL;
	crLinePL.left = xPos +8;
	crLinePL.right = crLinePL.left + 407 -8 -8 -4;
	crLinePL.top = yPos + 52 + nLetterFontY;
	crLinePL.bottom = crLinePL.top + 2;
	p2DRender->RenderFillRect( crLinePL, D3DCOLOR_ARGB( 75, 0, 0, 0 ) ); 

	CRect crLinePR;
	crLinePR.left = xPos + 407 + 9;
	crLinePR.right = crLinePR.left + 407 - 8 -8 -4;
	crLinePR.top = crLinePL.top;
	crLinePR.bottom = crLinePL.bottom;
	p2DRender->RenderFillRect( crLinePR, D3DCOLOR_ARGB( 75, 0, 0, 0 ) ); 

	int nBluePlusX = nBlueX, nBluePlusY = nBlueY+27;
	int nRedPlusX = nRedX, nRedPlusY = nRedY+27; 
	int nRegisterRed = 1, nRegisterBlue = 1;
	for( vector<DOMPLAYER>::iterator it = m_DomPlayer.begin(); it != m_DomPlayer.end(); ++it )
	{
		const DOMPLAYER& domPlayer = (*it);

		CString strBoard[7];
		strBoard[0].Format( "%u", ( domPlayer.nTeam == TEAM_A  ) ? nRegisterRed : nRegisterBlue   );
		strBoard[1] = CString( domPlayer.strName, 6 );  
		strBoard[1] += _T( "..." );
		strBoard[2].Format( "%u/%u", domPlayer.nKill, domPlayer.nDeath );  
		strBoard[3] = CString( prj.m_aJob[domPlayer.nJob].szName, 6 );
		strBoard[3] += _T( "..." );
		strBoard[4].Format( "%u", domPlayer.nCaptureS );
		strBoard[5].Format( "%u", domPlayer.nTouch ); 
		strBoard[6].Format( "%u", domPlayer.nPoint );

		if( domPlayer.nTeam == TEAM_A )
		{
			for( int i =0; i<7; i++ )
			{
				p2DRender->TextOut( nRedPlusX + nEnhanceName[i], nRedPlusY, strBoard[i], 0xAFFFFFFF, 0x9A000000 );
			} 
			nRedPlusY += 17;
			nRegisterRed++;
		}
		else if( domPlayer.nTeam == TEAM_B )
		{
			for( int i =0; i<7; i++ )
			{
				p2DRender->TextOut( nBluePlusX + nEnhanceName[i], nBluePlusY, strBoard[i], 0xAFFFFFFF, 0x9A000000 );
			}
			nBluePlusY += 17;
			nRegisterBlue++;
		}
	}






/*
	CRect rectDom; //white Dom for Double Domination Score
	rectDom.top = 163;
	rectDom.left = x / 2 - 276;
	rectDom.bottom = rectDom.top + 20;
	rectDom.right = x / 2 + rectDom.left + 70;  
	p2DRender->RenderFillRect( rectDom, D3DCOLOR_ARGB( 30, 0xFF, 0xFF, 0xFF ) );
	p2DRender->TextOut( rectDom.left+8, rectDom.top+3, "Double Domination - Paradise FlyFF", 0xFFFBB917, 0xED000000 );
	CRect rectBoard;
	rectBoard.top = rectDom.bottom;
	rectBoard.left = rectDom.left;
	rectBoard.right = rectDom.right;
	rectBoard.bottom = rectBoard.top + 38 + 180; //( 18 * m_DomPlayer.size() );
	CRect rectRed = rectBoard;
	rectRed.right = rectBoard.right / 2 - 10;
	CRect rectBlue = rectBoard;
	rectBlue.left = rectRed.right + 20;
	p2DRender->RenderFillRect( rectRed, D3DCOLOR_ARGB( 30, 120, 10, 10 ) );
	p2DRender->RenderFillRect( rectBlue, D3DCOLOR_ARGB( 30, 10, 10, 120 ) );
	p2DRender->RenderFillRect( rectBoard, D3DCOLOR_ARGB( 30, 0, 0, 0 ) );
	p2DRender->TextOut( rectBoard.left+18, rectBoard.top+16, "Name", 0xFFFBB917, 0xED000000 );
	p2DRender->TextOut( rectBoard.left+68, rectBoard.top+16, "Kill/Death", 0xFFFBB917, 0xED000000 );
	p2DRender->TextOut( rectBoard.left+138, rectBoard.top+16, "Touch", 0xFFFBB917, 0xED000000 );
	p2DRender->TextOut( rectBoard.left+181, rectBoard.top+16, "Capture", 0xFFFBB917, 0xED000000 );
	p2DRender->TextOut( rectBoard.left+241, rectBoard.top+16, "Point", 0xFFFBB917, 0xED000000 );	

	int nEnhance[6] = { 0, 52, 6, 70, 43, 60 };

	int redPlusX = rectRed.left+18, redPlusY = rectRed.top+16+16;
	int bluePlusX = rectBlue.left+18, bluePlusY = rectBlue.top+16+16;
	for( vector<DOMPLAYER>::iterator it = m_DomPlayer.begin(); it != m_DomPlayer.end(); ++it )
	{
		DOMPLAYER& domPlayer = (*it);

		CString strBoard[6];
		strBoard[0] = CString( domPlayer.strName, 6 );
		strBoard[0] += _T( "..." );
		strBoard[1].Format( "%u", domPlayer.nKill );
		strBoard[2].Format( "%u", domPlayer.nDeath );
		strBoard[3].Format( "%u", domPlayer.nTouch );
		strBoard[4].Format( "%u", domPlayer.nCaptureS );
		strBoard[5].Format( "%u", domPlayer.nPoint );
		
		if( domPlayer.nTeam == TEAM_A )
		{
			for( int i =0; i<6; i++ )
			{
				p2DRender->TextOut( redPlusX, redPlusY, strBoard[i], 0xAFF7002D, 0xAA000000 ); //0084F7
				redPlusX += nEnhance[i];
				redPlusY += 16;
			}
		}
		else if( domPlayer.nTeam == TEAM_B )
		{
			for( int i =0; i<6; i++ )
			{
				p2DRender->TextOut( bluePlusX, bluePlusY, strBoard[i], 0xAF0084F7, 0xAA000000 ); //0084F7
				bluePlusX += nEnhance[i];
				bluePlusY += 16;
			}
		}
	}*/
}




#endif








