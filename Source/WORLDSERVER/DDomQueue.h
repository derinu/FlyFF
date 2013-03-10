#pragma once

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


class CDDomQueue
{
private:
	CDDomQueue(){};
	~CDDomQueue(){};
public:
	static CDDomQueue& GetInstance( void )
	{
		static CDDomQueue obj;
		return obj;
	};
private:
	vector<DOMQPL> m_PlayerQueue;
public:
	void Add( CUser* pUser );
	void Remove( CUser* pUser );
	void Send( void );
	void Serialize( CAr & ar );
};

