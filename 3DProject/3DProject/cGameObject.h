#pragma once

/*
*/

enum eEnemyState
{
	ENEMY_IDLE = 0,				//���� ���ֱ�
	ENEMY_RUN = 1,				//���� �̵�
	ENEMY_DEATHWAIT = 2,		//���� ����
	ENEMY_DEATH = 3,			//���� �ױ� ����
	ENEMY_ATTACK = 4,			//���� ����
	ENEMY_SKILL1 = 5,			//���� ��ų1
	ENEMY_SKILL2 = 6,			//���� ��ų2
	ENEMY_BACKPOSITION = 7,		//���� �ǵ��ư���
	ENEMY_CHASE = 8,			//�÷��̾� �i�ư���
	ENEMY_NOTHING
};

enum ePlayerState
{
	PLAYER_BATTLEIDLE = 5,
	PLAYER_RUN = 46,
	PLAYER_TUMBLING = 6,
	PLAYER_SKILL1 = 26,			//���ϰ� �������
	PLAYER_SKILL2 = 0,			//������
	PLAYER_SKILL3 = 27,			//���ϰ� ������ ���
	PLAYER_SKILL4 = 28,			//ȸ����ġ��
	PLAYER_COMBO1 = 2,			//�÷��̾� �޺�1
	PLAYER_COMBO2 = 4,			//�÷��̾� �޺�2
	PLAYER_COMBO3 = 8,			//�÷��̾� �޺�3
	PLAYER_COMBO4 = 17,			//�÷��̾� �޺�4
	PLAYER_DEATH = 25,			//�÷��̾� ����
	PLAYER_DEATHWAIT = 24,		//�÷��̾� ��������
};

enum class CollisionType
{
	eUnknown,
	ePlayer,
	eMonster,
	eBuilding,
	eNpc,
};

class cGameObject
{
public:
	cGameObject( const std::string& objName );
	virtual ~cGameObject( );

	virtual void Render( );
	virtual void Update( );

public:
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
	
	//Hp
	int GetMaxHp() { return m_nMaxHp; }

	void SetCurrHp(int hp) { m_nCurrHp = hp; }
	int GetCurrHp() { return m_nCurrHp; }

	void SetCollisionType(CollisionType c);
	CollisionType GetCollisionType();

	//State
	void SetPlayerState(ePlayerState p);
	ePlayerState GetPlayerState();

	void SetEnemyState(eEnemyState e);
	eEnemyState GetEnemyState();

	/*
		State & Component
	*/
	void SetActive( bool isActive );
	bool IsActive( ) const;
	
	const std::string& GetName( ) const;
	D3DXMATRIXA16& GetWorld( );

protected:
	const D3DXVECTOR3& GetPrevPos( ) const;

private:
	void UpdateWorld( );

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_prevPos;
	D3DXVECTOR3 m_angle;
	D3DXVECTOR3 m_scale;
	D3DXMATRIXA16 m_matWorld;
	std::string m_objName;
	bool m_isActive;
	CollisionType	m_sType;
	int m_nMaxHp;
	int m_nCurrHp;
	
	ePlayerState m_sPState;
	eEnemyState m_sEState;
};

inline void cGameObject::SetPosition( const D3DXVECTOR3& pos )
{
	m_prevPos = m_pos;
	m_pos = pos;
	m_matWorld._41 = pos.x;
	m_matWorld._42 = pos.y;
	m_matWorld._43 = pos.z;
}

inline void cGameObject::Move( const D3DXVECTOR3& pos )
{
	m_prevPos = m_pos;
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

inline void cGameObject::SetCollisionType(CollisionType c)
{
	m_sType = c;
}

inline CollisionType cGameObject::GetCollisionType()
{
	return m_sType;
}

inline void cGameObject::SetPlayerState(ePlayerState p)
{
	m_sPState = p;
}

inline ePlayerState cGameObject::GetPlayerState()
{
	return m_sPState;
}

inline void cGameObject::SetEnemyState(eEnemyState e)
{
	m_sEState = e;
}

inline eEnemyState cGameObject::GetEnemyState()
{
	return m_sEState;
}

inline void cGameObject::SetActive( bool isActive )
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

inline D3DXMATRIXA16& cGameObject::GetWorld( )
{
	return m_matWorld;
}

inline const D3DXVECTOR3& cGameObject::GetPrevPos( ) const
{
	return m_prevPos;
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

