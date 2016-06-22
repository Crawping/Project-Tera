#pragma once
#include "cUnitObject.h"
#include "cCommandCombo.h"

#define TUMBLINGRANGE 150.f

class cSkinnedMesh;
class cPlayerWeapon;

enum ePlayerState
{
	PLAYER_BATTLEIDLE = 5,
	PLAYER_RUN = 46,
	PLAYER_TUMBLING = 6,
	PLAYER_SKILL1 = 26,			//���ϰ� �������
	PLAYER_SKILL2 = 0,			//������
	PLAYER_SKILL3 = 27,			//���ϰ� ������ ���
	PLAYER_SKILL4 = 28,			//ȸ����ġ��
	PLAYER_COMBO1 = 2,			//�÷��̾� �޺�1
	PLAYER_COMBO2 = 4,			//�÷��̾� �޺�2
	PLAYER_COMBO3 = 8,			//�÷��̾� �޺�3
	PLAYER_COMBO4 = 17,			//�÷��̾� �޺�4
	PLAYER_DEATH = 25,			//�÷��̾� ����
	PLAYER_DEATHWAIT = 24,		//�÷��̾� ��������
};

class cPlayer : 
	public cUnitObject
{
public:
	cPlayer( );
	virtual ~cPlayer( );

	virtual void Update( ) override;
	virtual void Render( ) override;

public:
	virtual void OnCollisionEnter( int colliderIndex, cCollisionObject* rhs ) override;
	virtual void OnCollisionStay( int colliderIndex, cCollisionObject* rhs ) override;
	virtual void OnCollisionEnd( int colliderIndex, cCollisionObject* rhs ) override;

public:
	void SetPlayerState( ePlayerState p );
	ePlayerState GetPlayerState( );

	void SetUpdateState();	//���¿� ���� ������Ʈ�� �ٸ��� ����
	void SetRenderState();	//���¿� ���� ������ �ٸ��� ����
	void SetFSMState();
	void KeyControl();
	void SetAniTrack(int nIndex);

private:
	ePlayerState	m_sPState;
	
	std::vector<cSkinnedMesh*> m_vecSkinnedMesh;
	cPlayerWeapon*	m_playerWeapon;

	cCommandCombo*	m_pCombo;

	D3DXMATRIXA16	m_matWorld;
	D3DXVECTOR3		m_vDirection;
	
	//��ü �׸���

	float			m_fSpeed;
	float			m_fPassTime;
	float			m_fPeriod;

	bool			m_bIsAction;
	bool			m_bAlive;
	bool			m_bPushBehind;
	float			n;
};

inline void cPlayer::SetPlayerState(ePlayerState p)
{
	m_sPState = p;
}

inline ePlayerState cPlayer::GetPlayerState()
{
	return m_sPState;
}