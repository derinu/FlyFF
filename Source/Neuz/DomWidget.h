#pragma once


enum RENDER_MODE
{
	RENDER_DISABLED,
	RENDER_SMALL,
	RENDER_FULL
};

typedef struct _tagDOMQUEUE
{
	u_long idPlayer;
	CString szName;
	int nJob;
	int nLevel;
	int nRebirth;
	DWORD WorldId;
	_tagDOMQUEUE()
	{
		szName = _T("");
		nJob = nLevel = nRebirth = 0;
		idPlayer = WorldId = 0;
	};
	bool operator==( const u_long& IDPlayer )
	{
		return this->idPlayer == IDPlayer;
	};
} DOMQPL, *LPDOMQPL;

class CDDomWidget
{
private:
	D3DCOLOR m_d3dTransColor[5];
	DWORD m_dwTransition;
	CD3DFont* m_pFont;
	RENDER_MODE m_RenderMode;
	vector<DOMQPL> m_vecDomPlayer;
	CDDomWidget()
	{ 
		m_RenderMode = RENDER_DISABLED; 
		m_pFont = NULL;

		for( int i =0; i<5; i++ )
			m_d3dTransColor[i] = 0xEF7AA2D6;
	};
	~CDDomWidget(){};
public:
	static CDDomWidget& GetInstance( void )
	{
		static CDDomWidget widget;
		return widget;
	}
public:
	void PaintFont( C2DRender* p2DRender );
	void PaintWidget( C2DRender* p2DRender );
	void SetFont( CD3DFont* pFont );
	void SetData( vector<DOMQPL>& vecDomPlayer );
	void ResetData( void ) { m_vecDomPlayer.clear(); m_RenderMode = RENDER_DISABLED; };
private:
	void SetColorTransition( void );
};

