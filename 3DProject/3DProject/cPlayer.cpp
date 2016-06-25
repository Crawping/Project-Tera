#include "stdafx.h"
#include "cPlayer.h"
#include "cCollisionManager.h"
#include "cBoundingSphere.h"
#include "cBoundingBox.h"
#include "Console.h"
#include "cPlayerWeapon.h"
#include "cSkinnedMesh.h"
#include "cPlayerUI.h"
#include "cUIObject.h"
#include "cEquipWindow.h"
#include "cPlayerGauge.h"

cPlayer::cPlayer( ) :
	m_vDirection(D3DXVECTOR3(0, 0, 1))
	, m_fSpeed(2.f)
	, m_nMana(1000.f)
	, m_fPassTime(0.f)
	, m_fPeriod(0.f)
	, m_bAlive(true)
	, m_bIsAction(false)
	, m_bPushBehind(false)
	, m_pCombo(nullptr)
	, m_pUI(nullptr)
{
	SetPlayerState(PLAYER_BATTLEIDLE);
	this->SetPosition({ 0.f, 0.f, 0.f });

	m_pAmor[0] = new cSkinnedMesh("./CH/Player/", "Player_Body.X");

	m_pAmor[1] = new cSkinnedMesh("./CH/Player/", "Player_Body2.X");

	m_pAmor[2] = new cSkinnedMesh("./CH/Player/", "Player_Body3.X");

	m_vecSkinnedMesh.push_back(m_pAmor[0]);

	cSkinnedMesh* pHead = new cSkinnedMesh("./CH/Player/", "Player_Head.X");
	m_vecSkinnedMesh.push_back(pHead);

	cSkinnedMesh* pHair = new cSkinnedMesh("./CH/Player/", "Player_Hair.X");
	m_vecSkinnedMesh.push_back(pHair);

	cSkinnedMesh* pTail = new cSkinnedMesh("./CH/Player/", "Player_Tail.X");
	m_vecSkinnedMesh.push_back(pTail);

	cSkinnedMesh* pWeapon = new cSkinnedMesh("./CH/Player/", "Lance.X");
	m_vecSkinnedMesh.push_back(pWeapon);

	//�޺� Ŭ����
	m_pCombo = new cCommandCombo;

	//���� �浹ü
	this->AddCollider(new cBoundingSphere(D3DXVECTOR3(0, 0, 0), 9.f));
	D3DXMATRIXA16 matLocal;
	D3DXMatrixTranslation(&matLocal, 0, 15, 0);
	this->GetColliderRepo()[0]->SetLocal(&matLocal);
	D3DXVECTOR3 vPos(GetPosition().x + matLocal._41, GetPosition().y + matLocal._42, GetPosition().z + matLocal._43);
	this->GetColliderRepo()[0]->SetPosition(vPos);

	// ���� �浹ü
	m_playerWeapon = new cPlayerWeapon( 
		D3DXVECTOR3(GetPosition().x + matLocal._41, GetPosition().y + matLocal._42, GetPosition().z + matLocal._43),
		matLocal,
		this
	);

	SetAniTrack(PLAYER_BATTLEIDLE);
	this->SetObjectType(ObjectType::ePlayer);

	SOUNDMANAGER->addSound("�÷��̾����", "./Music/�÷��̾�(Attack).ogg", false, false);

	m_pUI = new cPlayerUI;
}

cPlayer::~cPlayer( )
{
	for (size_t i = 0; i < _countof(m_pAmor); i++)
	{
		SAFE_DELETE(m_pAmor[i]);
	}

	//�ƸӴ� �̹� ����
	for (size_t i = 1; i < m_vecSkinnedMesh.size(); i++)
	{
		SAFE_DELETE(m_vecSkinnedMesh[i]);
	}

	SAFE_DELETE(m_pUI);
	SAFE_DELETE(m_pCombo);
	SAFE_DELETE(m_playerWeapon);
}

void cPlayer::Update( )
{
	__super::Update( );

	SetUpdateState();
	
	KeyControl();
	SetFSMState();

	m_pCombo->Update();
	m_playerWeapon->Update( );

	if (this->GetCurrHp() <= 0 && 
		GetPlayerState() != PLAYER_DEATH)
	{
		m_bIsAction = false;
		SetPlayerState(PLAYER_DEATH);
	}

	m_pUI->Update();

	ChangeAmor();
}

void cPlayer::Render( )
{
	__super::Render( );
	m_playerWeapon->Render( );

	m_pUI->Render();

	SetRenderState();
}

void cPlayer::KeyControl()
{
	D3DXMATRIXA16 matR, matT;

	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		this->SetAngle(this->GetAngle() - D3DXVECTOR3(0, 0.1f, 0));
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		this->SetAngle(this->GetAngle() + D3DXVECTOR3(0, 0.1f, 0));
	}

	m_vDirection = D3DXVECTOR3( 1, 0, 0 );
	D3DXMatrixRotationY( &matR, this->GetAngle( ).y );
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);

	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		//�޸��⺸�� �켱������ ���� �ൿ�� �ϰ� ���� ��� �� �ൿ�� �޽��� ���
		if (GetPlayerState() == PLAYER_BATTLEIDLE)
		{
			m_bIsAction = false;
		}

		//�켱������ ���� �ൿ�� �� �ൿ���� �Ұ�
		if (GetPlayerState() == PLAYER_TUMBLING || GetPlayerState() == PLAYER_SKILL1 || GetPlayerState() == PLAYER_SKILL2 ||
			GetPlayerState() == PLAYER_SKILL3 || GetPlayerState() == PLAYER_SKILL4 || GetPlayerState() == PLAYER_COMBO1 ||
			GetPlayerState() == PLAYER_COMBO2 || GetPlayerState() == PLAYER_COMBO3 || GetPlayerState() == PLAYER_COMBO4)
		{
			m_bIsAction = true;
		}

		else 
		{
			SetPlayerState(PLAYER_RUN);
			SetPosition(GetPosition() + m_vDirection * m_fSpeed);
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_UP))
	{
		SetPlayerState(PLAYER_BATTLEIDLE);
		SetAniTrack(PLAYER_BATTLEIDLE);
	}

	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		//�޸��⺸�� �켱������ ���� �ൿ�� �ϰ� ���� ��� �� �ൿ�� �޽��� ���
		if (GetPlayerState() == PLAYER_BATTLEIDLE)
		{
			m_bIsAction = false;
		}

		//�켱������ ���� �ൿ�� �� �ൿ���� �Ұ�
		if (GetPlayerState() == PLAYER_TUMBLING || GetPlayerState() == PLAYER_SKILL1 || GetPlayerState() == PLAYER_SKILL2 ||
			GetPlayerState() == PLAYER_SKILL3 || GetPlayerState() == PLAYER_SKILL4 || GetPlayerState() == PLAYER_COMBO1 || 
			GetPlayerState() == PLAYER_COMBO2 || GetPlayerState() == PLAYER_COMBO3 || GetPlayerState() == PLAYER_COMBO4)
		{
			m_bIsAction = true;
		}

		else 
		{
			SetPlayerState(PLAYER_RUN);
			SetPosition(GetPosition() + m_vDirection * m_fSpeed);
		}

		//����Ű �ڸ� ������ �� �ѹ��� �ڸ� �� ��
		if (!m_bPushBehind)
		{
			this->GetAngle( ).y = this->GetAngle( ).y - D3DX_PI;
			m_bPushBehind = true;
			int a = 0;
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
	{
		SetPlayerState(PLAYER_BATTLEIDLE);

		if (m_bPushBehind)
		{
			m_bPushBehind = false;
		}
	}

	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		if (m_bIsAction)
		{
			if (GetPlayerState() == PLAYER_RUN || GetPlayerState() == PLAYER_BATTLEIDLE)
				m_bIsAction = false;
		}

		if (GetPlayerState() != PLAYER_COMBO1 || GetPlayerState() != PLAYER_COMBO2 ||
			GetPlayerState() != PLAYER_COMBO3 || GetPlayerState() != PLAYER_COMBO4)
		{
			SetPlayerState(PLAYER_COMBO1);
		}

		//��Ÿ�� �ѹ��� ���� �ʾҴٸ�?
		if (m_pCombo->GetCommand().size() == 0)
		{
			if (!SOUNDMANAGER->isPlaySound("�÷��̾����"))
				SOUNDMANAGER->play("�÷��̾����", 1.f);
			m_pCombo->Input(m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_COMBO1));
		}

		else if (m_pCombo->GetCommand().size() == 1)
		{
			if (!SOUNDMANAGER->isPlaySound("�÷��̾����"))
				SOUNDMANAGER->play("�÷��̾����", 1.f);
			m_pCombo->Input(m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_COMBO2));
		}

		else if (m_pCombo->GetCommand().size() == 2)
		{
			if (!SOUNDMANAGER->isPlaySound("�÷��̾����"))
				SOUNDMANAGER->play("�÷��̾����", 1.f);
			m_pCombo->Input(m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_COMBO3));
		}

		else if (m_pCombo->GetCommand().size() == 3)
		{
			if (!SOUNDMANAGER->isPlaySound("�÷��̾����"))
				SOUNDMANAGER->play("�÷��̾����", 1.f);
			m_pCombo->Input(m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_COMBO4));
		}
	}

	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		//�Һ��� ��� �ൿ�� ���ؼ� �켱������ ���� ����.
		if (m_bIsAction)
		{
			m_bIsAction = false;
		}

		//��Ÿ �޺� ���
		m_pCombo->Destroy();
		SetPlayerState(PLAYER_TUMBLING);
	}

	//��ų �ൿ ���϶� �Һ� �̿ܿ��� ĵ���� �� ����.
	if (KEYMANAGER->isOnceKeyDown('A'))
	{
		if (GetPlayerState() != PLAYER_TUMBLING && GetPlayerState() != PLAYER_SKILL2 && 
			GetPlayerState() != PLAYER_SKILL3 && GetPlayerState() != PLAYER_SKILL4)
		{
			m_bIsAction = false;
		}

		if (GetPlayerState() == PLAYER_SKILL1)
		{
			m_bIsAction = true;
		}

		//��Ÿ �޺� ���
		m_pCombo->Destroy();
		SetPlayerState(PLAYER_SKILL1);
	}

	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		if (GetPlayerState() != PLAYER_TUMBLING && GetPlayerState() != PLAYER_SKILL1 && 
			GetPlayerState() != PLAYER_SKILL3 && GetPlayerState() != PLAYER_SKILL4)
		{
			m_bIsAction = false;
		}

		//���� ���϶� ��� �Ұ�
		if (GetPlayerState() == PLAYER_SKILL2)
		{
			m_bIsAction = true;
		}

		//��Ÿ �޺� ���
		m_pCombo->Destroy();
		SetPlayerState(PLAYER_SKILL2);
	}

	if (KEYMANAGER->isOnceKeyDown('D'))
	{
		if (GetPlayerState() != PLAYER_TUMBLING && 
			GetPlayerState() != PLAYER_SKILL1 &&
			GetPlayerState() != PLAYER_SKILL2 && 
			GetPlayerState() != PLAYER_SKILL4)
		{
			m_bIsAction = false;
		}

		if (GetPlayerState() == PLAYER_SKILL3)
		{
			m_bIsAction = true;
		}

		//��Ÿ �޺� ���
		m_pCombo->Destroy();
		SetPlayerState(PLAYER_SKILL3);
	}

	if (KEYMANAGER->isOnceKeyDown('F'))
	{
		if (GetPlayerState() != PLAYER_TUMBLING && GetPlayerState() != PLAYER_SKILL1 &&
			GetPlayerState() != PLAYER_SKILL2 && GetPlayerState() != PLAYER_SKILL3)
		{
			m_bIsAction = false;
		}

		if (GetPlayerState() == PLAYER_SKILL4)
		{
			m_bIsAction = true;
		}

		//��Ÿ �޺� ���
		m_pCombo->Destroy();
		SetPlayerState(PLAYER_SKILL4);
	}

	D3DXMatrixTranslation(&matT, GetPosition().x, GetPosition().y - 4.f, GetPosition().z);
	m_matWorld = matR * matT;
}

void cPlayer::OnCollisionEnter(
	int colliderIndex,
	cCollisionObject* rhs )
{
	switch ( colliderIndex )
	{
	case 0:
		{
			this->Move( -m_vDirection*m_fSpeed*2 );
		}
		break;
	}
}

void cPlayer::OnCollisionStay( 
	int colliderIndex, 
	cCollisionObject* rhs )
{
	switch ( colliderIndex )
	{
	case 0:
		{
			this->Move( -m_vDirection*m_fSpeed );
		}
		break;
	}


}

void cPlayer::OnCollisionEnd( 
	int colliderIndex, 
	cCollisionObject* rhs )
{
	Log( "End\n" );
}

void cPlayer::SetFSMState()
{
	switch (GetPlayerState())
	{
	case PLAYER_BATTLEIDLE:
	{
		if (!m_bIsAction)
		{
			SetAniTrack(PLAYER_BATTLEIDLE);
			m_bIsAction = true;
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_BATTLEIDLE) ;
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}
	}
		break;

	case PLAYER_RUN:
	{
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_RUN);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_RUN);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime();
			}
		}
	}
		break;

	case PLAYER_COMBO1:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_COMBO1);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_COMBO1);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				if (m_pCombo->GetCommand().size() > 1)
					SetPlayerState(PLAYER_COMBO2);

				else if (m_pCombo->GetCommand().size() <= 1)
					SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

	case PLAYER_COMBO2:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_COMBO2);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_COMBO2);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;

				if (m_pCombo->GetCommand().size() > 2)
					SetPlayerState(PLAYER_COMBO3);

				else if (m_pCombo->GetCommand().size() <= 2)
					SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

	case PLAYER_COMBO3:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_COMBO3);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_COMBO3);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				if (m_pCombo->GetCommand().size() > 3)
					SetPlayerState(PLAYER_COMBO4);

				if (m_pCombo->GetCommand().size() <= 3)
					SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

	case PLAYER_COMBO4:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_COMBO4);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_COMBO4);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;
	
	case PLAYER_TUMBLING:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_TUMBLING);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_TUMBLING);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime < m_fPeriod - 0.75f)
			{
				SetPosition(GetPosition() + m_vDirection * (m_fPeriod - 1));
			}

			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

		//���ϰ� ����ġ��
	case PLAYER_SKILL1:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_SKILL1);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_SKILL1);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime < m_fPeriod - 1.0f)
			{
				SetPosition(GetPosition() + m_vDirection * (m_fPeriod - 1.f));
			}

			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

		//������
	case PLAYER_SKILL2:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_SKILL2);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_SKILL2);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

		//���ϰ� ���������
	case PLAYER_SKILL3:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_SKILL3);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_SKILL3);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

		//ȸ���� ġ��
	case PLAYER_SKILL4:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_SKILL4);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_SKILL4);
		}

		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_BATTLEIDLE);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

	case PLAYER_DEATH:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_DEATH);
			m_fPassTime = 0.f;
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_DEATH);
		}
		else if (m_bIsAction)
		{
			if (m_fPassTime > m_fPeriod)
			{
				m_bIsAction = false;
				m_fPassTime = 0.f;
				m_fPeriod = 0.f;
				SetPlayerState(PLAYER_DEATHWAIT);
				this->SetDead(true);
			}

			else if (m_fPassTime < m_fPeriod)
			{
				SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y - 0.08f, GetPosition().z));
				m_fPassTime += g_pTimeManager->GetDeltaTime() ;
			}
		}
		break;

	case PLAYER_DEATHWAIT:
		if (!m_bIsAction)
		{
			m_bIsAction = true;
			SetAniTrack(PLAYER_DEATHWAIT);
			m_fPeriod = m_vecSkinnedMesh[0]->GetAniTrackPeriod(PLAYER_DEATHWAIT);
		}
		break;
	}
}

void cPlayer::SetUpdateState( )
{
	//�÷��̾��� ���� ��ġ�� �־���
	//����
	D3DXMATRIXA16 matT, matR, matWorld;
	D3DXMatrixTranslation(&matT, this->GetPosition().x, this->GetPosition().y, this->GetPosition().z);
	D3DXMatrixRotationY(&matR, this->GetAngle().y);
	matWorld = matR * matT;
	m_vecSkinnedMesh[0]->SetWorld(&matWorld);

	//�Ӹ�
	m_vecSkinnedMesh[1]->SetLocal(&(m_vecSkinnedMesh[0]->GetNeckTM()));

	//���
	m_vecSkinnedMesh[2]->SetLocal(&m_vecSkinnedMesh[0]->GetHairTM());

	//����
	m_vecSkinnedMesh[3]->SetLocal(&m_vecSkinnedMesh[0]->GetTailTM());

	//�÷��̾�
	this->GetColliderRepo()[0]->SetWorld(&m_matWorld);

	//����
	m_vecSkinnedMesh[4]->SetLocal(&m_vecSkinnedMesh[0]->GetWeaponTM());

	//���� ��ġ �����Ϸ��� ������� �ǵ� ����
	D3DXMATRIXA16 matLocal;
	D3DXMatrixTranslation(&matLocal, 1, 45, 0);
	matLocal *= (D3DXMATRIXA16)m_vecSkinnedMesh[0]->GetWeaponTM();
	m_playerWeapon->GetColliderRepo()[0]->SetWorld(&matLocal);

	//�÷��̾� ü�� ����
	m_pUI->GetGauge()->SetEnergy(this->GetCurrHp());
	m_pUI->GetGauge()->SetMana(m_nMana);

}

void cPlayer::SetRenderState()
{
	for (size_t i = 0; i < m_vecSkinnedMesh.size(); i++)
	{
		m_vecSkinnedMesh[i]->UpdateAndRender();
	}
}

void cPlayer::SetAniTrack(int nIndex)
{
	for (size_t i = 0; i < m_vecSkinnedMesh.size(); i++)
	{
		m_vecSkinnedMesh[i]->SetAnimationIndex(nIndex);
	}
}

void cPlayer::ChangeAmor()
{
	if (m_pUI->GetEquipState()->GetRoot()->GetChild()[2]->GetButtonTag() == E_TAG_ITEM1)
	{
		m_vecSkinnedMesh[0] = m_pAmor[0];
	}

	else if (m_pUI->GetEquipState()->GetRoot()->GetChild()[2]->GetButtonTag() == E_TAG_ITEM2)
	{
		m_vecSkinnedMesh[0] = m_pAmor[1];
	}

	else if (m_pUI->GetEquipState()->GetRoot()->GetChild()[2]->GetButtonTag() == E_TAG_ITEM3)
	{
		m_vecSkinnedMesh[0] = m_pAmor[2];
	}
}