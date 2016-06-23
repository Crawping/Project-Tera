#include "stdafx.h"
#include "cArgoniteKallashGuardLeader.h"

cArgoniteKallashGuardLeader::cArgoniteKallashGuardLeader()
{
	m_sName = "ArgoniteKallashGuardLeader";

	m_pBody = new cNpcSkinnedMesh("./CH/ArgoniteKallashGuardLeader/", "ArgoniteKallashGuardLeader.X");

	m_pBody->SetSpecTex(g_pTextureManager->GetTexture("./CH/ArgoniteKallashGuardLeader/ArgoniteKallashGuardLeader_spec.tga"));

	D3DXMatrixRotationY(&m_matLocal, D3DX_PI / 2.f);

	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, 0, 30, 0);
	this->AddCollider(new cBoundingSphere(D3DXVECTOR3(0, 0, 0), 30.f));
	this->GetColliderRepo()[0]->SetLocal(&matT);

	m_fRange = 30.f;
}


cArgoniteKallashGuardLeader::~cArgoniteKallashGuardLeader()
{
	SAFE_DELETE(m_pBody);
}

void cArgoniteKallashGuardLeader::Update()
{
	__super::Update();

	if (GetEnemyState() == ENEMY_DEATH)
	{
		SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y - 0.2f, GetPosition().z));
		m_fDeathTime = m_pBody->GetAniTrackPeriod(ENEMY_DEATH) - 0.4f;
		//�̵�����ŭ �����Ʈ���� ����
		D3DXMatrixTranslation(&m_matWorld, GetPosition().x, GetPosition().y, GetPosition().z);
	}
}

void cArgoniteKallashGuardLeader::Render()
{
	__super::Render();
	
}