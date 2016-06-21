#pragma once
#include "cUnitObject.h"
#include "cBoundingSphere.h"

class cNpcSkinnedMesh;

enum eEnemyState
{
	ENEMY_IDLE = 0,				//���� ���ֱ�
	ENEMY_RUN = 1,				//���� �̵�
	ENEMY_DEATHWAIT = 2,		//���� ����
	ENEMY_DEATH = 3,			//���� �ױ� ����
	ENEMY_ATTACK = 4,			//���� ����
	ENEMY_SKILL1 = 5,			//���� ��ų1
	ENEMY_SKILL2 = 6,			//���� ��ų2
	ENEMY_BACKPOSITION = 7,		//���� �ǵ��ư���
	ENEMY_CHASE = 8,			//�÷��̾� �i�ư���
	ENEMY_NOTHING
};

class cEnemy :
	public cUnitObject
{
public:
	cEnemy();
	virtual ~cEnemy();

	virtual void Update() override;
	virtual void Render() override;
	virtual void ActionState();
	virtual void OnCollisionStay(cCollisionObject* rhs) override ;

	//���� �����̱�
	virtual D3DXMATRIXA16 Move();
	virtual D3DXMATRIXA16 Rotate();
	virtual float RotateAngle();

	void SetEnemyState( eEnemyState e );
	eEnemyState GetEnemyState( );
	void ResetAttackDelay( );
	bool IsAbleAttacked( );

protected:
	eEnemyState m_sEState;

	cNpcSkinnedMesh*	m_pBody;

	D3DXMATRIXA16		m_matWorld;	//������ �����Ʈ����
	D3DXMATRIXA16		m_matLocal;
	D3DXMATRIXA16		matT;

	D3DXVECTOR3			m_vDirection;
	D3DXVECTOR3			m_vOrigin;	//���Ͱ� ������ �ʱ� ��ġ

	float				m_fSpeed;
	float				m_fAngle;
	float				m_fPeriod;
	float				m_fPassTime;
	float				m_fDeathTime;	//�״� �ð��� ���͸��� �޶� ��������
	bool				m_bIsAction;

	int					n;
	float				m_fAttackCurrDelay;
	float				m_fAttackMaxTime;

	std::vector<cEnemy*> m_vecEnemy;
};

inline void cEnemy::ResetAttackDelay( )
{
	m_fAttackCurrDelay = 0.f;
}

inline bool cEnemy::IsAbleAttacked( )
{
	return ( m_fAttackMaxTime - m_fAttackCurrDelay ) <= 0.001f;
}

inline void cEnemy::SetEnemyState( eEnemyState e )
{
	m_sEState = e;
}

inline eEnemyState cEnemy::GetEnemyState( )
{
	return m_sEState;
}