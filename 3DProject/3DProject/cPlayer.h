#pragma once
#include "cCollisionObject.h"

enum class ePlayerState
{
	PLAYER_IDLE,
	PLAYER_RUN,
	PLAYER_ATTACK,
};

class cBody;
class cHair;
class cFace;
class cPlayer : 
	public cCollisionObject
{
public:
	cPlayer( );
	virtual ~cPlayer( );

	virtual void Update( ) override;
	virtual void Render( ) override;

	void SetUpdateState();	//���¿� ���� ������Ʈ�� �ٸ��� ����
	void SetRenderState();	//���¿� ���� ������ �ٸ��� ����
	void KeyControl();

private:
	cBody*			m_pIdleBody;
	cHair*			m_pIdleHair;
	cFace*			m_pIdleFace;
		 
	cBody*			m_pAttackBody;
	cHair*			m_pAttackHair;
	cFace*			m_pAttackFace;
		 
	cBody*			m_pRunBody;
	cHair*			m_pRunHair;
	cFace*			m_pRunFace;

	D3DXMATRIXA16	m_matWorld;
	//D3DXVECTOR3	m_vPosition;
	D3DXVECTOR3		m_vDirection;

	float			m_fSpeed;
	float			m_fAngle;
	ePlayerState	m_sState;
};
