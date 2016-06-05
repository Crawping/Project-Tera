#pragma once

#include "cCollisionObject.h"
#include "cBoundingSphere.h"

enum eEnemyState
{
	ENEMY_IDLE			= 0,		//���� ���ֱ�
	ENEMY_RUN			= 1,		//���� �̵�
	ENEMY_DEATHWAIT		= 2,		//���� ����
	ENEMY_DEATH			= 3,		//���� �ױ� ����
	ENEMY_ATTACK		= 4,		//���� ����
	ENEMY_SKILL1		= 5,		//���� ��ų1
	ENEMY_SKILL2		= 6,		//���� ��ų2
	ENEMY_BACKPOSITION	= 7,		//���� �ǵ��ư���
	ENEMY_CHASE			= 8,		//�÷��̾� �i�ư���
};

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

	virtual int GetCurrentHp() { return m_nCurrentHp; }
	virtual int GetMaxHp() { return m_nMaxHp; }
	virtual void SetCurrentHp(int currHp) { m_nCurrentHp = currHp; }

	//���� �����̱�
	virtual D3DXMATRIXA16 Move();
	virtual D3DXMATRIXA16 Rotate();
	virtual float RotateAngle();

	virtual void SetPosition(D3DXVECTOR3* vPos) { m_vPosition = *vPos; }
	virtual D3DXVECTOR3 GetPosition() { return m_vPosition; }

protected:
	cEnemySkinMesh*		m_pBody;		//���� �ٵ�

	int	  				m_nMaxHp;		//������ �ִ� ��
	int	  				m_nCurrentHp;	//��������� ��

	D3DXMATRIXA16		m_matWorld;	//������ �����Ʈ����
	D3DXMATRIXA16		m_matLocal;
	D3DXMATRIXA16		matT;

	D3DXVECTOR3			m_vPosition;
	D3DXVECTOR3			m_vDirection;
	D3DXVECTOR3			m_vOrigin;	//���Ͱ� ������ �ʱ� ��ġ

	float				m_fSpeed;
	float				m_fAngle;
	float				m_fPeriod;
	float				m_fPassTime;

	bool				m_bIsAction;
	bool				m_bAlive;

	eEnemyState			m_sState;
	int				n;
};

