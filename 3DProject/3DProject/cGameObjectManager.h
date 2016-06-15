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
	void Render( );

public:
	template <class _ObjectTy>
	_ObjectTy* AddObject( const std::string& name, _ObjectTy* obj );

	cGameObject* FindObject( const std::string& key );
	void EraseObject( const std::string& key );
	
	// ��� ��ϵ� ������Ʈ�� �ı��մϴ�.
	void ResetAllObject( );

	iterator begin( );
	iterator end( );

protected:
	cGameObjectManager();
	virtual ~cGameObjectManager();

private:
	ObjectMap m_objMap;
};

template<class _ObjectTy>
inline _ObjectTy* cGameObjectManager::AddObject( 
	const std::string& name, 
	_ObjectTy* obj )
{
	obj->SetName( name );
	m_objMap.insert( std::make_pair( name, obj ));

	return obj;
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