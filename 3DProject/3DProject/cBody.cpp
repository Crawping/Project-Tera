#include "stdafx.h"
#include "cBody.h"
#include "cAllocateHierarchy.h"
#include "cMtlTex.h"


cBody::cBody()
	: m_pAlloc(nullptr)
	, m_pFrameRoot(nullptr)
	, m_pMesh(nullptr)
	, m_pBuffer(nullptr)
	, m_ft(0.1f)
{
	D3DXMatrixIdentity(&m_matNeckTM);
	D3DXMatrixIdentity(&m_matRootTM);
	D3DXMatrixIdentity(&m_matHairTM);
	D3DXMatrixIdentity(&m_matWorld);
}

cBody::~cBody()
{
	D3DXFrameDestroy(m_pFrameRoot, m_pAlloc);
	//m_pAlloc->DestroyFrame(m_pFrame);
	SAFE_DELETE(m_pAlloc);
	SAFE_RELEASE(m_pMesh);
}

void cBody::Setup(char* FolderName, char* FileName)
{
	std::string folder = (std::string)FolderName;
	std::string file = (std::string)FileName;

	m_pAlloc = new cAllocateHierarchy;

	m_pAlloc->SetFolder(folder);

	m_sPath = folder + std::string("/") + file;

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

void cBody::Update()
{
	m_pAnimControl->AdvanceTime(0.0075f, NULL);
	GetNeckWorld(m_pFrameRoot, nullptr);

	//�Ӹ��� �� ����TM�� ���� ��Ʈ���� ���Ѵ�.
	m_matHairTM = m_matHairTM * m_matWorld;
	m_matNeckTM = m_matNeckTM * m_matWorld;
	
	UpdateSkinnedMesh(m_pFrameRoot);
}
void cBody::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->LightEnable(0, true);
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//D3DXMatrixIdentity(&m_pFrame->TransformationMatrix);
	ST_BONE* pBone = (ST_BONE*)m_pFrameRoot;
	//D3DXMatrixScaling(&pBone->matWorldTM, 0.3f, 0.3f, 0.3f);//��ü ���� ũ������
	RecursiveFrameRender(pBone, &pBone->CombinedTransformationMatrix);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	g_pD3DDevice->SetTexture(0, nullptr);
}
void cBody::RecursiveFrameRender(D3DXFRAME* pFrame, D3DXMATRIX* pParentWorldTM)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	D3DXMATRIX matW;
	D3DXMatrixIdentity(&matW);

	matW = pFrame->TransformationMatrix * (*pParentWorldTM);

	pBone->CombinedTransformationMatrix = matW;

	D3DXMATRIXA16 matWorld;

	matWorld = pBone->CombinedTransformationMatrix * m_matWorld;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//m_pMesh->DrawSubset(0);//����
	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;
		//g_pD3DDevice->SetTexture(0, pFrame->pMeshContainer->pMaterials);
		for (size_t i = 0; i < pBoneMesh->dwNumSubset; ++i)
		{
			g_pD3DDevice->SetTexture(0, pBoneMesh->vecTexture[i]);
			g_pD3DDevice->SetMaterial(&pBoneMesh->vecMaterial[i]);
			g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
			pBoneMesh->MeshData.pMesh->DrawSubset(i);
			g_pD3DDevice->SetTexture(0, NULL);

			//���庯ȯ �������
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &pBone->CombinedTransformationMatrix);
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

void cBody::SetupWorldMatrix(D3DXFRAME* pFrame, D3DXMATRIX* pmatParent)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;

	if (pmatParent)
	{
		pBone->CombinedTransformationMatrix *= (*pmatParent);
	}

	if (pBone->pFrameSibling)
	{
		SetupWorldMatrix(pBone->pFrameSibling, pmatParent);
	}
	if (pBone->pFrameFirstChild)
	{
		SetupWorldMatrix(pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);
	}
}

void cBody::SetupBoneMatrixPtrs(D3DXFRAME* pFrame)
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

void cBody::UpdateSkinnedMesh(D3DXFRAME* pFrame)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;


	if (pBoneMesh)
	{
		pBoneMesh->matSphereW = pBone->CombinedTransformationMatrix;
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
void cBody::GetNeckWorld(D3DXFRAME* pFrame, D3DXMATRIX* pParentTM)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	D3DXMATRIXA16 matW;
	D3DXMatrixIdentity(&matW);
	if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Neck"))
	{
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix * (*pParentTM);
		m_matNeckTM = pBone->CombinedTransformationMatrix;
		//D3DXMatrixRotationX(&m_matNeckTM, D3DXToRadian(90));
	}
	else if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Bip01-Head"))
	{
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix * (*pParentTM);
		m_matHairTM = pBone->CombinedTransformationMatrix;
		//D3DXMatrixRotationX(&m_matNeckTM, D3DXToRadian(90));
	}
	else if (pBone->Name != nullptr && std::string(pBone->Name) == std::string("Dummy_root"))
	{
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix * (*pParentTM);
		m_matRootTM = pBone->CombinedTransformationMatrix;
		//D3DXMatrixRotationX(&m_matNeckTM, D3DXToRadian(90));
	}
	//-180 180 180
	if (pFrame->pFrameSibling)
	{
		GetNeckWorld(pFrame->pFrameSibling, &pBone->CombinedTransformationMatrix);
	}
	if (pFrame->pFrameFirstChild)
	{
		GetNeckWorld(pFrame->pFrameFirstChild, &pBone->CombinedTransformationMatrix);
	}
}


LPD3DXFRAME& cBody::GetFrameRoot()
{
	return this->m_pFrameRoot;
}
