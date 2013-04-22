#pragma once

BOOL IsValidBase( DDOM_BASE base );

namespace DoubleDom
{
	namespace Color
	{
		static const unsigned long int nBlueCol = 0xFF50DEB3;
		static const unsigned long int nGoldCol = 0xFFD9850F;
	};
	namespace Point
	{
		static unsigned int nKillPoint = 8;
		static unsigned int nCapturePoint = 50;
		static unsigned int nTouchPoint = 14;
	};
	namespace Time
	{
		static unsigned char nByteTime = 8;
	};
};

namespace DoubleDom
{
	namespace Queue
	{
		//static unsigned int nAmount = 12;
		static unsigned int nRounds = 3;
		static unsigned int nAmount = 2;
	};
};

enum DDOM_INC
{
	INC_KILL,
	INC_DEATH,
	INC_TOUCH,
	INC_CAPTURE
};

enum DDOM_RUN
{
	RUN_CLOSED,
	RUN_FIGHT,
	RUN_COOLDOWN,
	RUN_EXIT
};

class CDDomTeam
{
public:
	CDDomTeam();
private:
	u_int nKill;
	u_int nDeath;
	u_int nTouch;
	u_int nCaptureS;
	vector<DOMPLAYER> m_TeamPlayer;
public:
	void Init( void );
	~CDDomTeam();
	u_int GetCaptures() const { return nCaptureS; };
	BOOL Add( CUser* pUser, DDOM_TEAM team );
	BOOL Remove( CUser* pUser );
	void SendPrize( void );
	size_t GetAmount() const { return m_TeamPlayer.size(); };
	void Serialize( CAr & ar );
	
	void Increase( u_long idPlayer, DDOM_INC inc );
};

class CBase
{
public:
	CBase(){  m_Team = MAX_TEAM; };
	~CBase(){};
private:
	DDOM_TEAM m_Team;
public:
	void SetTouchedBy( DDOM_TEAM team ) { m_Team = team; };
	DDOM_TEAM GetTouchTeam() const { return m_Team; };
};

class CDDom
{
	friend CDDomTeam;
private:
	CDDom();
	~CDDom(){};
	BYTE m_wTime;
	u_long	m_idCapUser;
	DWORD m_dwRoundTime;
	DWORD m_dwRound;
	DDOM_RUN m_RunTime;
	vector<u_long> m_User;
	CBase	m_Base[MAX_BASE];
	CDDomTeam m_DomTeam[MAX_TEAM];
	size_t m_splitSize;
public:
	static CDDom& GetInstance( void ){ static CDDom dom; return dom; }
	BOOL Join( CUser* pUser );
	BOOL Kick( CUser* pUser );
	BOOL Start( vector<u_long>& playerList );
	void Safe( CUser* pUser );
	void Kill( CUser* pAttack, CUser* pTarget );
	void SendScore( void );
	void Serialize( CAr & ar );
	void Send( CString strMessage );
	void Send( CString strMessage, FONT_MSG font, D3DCOLOR d3dcolor );
	void Touch( CUser* pUser, DDOM_BASE base );
	void Process( void );
	void Capped( DDOM_TEAM team );
	void End( void );
	//void Increase( DDOM_TEAM team, DDOM_INC inc ); 
private:
	void ReturnAll( void );
	void SendTouch( DDOM_TEAM team, DDOM_BASE base, BOOL bCaptured = FALSE );
	void SendBaseTouch( CString strMessage, DDOM_BASE base, DDOM_TEAM team );
	CString GetName( DDOM_TEAM team );
	CString GetBaseName( DDOM_BASE base );
private:
	void StartRound( void );
	void SetRunTime( DDOM_RUN runtime ) { m_RunTime = runtime; };
public:
	void ResetCapTeam( void );
	DDOM_RUN GetRunTime() const { return m_RunTime; };
	void ResetTime() { m_wTime = 0; };
	BYTE GetTime() const { return m_wTime; }
	void SetTime() { m_wTime = DoubleDom::Time::nByteTime; };
	void DecreaseTime() { if( m_wTime > 0 ) { m_wTime--; } };
	DDOM_BASE GetOponentBase( DDOM_BASE base );
	DDOM_TEAM GetOponentTeam( DDOM_TEAM team );
};



