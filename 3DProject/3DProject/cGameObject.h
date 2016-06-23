#pragma once

enum class ObjectType
{
	eUnknown,
	ePlayer,
	ePlane,
	eMonster,
	eBuilding,
	eNpc,
};

class cGameObject
{
public:
	cGameObject( );
	virtual ~cGameObject( );

	virtual void Render( );
	virtual void Update( );

public:
	// Position
	virtual void SetPosition( const D3DXVECTOR3& pos );
	virtual void Move( const D3DXVECTOR3& pos );
	virtual D3DXVECTOR3& GetPosition();
	virtual const D3DXVECTOR3& GetPosition() const;

	// Roatation
	virtual void SetAngle( const D3DXVECTOR3& rot );
	virtual void Rotate( const D3DXVECTOR3& rot );
	virtual D3DXVECTOR3& GetAngle( );
	virtual const D3DXVECTOR3& GetAngle( ) const;
	
	// Scale
	virtual void SetScale( const D3DXVECTOR3& scale );
	virtual void Scale( const D3DXVECTOR3& scale );
	virtual const D3DXVECTOR3& GetScale( ) const;
	
	// Transform�� ���ÿ� ���� ����� �����ɴϴ�.
	const D3DXMATRIXA16& GetWorld( ) const;
	
	// ������Ʈ�� �ڵ� Update, Render ���θ� �����ϰų� �����ɴϴ�.
	// �� ������Ʈ�� cGameObjectManager�� AddObject���� �ʴ� ���
	// �� �Լ����� ������ �ʽ��ϴ�.
	void SetActive( bool isActive );
	bool IsActive( ) const;
	
	// �� ������Ʈ�� �̸��� �����ϰų� �����ɴϴ�.
	// �� �̸��� �� ������Ʈ���� �ߺ��Ǿ�� �ȵ˴ϴ�.
	void SetName( const std::string& name );
	const std::string& GetName( ) const;

	// ������Ʈ�� ������ �����մϴ�.
	void SetObjectType( ObjectType objectType );
	
	// ������Ʈ�� ������ �����ɴϴ�.
	ObjectType GetObjectType( ) const;


private:
	void UpdateWorld( );

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_angle;
	D3DXVECTOR3 m_scale;
	D3DXMATRIXA16 m_matWorld;
	std::string m_objName;
	bool m_isActive;
	ObjectType	m_objectType;
};

inline void cGameObject::SetPosition( const D3DXVECTOR3& pos )
{
	m_pos = pos;
	m_matWorld._41 = pos.x;
	m_matWorld._42 = pos.y;
	m_matWorld._43 = pos.z;
}

inline void cGameObject::Move( const D3DXVECTOR3& pos )
{
	m_pos += pos;
	m_matWorld._41 += pos.x;
	m_matWorld._42 += pos.y;
	m_matWorld._43 += pos.z;
}

inline void cGameObject::SetAngle( const D3DXVECTOR3& rot )
{
	m_angle = rot;
	this->UpdateWorld( );
}

inline void cGameObject::Rotate( const D3DXVECTOR3& rot )
{
	m_angle += rot;
	this->UpdateWorld( );
}

inline void cGameObject::SetScale( const D3DXVECTOR3& scale )
{
	m_scale = scale;
	this->UpdateWorld( );
}

inline void cGameObject::Scale( const D3DXVECTOR3& scale )
{
	m_scale += scale;
	this->UpdateWorld( );
}

inline void cGameObject::SetObjectType( 
	ObjectType objectType )
{
	m_objectType = objectType;
}

inline ObjectType cGameObject::GetObjectType( ) const
{
	return m_objectType;
}

inline void cGameObject::SetActive( 
	bool isActive )
{
	m_isActive = isActive;
}

inline D3DXVECTOR3& cGameObject::GetPosition()
{
	return m_pos;
}

inline const D3DXVECTOR3& cGameObject::GetPosition() const
{
	return m_pos;
}

inline D3DXVECTOR3& cGameObject::GetAngle( )
{
	return m_angle;
}

inline const D3DXVECTOR3& cGameObject::GetAngle( ) const
{
	return m_angle;
}

inline const D3DXVECTOR3& cGameObject::GetScale( ) const
{
	return m_scale;
}

inline const std::string& cGameObject::GetName( ) const
{
	return m_objName;
}

inline const D3DXMATRIXA16& cGameObject::GetWorld( ) const
{
	return m_matWorld;
}

inline bool cGameObject::IsActive( ) const
{
	return m_isActive;
}

inline void cGameObject::UpdateWorld( )
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling( &matScale, m_scale.x, m_scale.y, m_scale.z );
	
	D3DXMATRIXA16 matRot, matRotX, matRotY, matRotZ;
	D3DXMatrixRotationX( &matRotX, m_angle.x );
	D3DXMatrixRotationY( &matRotY, m_angle.y );
	D3DXMatrixRotationZ( &matRotZ, m_angle.z );
	matRot = matRotX * matRotY * matRotZ;

	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation( &matTrans, m_pos.x, m_pos.y, m_pos.z );

	m_matWorld = matScale * matRot * matTrans;
}

