#include "stdafx.h"
#include "cLightObject.h"


cLightObject::cLightObject( ) :
	m_lightColor( 1.f, 1.f,1.f,1.f )
{
	this->SetPosition({ 500.0f, 500.0f, 500.0f });
}

cLightObject::~cLightObject( )
{
}

void cLightObject::Update( )
{
	__super::Update( );
}
