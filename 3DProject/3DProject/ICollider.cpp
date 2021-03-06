#include "stdafx.h"
#include "ICollider.h"


ICollider::ICollider( ) :
#ifdef _DEBUG
	m_isDebugRender( true )
#else 
	m_isDebugRender( false )
#endif
{
}

void ICollider::PreRender( )
{
	g_pD3DDevice->SetRenderState( D3DRENDERSTATETYPE::D3DRS_LIGHTING, FALSE );
	g_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	g_pD3DDevice->SetTexture( 0, nullptr );

}

void ICollider::PostRender( )
{
	g_pD3DDevice->SetRenderState( D3DRENDERSTATETYPE::D3DRS_LIGHTING, TRUE );
	g_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
}


