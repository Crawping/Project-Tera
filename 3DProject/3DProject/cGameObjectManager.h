/*
cGameObjectManager��
��ϵ� ������Ʈ��

Update
Render
Delete�� �ڵ����� ȣ�����ش�.

��� �����

cGameObjectManager::AddObject( new SomeObject( ... ));

AddObject�� �߰��� ��ü�� �����͸� ��ȯ�ؼ�
�� ��ü �����͸� ���� ����ϰ� �ʹٸ� ���� �����ؼ� ���� �ȴ�.

( ������ ��� �ǵ��� �ϴ� �̷���. )

*/

#pragma once
#include "Singleton.h"
#include "cGameObject.h"

class cGameObject;
class cGameObjectManager : 
	public TSingleton<cGameObjectManager>
{
	using ObjectMap = std::unordered_map<std::string, cGameObject*>;
	
public:
	using iterator = ObjectMap::iterator;
	using const_iterator = ObjectMap::const_iterator;

public:
	void Update( );

	void AddObject( cGameObject* pObject );
	void EraseObject( const std::string& key );
	cGameObject* FindObject( const std::string& key );

	iterator begin( );
	iterator end( );

protected:
	cGameObjectManager();
	virtual ~cGameObjectManager();

private:
	ObjectMap m_objMap;
};

inline void cGameObjectManager::AddObject(
	cGameObject* obj )
{
	m_objMap.insert( std::make_pair( obj->GetName( ), obj ) );
}

inline cGameObject* cGameObjectManager::FindObject(
	const std::string& key )
{
	return m_objMap[key];
}

inline void cGameObjectManager::EraseObject(
	const std::string& key )
{
	m_objMap.erase( key );
}

inline cGameObjectManager::iterator cGameObjectManager::begin()
{
	return m_objMap.begin( );
}

inline cGameObjectManager::iterator cGameObjectManager::end()
{
	return m_objMap.end( );
}
