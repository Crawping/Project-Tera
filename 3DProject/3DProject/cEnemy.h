#pragma once

#include "cCollisionObject.h"

class cBoundingSphere;
class cBody;

class cEnemy :
	public cCollisionObject
{
public:
	cEnemy();
	virtual ~cEnemy();

	virtual void Update() override;
	virtual void Render() override;

	virtual cBoundingSphere* GetBound() { return m_pBound; }
	virtual int GetCurrentHp() { return m_nCurrentHp; }
	virtual int GetMaxHp() { return m_nMaxHp; }
	virtual D3DXMATRIXA16 GetWorld() { return m_matWorld; }

	virtual void SetBound(cBoundingSphere* bound) { m_pBound = bound; }
	virtual void SetCurrentHp(int currHp) { m_nCurrentHp = currHp; }
	virtual void SetWorld(D3DXMATRIXA16* world) { m_matWorld = *world; }

protected:
	cBoundingSphere*		m_pBound;		//���� �ٿ���ڽ�
	cBody*					m_pBody;		//���� �ٵ�

	int						m_nMaxHp;		//������ �ִ� ��
	int						m_nCurrentHp;	//��������� ��
		
	D3DXMATRIXA16			m_matWorld;		//���� ���� ��Ʈ����
};

