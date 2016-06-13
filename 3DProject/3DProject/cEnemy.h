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
	virtual void OnCollisionStay(cCollisionObject* rhs) override;

	//���� �����̱�
	virtual D3DXMATRIXA16 Move();
	virtual D3DXMATRIXA16 Rotate();
	virtual float RotateAngle();

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
	float				m_fDeathTime;	//�״� �ð��� ���͸��� �޶� ��������
	bool				m_bIsAction;

	int					n;
};

