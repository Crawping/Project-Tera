#pragma once

#include "cCollisionObject.h"
#include "cBoundingSphere.h"

class cEnemySkinMesh;

class cEnemy :
	public cCollisionObject
{
public:
	cEnemy();
	virtual ~cEnemy();

	virtual void Update() override;
	virtual void Render() override;
	virtual void ActionState();

	//���� �����̱�
	virtual D3DXMATRIXA16 Move();
	virtual D3DXMATRIXA16 Rotate();
	virtual float RotateAngle();

	void ResetAttackDelay( );
	bool IsAbleAttacked( );

protected:
	cEnemySkinMesh*		m_pBody;		//���� �ٵ�

	D3DXMATRIXA16		m_matWorld;	//������ �����Ʈ����
	D3DXMATRIXA16		m_matLocal;
	D3DXMATRIXA16		matT;

	D3DXVECTOR3			m_vDirection;
	D3DXVECTOR3			m_vOrigin;	//���Ͱ� ������ �ʱ� ��ġ

	float				m_fSpeed;
	float				m_fAngle;
	float				m_fPeriod;
	float				m_fPassTime;
	bool				m_bIsAction;

	int					n;
	float				m_fAttackCurrDelay;
	float				m_fAttackMaxTime;
};

inline void cEnemy::ResetAttackDelay( )
{
	m_fAttackCurrDelay = 0.f;
}

inline bool cEnemy::IsAbleAttacked( )
{
	return ( m_fAttackMaxTime - m_fAttackCurrDelay ) <= 0.001f;
}