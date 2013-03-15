#pragma once

struct ARENAPLAYER
{
	u_int nJob;
	u_int nKill;
	u_int nDeath;
	u_int nRow;
	unsigned __int64 n64ArenaPoint;
	u_int nDeathMatchWin;
	u_int nDeathMatchLose;
	u_int nDuelWin;
	u_int nDuelLose;
	CString szName;
	BOOL bUpdate; //QryUpdate
	ARENAPLAYER()
	{
		nJob = nKill = nDeath = nRow =
		nDeathMatchWin = nDeathMatchLose =
		nDuelWin = nDuelLose = 0;
		bUpdate = FALSE;
		szName = _T("");
	};
};


class CArenaClient
{
private:
	CArenaClient(){ m_bFullType = FALSE;
	
	ARENAPLAYER player;
	player.szName = "Shunny";
	player.nKill = 32;
	player.nDeath = 11;


	
	for( int i =0; i<8; i ++ )
		m_Arena.push_back( player );

	//SetScore( m_Arena );

	
	};
	~CArenaClient(){};
public:
	static CArenaClient& GetInstance( void )
	{
		static CArenaClient obj;
		return obj;
	};
private:
	DWORD	m_dwArenaTick;
	BOOL	m_bFullType;
	vector<ARENAPLAYER>m_Arena;
public:
	void SetScore( vector<ARENAPLAYER>& vArena );
	void Reset() { m_Arena.clear(); };
	void Paint( C2DRender* p2DRender );
	void PaintFullType( C2DRender* p2DRender );
public:
	void ToggleType() { m_bFullType = !m_bFullType; };
	BOOL IsFullType() const { return m_bFullType; };
};