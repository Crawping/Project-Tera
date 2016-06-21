#include "stdafx.h"
#include "cEnemy.h"
#include "Console.h"

cEnemy::cEnemy()
	: n(0)
	, m_fSpeed(0.5f)
	, m_fAngle(0.f)
	, m_bIsAction(false)
	, m_bOncePlay(false)
	, m_vOrigin(0, 0, 0)
	, m_vDirection(1, 0, 0)
{
	this->SetCurrHp(50);
	SetEnemyState(ENEMY_IDLE);
	D3DXMatrixTranslation(&m_matWorld, GetPosition().x, GetPosition().y, GetPosition().z);

	m_vOrigin = this->GetPosition();

	this->SetObjectType(ObjectType::eMonster);

	D3DXMatrixIdentity(&m_matLocal);
	D3DXMatrixIdentity(&matT);
}

cEnemy::~cEnemy()
{
	SAFE_DELETE( m_pBody );
}

void cEnemy::Update()
{
	__super::Update();

	//���Ͱ� ������ �� �ִ� ���� ����
	float Length = D3DXVec3Length(&(GetPosition() - m_vOrigin));
	//���Ϳ� �÷��̾��� ���� �Ÿ� 
	float Distance = D3DXVec3Length(&(GetPosition() - g_player->GetPosition()));

	m_fAttackCurrDelay += g_pTimeManager->GetDeltaTime();
	if (m_fAttackCurrDelay >= m_fAttackMaxTime)
	{
		m_fAttackCurrDelay = m_fAttackMaxTime;
	}

	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		this->SetCurrHp(50);
		SetEnemyState(ENEMY_IDLE);
		this->SetDead(false);
		m_bIsAction = true;
		m_fPassTime = 0.f;
		m_fPeriod = 0.f;
	}

	//������ HP�� 0�ε� ���� ���� ó���� �ȵǾ��ٸ�
	if (GetCurrHp() < 0.f && !this->IsDead())
	{
		//�ش� �̺�Ʈ�� ���� ���� �ƴ�
		if (GetEnemyState() != ENEMY_DEATH)
		{
			//���� ���� �ൿ�� �����ϰ� ��������� �ൿ�� �� ��
			m_bIsAction = false;

			//��� ���� ó���� �� �������Ƿ� ����ִٴ� ���� false ��Ų��.
			this->SetDead(true);
			SetEnemyState(ENEMY_DEATH);
		}
	}

	//���� ������ ��ġ�� �÷��̾�� �����ٸ� ���ݸ��
	else if (abs(Distance) < 40.f && !this->IsDead() && GetEnemyState() != ENEMY_BACKPOSITION)
	{
		//�ش� �̺�Ʈ�� ���� ���� �ƴ�
		if (GetEnemyState() != ENEMY_ATTACK)
		{
			SetEnemyState(ENEMY_ATTACK);

			//���� ���� �ൿ�� �����ϰ� ��������� �ൿ�� �� ��
			m_bIsAction = false;
		}
	}

	//���Ͱ� ���� ������ �Ѿ�� �ٽ� �ǵ��� �� ��
	else if (abs(Length) > 300.f && !this->IsDead())
	{
		//�ش� �̺�Ʈ�� ���� ���� �ƴ�
		if (GetEnemyState() != ENEMY_BACKPOSITION && GetEnemyState() != ENEMY_CHASE &&
			GetEnemyState() != ENEMY_ATTACK)
		{
			SetEnemyState(ENEMY_BACKPOSITION);

			//���� ���� �ൿ�� �����ϰ� ��������� �ൿ�� �� ��
			m_bIsAction = false;
		}
	}

	//���� ������ ���°� �ǵ��ư��Ⱑ �ƴ϶�� �÷��̾� �i��
	else if (abs(Distance) < 100.f && !this->IsDead() && GetEnemyState() != ENEMY_BACKPOSITION)
	{
		//�ش� �̺�Ʈ�� ���� ���� �ƴ�
		if (GetEnemyState() != ENEMY_CHASE && GetEnemyState() != ENEMY_ATTACK)
		{
			SetEnemyState(ENEMY_CHASE);

			//���� ���� �ൿ�� �����ϰ� ��������� �ൿ�� �� ��
			m_bIsAction = false;
		}
	}

	ActionState();
	
	for (size_t i = 0; i < this->GetColliderRepo().size(); i++)
	{
		this->GetColliderRepo()[i]->SetWorld(&m_matWorld);
		D3DXMATRIXA16 mat = this->GetColliderRepo()[i]->GetLocal() * this->GetColliderRepo()[i]->GetWorld();
		D3DXVECTOR3 vPos = D3DXVECTOR3(mat._41, mat._42, mat._43);
		this->GetColliderRepo()[i]->SetPosition(vPos);
	}

	m_pBody->SetWorld(&m_matWorld);
}

void cEnemy::Render()
{
	__super::Render();

	if (m_pBody)
	{
		m_pBody->UpdateAndRender();
	}
}

void cEnemy::ActionState()
{
	switch (GetEnemyState())
	{
	case ENEMY_IDLE:
	{
		//�� �ൿ�� ���� �޽����� ��� �޾�����
		if (!m_bIsAction)
		{
			m_pBody->SetAnimationIndex(ENEMY_IDLE);
			m_bIsAction = true;
			m_fPeriod = m_pBody->GetAniTrackPeriod(ENEMY_IDLE);
		}

		//��� ���ð��� ����Ǹ� �����ϱ�� ���ư���.
		if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_fPeriod = 0.f;
				m_fPassTime = 0.f;
				SetEnemyState(ENEMY_RUN);
				m_bIsAction = false;
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}
	}
	break;

	case ENEMY_DEATH:
	{
		//�� �ൿ�� ���� �޽����� ��� �޾�����
		if (!m_bIsAction)
		{
			std::string Name;
			Name = m_sName + std::string("����");
			SOUNDMANAGER->play(Name, 1.f);
			m_bIsAction = true;
			m_pBody->SetAnimationIndex(ENEMY_DEATH);
			m_fPassTime = 0.f;
		}

		if (m_bIsAction)
		{
			if (m_fPassTime > m_fDeathTime)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetEnemyState(ENEMY_DEATHWAIT);
			}

			else if (m_fPassTime < m_fDeathTime)
			{
				Log(m_fPassTime);
				Log("\n");
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}
	}
	break;

	case ENEMY_DEATHWAIT:
	{
		//�� �ൿ�� ���� �޽����� ��� �޾�����
		if (!m_bIsAction)
		{
			m_pBody->SetAnimationIndex(ENEMY_DEATHWAIT);
			m_bIsAction = true;
			m_fPeriod = m_pBody->GetAniTrackPeriod(ENEMY_DEATHWAIT);
		}

		if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetEnemyState(ENEMY_NOTHING);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}
	}
	break;

	case ENEMY_BACKPOSITION:
	{
		//���� ���Ͱ� ���� �������� ����ٸ� �ڽ��� ������ ��ġ�� ���ư�
		float Length = D3DXVec3Length(&(GetPosition() - m_vOrigin));

		if (!m_bIsAction)
		{
			//�޸��� ������� �ٲپ� �ش�.
			m_bIsAction = true;
			m_pBody->SetAnimationIndex(ENEMY_RUN);
			int a = 0;
		}

		if (m_bIsAction)
		{
			if (abs(Length) < 40.f)
			{
				//���� ��ġ�� �����ߴٸ�? �ٽ� IDLE ���·� ������.
				m_bIsAction = false;
				SetEnemyState(ENEMY_IDLE);
			}

			//���� ���� ��ġ�� ���ƿ��� �ʾҴٸ� ��� �̵��� ��
			else if (abs(Length) >= 40.f)
			{
				D3DXMATRIX matR;

				m_vDirection = m_vOrigin - GetPosition();
				D3DXVec3Normalize(&m_vDirection, &m_vDirection);

				D3DXMatrixLookAtLH(&matR,
					&D3DXVECTOR3(0, 0, 0),
					&-D3DXVECTOR3(m_vDirection.x, m_vDirection.y, m_vDirection.z),
					&D3DXVECTOR3(0, 1, 0));

				D3DXMatrixTranspose(&matR, &matR);

				m_matWorld = matR * Move();
				m_matWorld = m_matLocal * m_matWorld;
			}
		}
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
				SetEnemyState(ENEMY_IDLE);
				m_bIsAction = false;
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
				m_matWorld = Rotate() * Move();
			}
		}
	}
	break;
	case ENEMY_ATTACK:
	{
		//���� ���Ͱ� ���� ������ ����� �ʾҴٸ� �÷��̾ �i�ư���.
		float Distance = D3DXVec3Length(&(GetPosition() - g_player->GetPosition()));
		
		if (!m_bIsAction)
		{
			std::string Name;
			Name = m_sName + std::string("����");
			SOUNDMANAGER->play(Name, 1.f);

			m_bIsAction = true;
			m_pBody->SetAnimationIndex(ENEMY_ATTACK);
			m_fPeriod = m_pBody->GetAniTrackPeriod(ENEMY_ATTACK);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;

				if (abs(Distance) < 20.f)
				{
					SetEnemyState(ENEMY_ATTACK);
				}

				else if (abs(Distance) > 50.f)
				{
					SetEnemyState(ENEMY_CHASE);
				}
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}
	}
		break;
	case ENEMY_SKILL1:
		break;
	case ENEMY_SKILL2:
		break;
	case ENEMY_CHASE:
	{
		//���� ���Ͱ� ���� ������ ����� �ʾҴٸ� �÷��̾ �i�ư���.
		float Distance = D3DXVec3Length(&(GetPosition() - g_player->GetPosition()));

		if (!m_bIsAction)
		{
			m_bIsAction = true;
			m_pBody->SetAnimationIndex(ENEMY_RUN);
			int a = 0;
		}

		if (m_bIsAction)
		{
			float Length = D3DXVec3Length(&(GetPosition() - m_vOrigin));
			
			//�÷��̾�� �־����ٸ� ��� ���� ��
			D3DXMATRIX matR, matTemp;

			m_vDirection = g_player->GetPosition() - GetPosition();
			D3DXVec3Normalize(&m_vDirection, &m_vDirection);

			D3DXMatrixLookAtLH(&matR,
				&D3DXVECTOR3(0, 0, 0),
				&-D3DXVECTOR3(m_vDirection.x, m_vDirection.y, m_vDirection.z),
				&D3DXVECTOR3(0, 1, 0));


			D3DXMatrixTranspose(&matR, &matR);
			/*D3DXMatrixRotationY(&matTemp, -D3DX_PI / 2.f);

			matR *= matTemp;*/

			m_matWorld = matR * Move();
			m_matWorld = m_matLocal * m_matWorld;
			
			//���� ������ �Ѿ�ų�, �÷��̾�� ����� �Ÿ��� �������ٸ�
			if (abs(Length) > 600.f || abs(Distance) > 300.f)
			{
				SetEnemyState(ENEMY_BACKPOSITION);
			}
		}
	}
	break;
	}
}

D3DXMATRIXA16 cEnemy::Move()
{
	D3DXMATRIXA16 matT;

	SetPosition(GetPosition() + m_vDirection * m_fSpeed);

	D3DXMatrixTranslation(&matT, GetPosition().x, GetPosition().y, GetPosition().z);

	return matT;
}

void cEnemy::OnCollisionStay(cCollisionObject* rhs)
{
	//if (rhs->GetCollisionType() == CollisionType::ePlayer && !this->GetCollision())
	//{
	//	if (GetEnemyState() == ENEMY_ATTACK)
	//	{
	//		Log("�浹�Ͽ���");
	//		this->SetCollision(true);
	//		rhs->SetCurrHp(rhs->GetCurrHp() - 100);
	//		int a = 0;
	//	}
	//}
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

