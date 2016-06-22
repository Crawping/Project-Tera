#pragma once
// ������ UI ���̽��� ��ӹ޾ƾ� ��. ���� �ȳ������Ƿ� �н�.

#include "c3DSprite.h"

class cUnitObject;
class cHPGaugeBar :
	public cGameObject
{
public:
	explicit cHPGaugeBar( 
		const char* hpGuagePath,
		const char* hpFramePath );
	virtual ~cHPGaugeBar( );

	virtual void Update( );
	virtual void Render( );

public:
	void SetOwner( cUnitObject* ); // ���� UI�ʿ� �־����
	cUnitObject* GetOwner( );

private:
	//D3DXVECTOR3 m_myPos; // Final present pos is <ownerPos + m_myPos>
	cUnitObject* m_owner;
	cBillboard3DSprite* m_hpGuage;
	cBillboard3DSprite* m_hpFrame;
	LPD3DXEFFECT m_hpShader;
	LPD3DXEFFECT m_texShader;
};

inline void cHPGaugeBar::SetOwner( 
	cUnitObject* owner )
{
	m_owner = owner;
}

inline cUnitObject* cHPGaugeBar::GetOwner( )
{
	return m_owner;
}