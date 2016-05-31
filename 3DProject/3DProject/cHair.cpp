#include "stdafx.h"
#include "cHair.h"
#include "cAllocateHierarchy.h"

cHair::cHair()
	: m_pAlloc(nullptr)
	, m_pFrameRoot(nullptr)
	, m_pMesh(nullptr)
	, m_pBuffer(nullptr)
	, m_ft(0.1f)
{
	D3DXMatrixIdentity(&m_matHairTM);
}


cHair::~cHair()
{
	D3DXFrameDestroy(m_pFrameRoot, m_pAlloc);
	SAFE_DELETE(m_pAlloc);
	SAFE_RELEASE(m_pMesh);
}

void cHair::Setup(char* szFolderName, char* szFileName)
{
	m_pAlloc = new cAllocateHierarchy;

	m_pAlloc->SetFolder(szFolderName);

	m_sPath = szFolderName + std::string("/") + szFileName;

	HRESULT hr = D3DXLoadMeshHierarchyFromX(m_sPath.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		m_pAlloc,
		NULL,
		&m_pFrameRoot,
		&m_pAnimControl);
	assert(hr == S_OK);
	SetupBoneMatrixPtrs((ST_BONE*)m_pFrameRoot);
	D3DXMATRIX matW;
	D3DXMatrixIdentity(&matW);
}

void cHair::Update()
{
	m_pAnimControl->AdvanceTime(0.0075f, NULL);

	D3DXMATRIX matW;

	matW = m_matHairTM;
	SetupWorldMatrix(m_pFrameRoot, &matW);

	UpdateSkinnedMesh(m_pFrameRoot);
}

void cHair::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->LightEnable(0, true);
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//D3DXMatrixIdentity(&m_pFrame->TransformationMatrix);
	ST_BONE* pBone = (ST_BONE*)m_pFrameRoot;
	RecursiveFrameRender(pBone, &pBone->CombinedTransformationMatrix);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_pD3DDevice->SetTexture(0, nullptr);
}

void cHair::RecursiveFrameRender(D3DXFRAME* pFrame, D3DXMATRIX* pParentWorldTM)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	D3DXMATRIX matW;
	D3DXMatrixIdentity(&matW);

	matW = pFrame->TransformationMatrix * (*pParentWorldTM);
	pBone->CombinedTransformationMatrix = matW;
	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->matWorldTM);

	//m_pMesh->DrawSubset(0);//����
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		//g_pD3DDevice->SetTexture(0, pFrame->pMeshContainer->pMaterials);
		for (size_t i = 0; i < pBoneMesh->dwNumSubset; ++i)
		{
			g_pD3DDevice->SetTexture(0, pBoneMesh->vecTexture [i]);
			g_pD3DDevice->SetMaterial(&pBoneMesh->vecMaterial[i]);
			pBoneMesh->MeshData.pMesh->DrawSubset(i);
		}
	}
	//�ڽ�
	if (pFrame->pFrameFirstChild)
	{
		RecursiveFrameRender(pFrame->pFrameFirstChild, &matW);
	}
	//����
	if (pFrame->pFrameSibling)
	{
		RecursiveFrameRender(pFrame->pFrameSibling, pParentWorldTM);
	}
}

void cHair::SetupWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIX* pMatParent)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;

	if (pMatParent)
	{
		pBone->CombinedTransformationMatrix *= (*pMatParent);
	}

	if (pBone->pFrameSibling)
	{
		SetupWorldMatrix(pBone->pFrameSibling, pMatParent);
	}
	if (pBone->pFrameFirstChild)
	{
		SetupWorldMatrix(pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);
	}
}

void cHair::SetupBoneMatrixPtrs(D3DXFRAME* pFrame)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

	// �� �������� �޽� �����̳ʿ� �ִ� pSkinInfo�� �̿��Ͽ� ����޴� ��� 
	// �������� ��Ʈ������ ppBoneMatrixPtrs�� �����Ѵ�.

	// pSkinInfo->GetNumBones() ���� ����޴� ���� ������ ã��.
	// pSkinInfo->GetBoneName(i) �� i�� �������� �̸��� ã��
	// D3DXFrameFind(��Ʈ ������, ������ �̸�) �� �������� ã��.
	// ã�Ƽ� �����Ʈ������ �ɾ����.

	//��������� ��� �����ӿ� ���ؼ� ����.
	if (pBoneMesh)
	{
		if (pBoneMesh->pSkinInfo)
		{
			DWORD nNumBones = pBoneMesh->pSkinInfo->GetNumBones();

			std::string sBoneName;
			for (size_t i = 0; i < nNumBones; ++i)
			{
				sBoneName = pBoneMesh->pSkinInfo->GetBoneName(i);
				ST_BONE* pFindedBone = (ST_BONE*)D3DXFrameFind(m_pFrameRoot, sBoneName.c_str());
				pBoneMesh->ppBoneMatrixPtrs[i] = &pFindedBone->CombinedTransformationMatrix;
				//pTargetBone->matWorldTM = pTargetBone->TransformationMatrix * ((ST_BONE*)m_pFrameRoot)->matWorldTM;
			}
		}
	}
	if (pBone->pFrameSibling)
	{
		SetupBoneMatrixPtrs(pBone->pFrameSibling);
	}
	if (pBone->pFrameFirstChild)
	{
		SetupBoneMatrixPtrs(pBone->pFrameFirstChild);
	}
}

void cHair::UpdateSkinnedMesh(D3DXFRAME* pFrame)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

	if (pBoneMesh)
	{
		DWORD nNumBones = pBoneMesh->pSkinInfo->GetNumBones();
		for (size_t i = 0; i < nNumBones; ++i)
		{
			pBoneMesh->pCurrentBoneMatrices[i] = pBoneMesh->pBoneOffsetMatrices[i] * (*pBoneMesh->ppBoneMatrixPtrs[i]);
		}
		BYTE* src = NULL;
		BYTE* dest = NULL;

		pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
		pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&dest);

		//MeshData.pMesh�� ������Ʈ �����ش�.
		pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
			pBoneMesh->pCurrentBoneMatrices, NULL, src, dest);

		pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
		pBoneMesh->pOrigMesh->UnlockVertexBuffer();
	}
	if (pBone->pFrameSibling)
	{
		UpdateSkinnedMesh(pBone->pFrameSibling);
	}
	if (pBone->pFrameFirstChild)
	{
		UpdateSkinnedMesh(pBone->pFrameFirstChild);
	}
}
