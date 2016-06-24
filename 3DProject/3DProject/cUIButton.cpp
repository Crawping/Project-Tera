#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
	: m_pDelegate(nullptr)
	, m_eType(E_TYPE_BUTTONSTAY)
{
}

cUIButton::~cUIButton()
{
}

void cUIButton::SetButtonTexture(std::string sFullPath)
{
	D3DXIMAGE_INFO stImageInfo;

	//�̹����� ���� ������ �����Ѵ�.
	m_pTexture = g_pTextureManager->GetTexture(sFullPath, &stImageInfo);

	//�̹���ũ�⿡ ���߾� ���̿� ���̰� ����
	m_nWidth = stImageInfo.Width;
	m_nHeight = stImageInfo.Height;
}

void cUIButton::Update()
{
	//���콺 ��ġ�������� �����´�.
	POINT ptCurrMouse;
	GetCursorPos(&ptCurrMouse);
	ScreenToClient(g_hWnd, &ptCurrMouse);

	//��ġ�� ����
	cUIObject::Update();

	if (PtInRect(&m_Rc, ptCurrMouse) && m_eType == E_TYPE_BUTTONSTAY)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			if (m_pDelegate)
			{
				m_pDelegate->OnClick(this);
			}
		}

		if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
		{
			if (m_pDelegate)
			{
				m_pDelegate->OnChangeItem(this);
			}
		}
	}

	else if (PtInRect(&m_Rc, ptCurrMouse) && m_eType == E_TYPE_BUTTONMOVE)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			if (m_pDelegate)
			{
				m_pDelegate->OnStayClick(this);
			}
		}
	}
}

void cUIButton::Render(LPD3DXSPRITE pSprite)
{
	RECT rc;
	SetRect(&rc, 0, 0, m_nWidth, m_nHeight);

	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	pSprite->SetTransform(&m_matWorld);
	pSprite->Draw(m_pTexture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255));

	pSprite->End();

	cUIObject::Render(pSprite);
}