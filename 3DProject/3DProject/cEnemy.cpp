#include "stdafx.h"
#include "cEnemy.h"
#include "cEnemySkinMesh.h"


cEnemy::cEnemy()
	:cCollisionObject("monster")
	, m_pBody(nullptr)
	, m_nMaxHp(1000)
	, m_nCurrentHp(1000)
	, n(0)
	, m_fSpeed(0.5f)
	, m_fAngle(0.f)
	, m_sState(ENEMY_RUN)
	, m_bIsAction(false)
	, m_vOrigin(0, 0, 0)
	, m_vPosition(0, 0, 0)
	, m_vDirection(1, 0, 0)
	, m_bAlive(true)
{
	D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&matT);
}


cEnemy::~cEnemy()
{
}

void cEnemy::Update()
{
	__super::Update();

	//���Ͱ� ������ �� �ִ� ���� ����
	float Length = D3DXVec3Length(&(m_vPosition - m_vOrigin));

	//���Ϳ� �÷��̾��� ���� �Ÿ� 
	float Distance = D3DXVec3Length(&(m_vPosition - g_player->GetPosition()));

	//������ HP�� 0�ε� ���� ���� ó���� �ȵǾ��ٸ�
	if (m_nCurrentHp < 0.f && m_bAlive)
	{
		m_sState = ENEMY_DEATH;
		
		//���� ���� �ൿ�� �����ϰ� ��������� �ൿ�� �� ��
		m_bIsAction = false;
	}

	//���Ͱ� ���� ������ �Ѿ�� �ٽ� �ǵ��� �� ��
	else if (abs(Length) > 300.f && m_bAlive)
	{
		m_sState = ENEMY_BACKPOSITION;

		//���� ���� �ൿ�� �����ϰ� ��������� �ൿ�� �� ��
		m_bIsAction = false;

	}

	//���� ������ ���°� �ǵ��ư��Ⱑ �ƴ϶�� �÷��̾� �i��
	else if (abs(Distance) < 100.f && m_bAlive && m_sState != ENEMY_BACKPOSITION)
	{
		m_sState = ENEMY_CHASE;

		//���� ���� �ൿ�� �����ϰ� ��������� �ൿ�� �� ��
		m_bIsAction = false;
	}

	ActionState();

	if (m_pBody)
	{
		m_pBody->Update();
		m_pBody->SetWorld(&m_matWorld);
	}
}

void cEnemy::Render()
{
	__super::Render();

	if (m_pBody)
	{
		m_pBody->Render();
	}
}

void cEnemy::ActionState()
{
	switch (m_sState)
	{
	case ENEMY_IDLE:
	{
		//�� �ൿ�� ���� �޽����� ��� �޾�����
		if (!m_bIsAction)
		{
			m_pBody->SetAnimationIndex(ENEMY_IDLE);
			m_bIsAction = true;
			m_fPeriod = 2.f;
		}

		//��� ���ð��� ����Ǹ� �����ϱ�� ���ư���.
		if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_fPeriod = 0.f;
				m_fPassTime = 0.f;
				m_sState = ENEMY_RUN;
				m_bIsAction = false;
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}

		int a = 0;
	}

	break;
	case ENEMY_DEATH:
	{
		//�� �ൿ�� ���� �޽����� ��� �޾�����
		if (!m_bIsAction)
		{
			m_pBody->SetAnimationIndex(ENEMY_DEATH);
			m_bIsAction = true;
			m_fPeriod = m_pBody->GetAniTrackPeriod(3) - 0.25f;
		}

		if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				m_sState = ENEMY_DEATHWAIT;

				//��� ���� ó���� �� �������Ƿ� ����ִٴ� ���� false ��Ų��.
				m_bAlive = false;
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}
		int a = 0;

	}
	break;

	case ENEMY_DEATHWAIT:
	{
		//�� �ൿ�� ���� �޽����� ��� �޾�����
		if (!m_bIsAction)
		{
			m_pBody->SetAnimationIndex(ENEMY_DEATHWAIT);
			m_bIsAction = true;
			m_fPeriod = 3.f;
		}

		if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				m_sState = ENEMY_DEATHWAIT;
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}
		int a = 0;

	}
		break;
	
	case ENEMY_BACKPOSITION:
	{
		//���� ���Ͱ� ���� �������� ����ٸ� �ڽ��� ������ ��ġ�� ���ư�
		float Length = D3DXVec3Length(&(m_vPosition - m_vOrigin));

		if (!m_bIsAction)
		{
			//�޸��� ������� �ٲپ� �ش�.
			m_bIsAction = true;
			m_pBody->SetAnimationIndex(ENEMY_RUN);
		}

		if (m_bIsAction)
		{
			if (abs(Length) < 1.f)
			{
				//���� ��ġ�� �����ߴٸ�? �ٽ� IDLE ���·� ������.
				m_bIsAction = false;
				m_sState = ENEMY_IDLE;
			}

			//���� ���� ��ġ�� ���ƿ��� �ʾҴٸ� ��� �̵��� ��
			else if (abs(Length) >= 1.f)
			{
				D3DXMATRIX matR;

				m_vDirection = m_vOrigin - m_vPosition;
				D3DXVec3Normalize(&m_vDirection, &m_vDirection);

				D3DXMatrixLookAtLH(&matR,
					&D3DXVECTOR3(0, 0, 0),
					&D3DXVECTOR3(m_vDirection.x, m_vDirection.y, m_vDirection.z),
					&D3DXVECTOR3(0, 1, 0));

				m_matWorld = matR * Move();
			}
		}

		int a = 0;

	}
		break;

	case ENEMY_RUN:
	{
		//���� �ð�
		m_fPeriod = 1.f;

		//�ൿ�� ���۵��� �˸�
		if (!m_bIsAction)
		{
			//Run �ִϸ��̼� ����
			m_pBody->SetAnimationIndex(ENEMY_RUN);
			m_bIsAction = true;
			m_fAngle = RotateAngle();
		}

		if (m_bIsAction)
		{
			//�ֱ� �ð����� ������ �ð��� ũ�ٸ� IDLE ������� ��ȯ
			if (m_fPassTime > m_fPeriod)
			{
				m_fPeriod = 0.f;
				m_fPassTime = 0.f;
				m_sState = ENEMY_IDLE;
				m_bIsAction = false;
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
				m_matWorld = Rotate() * Move();
			}
		}

		int a = 0;

	}
	break;
	case ENEMY_ATTACK:
		break;
	case ENEMY_SKILL1:
		break;
	case ENEMY_SKILL2:
		break;
	case ENEMY_CHASE:
	{
		//���� ���Ͱ� ���� ������ ����� �ʾҴٸ� �÷��̾ �i�ư���.
		float Distance = D3DXVec3Length(&(m_vPosition - g_player->GetPosition()));

		if (!m_bIsAction)
		{
			m_bIsAction = true;
			m_pBody->SetAnimationIndex(ENEMY_RUN);
		}

		if (m_bIsAction)
		{
			if (abs(Distance) < 10.f)
			{
				//���� ��ġ�� �����ߴٸ�? �ٽ� IDLE ���·� ������.
				m_bIsAction = false;
				m_sState = ENEMY_ATTACK;
			}

			//���� ���� ��ġ�� ���ƿ��� �ʾҴٸ� ��� �̵��� ��
			else if (abs(Distance) >= 10.f)
			{
				D3DXMATRIX matR, matTemp;

				m_vDirection = g_player->GetPosition() - m_vPosition;
				D3DXVec3Normalize(&m_vDirection, &m_vDirection);

				D3DXMatrixLookAtLH(&matR,
					&D3DXVECTOR3(0, 0, 0),
					&-D3DXVECTOR3(m_vDirection.x, m_vDirection.y, m_vDirection.z),
					&D3DXVECTOR3(0, 1, 0));


				D3DXMatrixTranspose(&matR, &matR);
				/*D3DXMatrixRotationY(&matTemp, -D3DX_PI / 2.f);

				matR *= matTemp;*/

				m_matWorld = matR * Move();
			}

			//�ʹ� �������ٸ�?
			else if (abs(Distance) >= 100.f)
			{
				D3DXMATRIX matR, matTemp;
				m_bIsAction = false;
				m_sState = ENEMY_BACKPOSITION;
			}
		}
		int a = 0;
	}
		break;
	}
}

D3DXMATRIXA16 cEnemy::Move()
{
	D3DXMATRIXA16 matT;

	m_vPosition = m_vPosition + m_vDirection * m_fSpeed;

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	
	return matT;
}

D3DXMATRIXA16 cEnemy::Rotate()
{
	D3DXMATRIXA16 matR;

	m_vDirection = D3DXVECTOR3(1, 0, 0);
	D3DXMatrixRotationY(&matR, m_fAngle);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);
	
	return matR;
}

float cEnemy::RotateAngle()
{
	//���� �� ���Ͱ� �ٶ󺸴� ������ �׻� �ٸ���.
	float Angle = cRandomUtil::GetFloat(D3DX_PI * 2, 0);

	return Angle;
}