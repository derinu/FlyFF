#pragma once

enum DOMINATING_TEAM
{
	DTEAM_NONE,
	DTEAM_A,
	DTEAM_B
};

DOMINATING_TEAM DomTeamToDominatingTeam( DDOM_TEAM team );
DDOM_TEAM DominatingTeamToDomTeam( DOMINATING_TEAM team );

class CDDomWidgetA
{
private:
	DOMINATING_TEAM m_ToBeDominated;
	DWORD m_dwTime;

	CDDomWidgetA();
	~CDDomWidgetA(){};
public:
	static CDDomWidgetA& GetInstance( void ) { static CDDomWidgetA widget; return widget; };
public:
	DOMINATING_TEAM m_BaseA;
	DOMINATING_TEAM m_BaseB;
	DWORD GetAlpha( void );
	void Paint( C2DRender* p2DRender );
	void SetCapTeam( DOMINATING_TEAM team );
	void MakeColor( D3DCOLOR& d3dcolor, DOMINATING_TEAM team );
	void MakeColor( D3DCOLOR& d3dcolor, DWORD nTime );
	void SetDominatingTeam( DDOM_BASE base, DOMINATING_TEAM team );
	void PaintMiddleNumber( C2DRender* p2DRender, int x, int y );
	DOMINATING_TEAM GetDominatingTeam( DDOM_BASE base );
public:
	DWORD GetTimeNumber( void );

};
