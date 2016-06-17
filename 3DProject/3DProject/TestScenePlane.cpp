#include "stdafx.h"
#include "TestScenePlane.h"

#include "cShaderManager.h"


// ���� ��ġ
D3DXVECTOR4	gWorldLightPosition( 500.0f, 500.0f, -500.0f, 1.0f );
// ���� ����
D3DXVECTOR4	gLightColor( 1.0f, 1.0f, 1.0f, 1.0f );



TestScenePlane::TestScenePlane( const char* objName ) :
	m_diffuseMap( nullptr ),
	m_specularMap( nullptr ),
	m_normalMap( nullptr ),
	m_normalMappingShader( nullptr ),
	m_owner( cGameObjectManager::Get( )->FindObject( objName ))
{
	assert( m_owner );

	m_owner->SetActive( false );
	m_owner->Scale({ 5.f, 5.f, 5.f });
	
	m_normalMappingShader = cShaderManager::Get( )->
		GetShader( "Shader/NormalMapping.fx" );
	
	m_diffuseMap = cTextureManager::Get( )->GetTexture(
		"C:/Users/ggomdyu/Desktop/obj/Free_Stonewall/StonePlaneDiffuse.jpg" );
	m_specularMap = cTextureManager::Get( )->GetTexture(
		"C:/Users/ggomdyu/Desktop/obj/Free_Stonewall/StonePlaneSpecular.jpg" );
	m_normalMap = cTextureManager::Get( )->GetTexture(
		"C:/Users/ggomdyu/Desktop/obj/Free_Stonewall/StonePlaneNormal.jpg" );
}

TestScenePlane::~TestScenePlane( )
{
}

void TestScenePlane::Render( )
{
	// �� ����� �����.
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 vEyePt = cCamera::Get( )->GetEye( );
	D3DXVECTOR3 vLookatPt = cCamera::Get( )->GetLookAt( );
	D3DXVECTOR3 vUpVec = cCamera::Get( )->GetUp( );
 	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );

	// ����/��/��������� �̸� ���Ѵ�.
	D3DXMATRIXA16 matWorldView;
	D3DXMATRIXA16 matWorldViewProjection;
	D3DXMatrixMultiply(&matWorldView, &m_owner->GetWorld( ), &matView);
	D3DXMatrixMultiply(&matWorldViewProjection, &matWorldView, &cCamera::Get( )->GetProjection( ));

	// ���̴� ������������ ����
	m_normalMappingShader->SetMatrix("gWorldMatrix", &m_owner->GetWorld( ));
	m_normalMappingShader->SetMatrix("gWorldViewProjectionMatrix",  &matWorldViewProjection);

	m_normalMappingShader->SetVector("gWorldLightPosition", &gWorldLightPosition);
	m_normalMappingShader->SetVector("gWorldCameraPosition", &D3DXVECTOR4( cCamera::Get( )->GetEye( ), 1.f ));

	m_normalMappingShader->SetVector("gLightColor", &gLightColor);
	m_normalMappingShader->SetTexture("DiffuseMap_Tex", m_diffuseMap );
	m_normalMappingShader->SetTexture("SpecularMap_Tex", m_specularMap);
	m_normalMappingShader->SetTexture("NormalMap_Tex", m_normalMap);

	// ���̴��� �����Ѵ�.
	UINT numPasses = 0;
	m_normalMappingShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i )
		{
			m_normalMappingShader->BeginPass(i);
			{
				// ��ü�� �׸���.
				g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
				m_owner->Render( );
				g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

			}
			m_normalMappingShader->EndPass();
		}
	}
	m_normalMappingShader->End();
}

void TestScenePlane::Update( )
{
	if ( m_owner )
	{
		m_owner->Update( );
	}
}
