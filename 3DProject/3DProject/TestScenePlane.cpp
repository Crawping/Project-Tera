#include "stdafx.h"
#include "TestScenePlane.h"

#include "cShaderManager.h"


// ���� ��ġ
D3DXVECTOR4	gWorldLightPosition( 500.0f, 500.0f, -500.0f, 1.0f );
// ���� ����
D3DXVECTOR4	gLightColor( 1.0f, 1.0f, 1.0f, 1.0f );



TestScenePlane::TestScenePlane( const char* objName ) :
	gpStoneDM( nullptr ),
	gpStoneSM( nullptr ),
	gpStoneNM( nullptr ),
	gpNormalMappingShader( nullptr ),
	m_owner( cGameObjectManager::Get( )->FindObject( objName ))
{
	assert( m_owner );

	m_owner->SetActive( false );
	m_owner->Scale({ 5.f, 5.f, 5.f });
	
	gpNormalMappingShader = cShaderManager::Get( )->
		GetShader( "Shader/NormalMapping.fx" );
	
	gpStoneDM = cTextureManager::Get( )->GetTexture(
		"C:/Users/ggomdyu/Desktop/obj/Free_Stonewall/StonePlaneDiffuse.jpg" );
	gpStoneSM = cTextureManager::Get( )->GetTexture(
		"C:/Users/ggomdyu/Desktop/obj/Free_Stonewall/StonePlaneSpecular.jpg" );
	gpStoneNM = cTextureManager::Get( )->GetTexture(
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
	gpNormalMappingShader->SetMatrix("gWorldMatrix", &m_owner->GetWorld( ));
	gpNormalMappingShader->SetMatrix("gWorldViewProjectionMatrix",  &matWorldViewProjection);

	gpNormalMappingShader->SetVector("gWorldLightPosition", &gWorldLightPosition);
	gpNormalMappingShader->SetVector("gWorldCameraPosition", &D3DXVECTOR4( cCamera::Get( )->GetEye( ), 1.f ));

	gpNormalMappingShader->SetVector("gLightColor", &gLightColor);
	//gpNormalMappingShader->SetTexture("DiffuseMap_Tex", gpStoneDM );
	gpNormalMappingShader->SetTexture("SpecularMap_Tex", gpStoneSM);
	gpNormalMappingShader->SetTexture("NormalMap_Tex", gpStoneNM);

	// ���̴��� �����Ѵ�.
	UINT numPasses = 0;
	gpNormalMappingShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i )
		{
			gpNormalMappingShader->BeginPass(i);
			{
				// ��ü�� �׸���.
				g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
				m_owner->Render( );
				g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

			}
			gpNormalMappingShader->EndPass();
		}
	}
	gpNormalMappingShader->End();
}

void TestScenePlane::Update( )
{
	if ( m_owner )
	{
		m_owner->Update( );
	}
}
