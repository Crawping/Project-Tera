#pragma once

#include "GameObject.h"

class cBoundingSphere : public GameObject
{
	LPD3DXMESH		sphereMesh;
	D3DMATERIAL9	stMtl;
	D3DXMATRIXA16	m_matWorld;
	bool			m_bWireDraw;		//���̾� ���������� �׸� ������?

public:
	cBoundingSphere();
	~cBoundingSphere();

	void Setup(D3DXVECTOR3* vCenter, float radius);
	void Update();
	void Render();
	bool SetWireDraw(bool isDraw) { m_bWireDraw = isDraw; }
};

