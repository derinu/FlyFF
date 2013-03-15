#pragma once

namespace Arena
{
	namespace Point
	{
		static unsigned long nMultiplierFirst = 43;
		static unsigned long nMultiplierSecond = 137+1; //xRandom+1(?)
	};
	namespace Time
	{
		static DWORD nProcessTime = 180;
	};
};

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
		n64ArenaPoint = 0;
		szName = _T("");
	};
};

enum ARENA_ADD_TYPE
{
	ARENA_ADD_FAILURE,
	ARENA_ADD_ALREADY_EXIST,
	ARENA_ADD_SUCCESS
};

typedef int nomember;

class CArena
{
private:
	CArena(){
	m_dwArenaProcessTime = 0;
	};
	~CArena(){};
public:
	static CArena& GetInstance( void )
	{
		static CArena obj;
		return obj;
	};
private:
	DWORD m_dwArenaProcessTime;
	map<u_long, ARENAPLAYER>m_mArenaMap;
	map<u_long, nomember> m_Online;
private:
	u_long GetPointMultiplierFactor( void );
	void ResetTime() { m_dwArenaProcessTime = Arena::Time::nProcessTime; };
public:
	void DecreaseTime()
	{
		if( m_dwArenaProcessTime > 0 )
		{
			m_dwArenaProcessTime--;
		}
	};
	DWORD GetTime() const { return m_dwArenaProcessTime; };
public:
	ARENA_ADD_TYPE Add( CUser* pUser );
	ARENAPLAYER* GetPlayer( u_long idPlayer );
	void Serialize( CAr & ar );
	void Kill( CUser* pUser, CUser* pDeath );
	void Process( void );
	BOOL Load( void );
private:
	BOOL SaveToDatabase( void );
	BOOL AddToDatabase( u_long idPlayer, CString szName, u_int nJob );
	//ARENAPLAYER QueryPlayerFromDatabase( u_long idPlayer ); //directly querying now unallowed
	//player MUST exist on map, otheriwse ---> NULL!
public: //Online User Management
	void AddOnline( CUser* pUser );
	void RemoveOnline( CUser* pUser );
	void Send( void );
	void Send( CString strMessage );

};

BOOL LoadArena( void );