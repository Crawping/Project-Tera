#include "stdafx.h"
#include "cCommandCombo.h"


cCommandCombo::cCommandCombo()
	: m_fCommandClear(13.f)
	, m_fCommandTime(0.f)
{
}


cCommandCombo::~cCommandCombo()
{
	m_vecCommand.clear();

}

void cCommandCombo::Update()
{
	if (m_vecCommand.size() > 0)
		m_fCommandTime += g_pTimeManager->GetDeltaTime();

	if (m_vecCommand.size() == 0)
		m_fCommandTime = 0.f;

	if (m_fCommandTime > m_fCommandClear)
	{
		Destroy();
	}
}

void cCommandCombo::Input(float InputKey)
{
	m_vecCommand.push_back(InputKey);
}

void cCommandCombo::Destroy()
{
	//���� �ð��� ������ �׿��ִ� Ű�Է� ���͵��� ����
	m_vecCommand.clear();

	m_fCommandTime = 0.f;
}