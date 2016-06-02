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

	virtual int GetCurrentHp() { return m_nCurrentHp; }
	virtual int GetMaxHp() { return m_nMaxHp; }

	virtual void SetCurrentHp(int currHp) { m_nCurrentHp = currHp; }

protected:
	cBody*	m_pBody;		//���� �ٵ�

	int	  	m_nMaxHp;		//������ �ִ� ��
	int	  	m_nCurrentHp;	//��������� ��
};

