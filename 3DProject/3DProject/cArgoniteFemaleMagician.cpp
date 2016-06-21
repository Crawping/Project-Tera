#include "stdafx.h"
#include "cArgoniteFemaleMagician.h"

cArgoniteFemaleMagician::cArgoniteFemaleMagician()
{
	m_sName = "ArgoniteFemaleMagician";

	m_pBody = new cNpcSkinnedMesh("./CH/ArgoniteFemaleMagician/", "ArgoniteFemaleMagician.X");

	D3DXMATRIX matR;
	D3DXMatrixRotationY(&m_matLocal, D3DX_PI / 2.f);

	//������ ���� ������ ������ �����ߴ�
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, 0, 50, 0);
	this->AddCollider(new cBoundingSphere(D3DXVECTOR3(0, 0, 0), 30.f));

	//���� �ڵ�� �ٿ��üũ �� �� ��ġ ����ִ� ��
	this->GetColliderRepo()[0]->SetLocal(&matT);
	D3DXVECTOR3 vPos(GetPosition().x + matT._41, GetPosition().y + matT._42, GetPosition().z + matT._43);
	this->GetColliderRepo()[0]->SetPosition(vPos);
}


cArgoniteFemaleMagician::~cArgoniteFemaleMagician()
{
	SAFE_DELETE(m_pBody);
}

void cArgoniteFemaleMagician::Update()
{
	__super::Update();

	if (GetEnemyState() == ENEMY_DEATH)
	{
		SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y - 0.23f, GetPosition().z));
		m_fDeathTime = m_pBody->GetAniTrackPeriod(ENEMY_DEATH) - 0.4f;
		//�̵�����ŭ �����Ʈ���� ����
		D3DXMatrixTranslation(&m_matWorld, GetPosition().x, GetPosition().y, GetPosition().z); 
	}
}

void cArgoniteFemaleMagician::Render()
{
	__super::Render();
}