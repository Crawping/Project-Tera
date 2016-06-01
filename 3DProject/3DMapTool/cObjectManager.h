#pragma once
#include "Singleton.h"

#define g_pObjectManager cObjectManager::Get()

class cObject;
class cObjectManager :
	public TSingleton<cObjectManager>
{
public:
	void AddObject(cObject* pObject)
	{
		m_setObject.insert(pObject);
	}
	void RemoveObject(cObject* pObject)
	{
		m_setObject.erase(pObject);
	}
	void Destroy()
	{
		assert(m_setObject.empty() && "������ cObject�� ��ӹ��� ��ü �� �������� ���� ��ü�� �ֽ��ϴ�.");
	}

protected:
	cObjectManager( ) {}
	virtual ~cObjectManager(  ) {}

private:
	std::set<cObject*> m_setObject;
};

