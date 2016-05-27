#pragma once

#include "Particle.h"

class cParticle_Base
{
protected:
	D3DXMATRIXA16						m_matWorld;			
	float								m_fSize;			//��ƼŬ ũ��
	LPDIRECT3DTEXTURE9					m_pTexture;			//��ƼŬ �ؽ�ó
	LPDIRECT3DVERTEXBUFFER9				m_pVertexBuffer;	//��ƼŬ ���ؽ� ����
	std::list<ParticleAttribute>		m_lParticles;		//��ü ��ƼŬ ����Ʈ

	//�Ʒ� 3���� ��ƼŬ�� �� �� ȿ�������� �׸��� ���� ����
	DWORD								m_dwVbSize;			//���ؽ� ���� ũ��
	DWORD								m_dwVbOffset;		//���ؽ� ���۸� ��ױ� ���� offset(��ƼŬ ���� ����)
	DWORD								m_dwVbBatchSize;	//�ϳ��� �ܰ迡 �� ��ƼŬ�� ����

	virtual void PreRender();
	virtual void PostRender();
	DWORD FloatToDword(float f) { return *((DWORD*)(&f)); }

public:
	cParticle_Base();
	virtual ~cParticle_Base();

	void Setup(char* szFullPath);
	virtual void Update() = 0;
	void Render();

	virtual void AddParticle();		//��ƼŬ �ϳ� �߰�

	virtual void Reset();			//��ƼŬ �����
	virtual void ResetParticle(ParticleAttribute& attribute) = 0;		//��ƼŬ Ÿ�Կ� �°� �����ϱ� ���� ��

	bool IsEmpty();		//��ƼŬ�� �������
	bool IsDead();		//��ƼŬ�� �� ����
};

