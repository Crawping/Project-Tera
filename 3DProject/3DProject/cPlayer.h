#pragma once
#include "cCollisionObject.h"
#include "cCommandCombo.h"

#define TUMBLINGRANGE 150.f

class cTail;
class cBody;
class cHair;
class cFace;
class cWeaponMesh;
class cPlayerWeapon;

class cPlayer : 
	public cCollisionObject
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
	void SetUpdateState();	//���¿� ���� ������Ʈ�� �ٸ��� ����
	void SetRenderState();	//���¿� ���� ������ �ٸ��� ����
	void SetFSMState();
	void KeyControl();
	void SetAniTrack(int nIndex);

private:
	cBody*			m_pBody;
	cHair*			m_pHair;
	cFace*			m_pFace;
	cTail*			m_pTail;
	cWeaponMesh*	m_pHand;
	cPlayerWeapon*	m_playerWeapon;
	cCommandCombo*	m_pCombo;

	D3DXMATRIXA16	m_matWorld;
	D3DXVECTOR3		m_vDirection;

	float			m_fSpeed;
	float			m_fPassTime;
	float			m_fPeriod;

	bool			m_bIsAction;
	bool			m_bAlive;
	bool			m_bPushBehind;
	float			n;
};
