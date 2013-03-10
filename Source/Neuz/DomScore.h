#pragma once

class CDDomScore
{
private:
	CDDomScore();
	~CDDomScore(){};
	void Init( void ) { m_DomPlayer.clear(); m_DomTeam.clear(); };
public:
	static CDDomScore& GetInstance( void ){ static CDDomScore obj; return obj; };
private:
	vector<DOMPLAYER> m_DomPlayer;
	vector<DOMTEAM> m_DomTeam;
public:
	void SetData( vector<DOMPLAYER>& dataPlayer, vector<DOMTEAM>& dataTeam );
	void Render( C2DRender* p2DRender, const int& xPos, const int& yPos );
};
	