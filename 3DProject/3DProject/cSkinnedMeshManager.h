#pragma once
#include "cSkinnedMesh.h"
#include "Singleton.h"

#define g_pSkinnedMeshManager cSkinnedMeshManager::Get()

class cSkinnedMeshManager
{
	TSingleton(cSkinnedMeshManager);

private:
	std::map<std::string, cSkinnedMesh*> m_mapSkinnedMesh;

public:
	cSkinnedMesh* GetSkinnedMesh(char* szFolder, char* szFile);
	void Destroy();
};

