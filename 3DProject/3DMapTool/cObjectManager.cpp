#include "stdafx.h"
#include "cObjectManager.h"


cObjectManager::cObjectManager()
{
}


cObjectManager::~cObjectManager()
{
	assert( m_setObject.empty( ) &&
		"������ cObject�� ��ӹ��� ��ü �� �������� ���� ��ü�� �ֽ��ϴ�." );
}
