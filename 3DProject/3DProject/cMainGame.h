#pragma once

class cGrid;
class cSkinnedMesh;
class cParticle_Firework;
class cSkyBox;

class cMainGame
{
	cGrid*					m_pGrid;	//�׸���
	cSkyBox*				m_pSkyBox;	//��ī�̹ڽ�

public:
	cMainGame();
	~cMainGame();

	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

