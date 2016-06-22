#include "stdafx.h"
#include "cParticle_Base.h"


cParticle_Base::cParticle_Base()
	: m_fSize(0.f)
	, m_pVertexBuffer(nullptr)
	, m_pTexture(nullptr)
	, m_dwVbBatchSize(512)
	, m_dwVbOffset(0)
	, m_dwVbSize(2048)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cParticle_Base::~cParticle_Base()
{
	SAFE_RELEASE(m_pVertexBuffer);
	m_pTexture = nullptr;
	m_lParticles.clear();
}

void cParticle_Base::Setup(char* szFullPath)
{
	HRESULT hr = 0;

	hr = g_pD3DDevice->CreateVertexBuffer(
		m_dwVbSize * sizeof(Particle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		Particle::FVF,
		D3DPOOL_DEFAULT,
		&m_pVertexBuffer, 0
		);

	if (FAILED(hr))
	{
		::MessageBox(0, "CreateVertexBuffer() - FAILED", "Particle_Base", MB_OK);
	}

	m_pTexture = g_pTextureManager->GetTexture(szFullPath);
}

void cParticle_Base::Render()
{
	if (!m_lParticles.empty())
	{
		PreRender();

		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
		g_pD3DDevice->SetTexture(0, m_pTexture);
		g_pD3DDevice->SetFVF(Particle::FVF);
		g_pD3DDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Particle));

		//vbBathSize������ �ϳ��� �׸���.
		//offset�� ũ�Ⱑ ���ؽ������� ���� ���������� �ٽ� ó������
		if (m_dwVbOffset >= m_dwVbSize)
		{
			m_dwVbOffset = 0;
		}

		Particle* v = 0;

		m_pVertexBuffer->Lock(
			m_dwVbOffset * sizeof(Particle),
			m_dwVbBatchSize * sizeof(Particle),
			(void**)&v,
			m_dwVbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

		DWORD m_dwnumParticlesinBatch = 0;

		// ��� ��ƼŬ�� �� �׸���.
		for (auto iter = m_lParticles.begin(); iter != m_lParticles.end(); ++iter)
		{
			if (iter->isAlive)
			{
				//���ؽ� ���۷� ����
				v->pos = iter->pos;
				v->color = (D3DCOLOR)iter->color;
				v++; //���ؽ� ������ ���� ��ġ�� �̵�;

				m_dwnumParticlesinBatch++; //���� ���� ����

				// �� �ܰ�(batch, ���)�� �� ������ Ȯ��
				if (m_dwnumParticlesinBatch == m_dwVbBatchSize)
				{
					// �� �ܰ� ���ΰ��� �׸���
					m_pVertexBuffer->Unlock();

					g_pD3DDevice->DrawPrimitive(
						D3DPT_POINTLIST,
						m_dwVbOffset,
						m_dwVbBatchSize);

					// �׸��� ���� ���� �ܰ踦 ä���					
					m_dwVbOffset += m_dwVbBatchSize;	// ���� �ܰ�� �̵�

					// offset�� ũ�Ⱑ ���ؽ������� ���� ���������� �ٽ� ó������
					if (m_dwVbOffset >= m_dwVbSize)
						m_dwVbOffset = 0;

					m_pVertexBuffer->Lock(
						m_dwVbOffset    * sizeof(Particle),
						m_dwVbBatchSize * sizeof(Particle),
						(void**)&v,
						m_dwVbOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);	//ó�� ���ۺκа� ����

					m_dwnumParticlesinBatch = 0; // �ٽ� ó������ ī����
				}
			}
		}
		m_pVertexBuffer->Unlock();	//������ ���

		// ������ �׸���
		if (m_dwnumParticlesinBatch)
		{
			g_pD3DDevice->DrawPrimitive(
				D3DPT_POINTLIST,
				m_dwVbOffset,
				m_dwnumParticlesinBatch);
		}

		// ���� �������� ���� offset����
		m_dwVbOffset += m_dwVbBatchSize;

		PostRender();
	}
}

void cParticle_Base::AddParticle()
{
	ParticleAttribute attribute;
	ResetParticle(attribute);
	m_lParticles.push_back(attribute);
}

void cParticle_Base::Reset()
{
	for (auto iter = m_lParticles.begin(); iter != m_lParticles.end(); ++iter)
	{
		ResetParticle(*iter);
	}
}

bool cParticle_Base::IsEmpty()
{
	return m_lParticles.empty();
}

bool cParticle_Base::IsDead()
{
	for (auto iter = m_lParticles.begin(); iter != m_lParticles.end(); ++iter)
	{
		if (iter->isAlive)
			return false;
	}
	return true;
}

void cParticle_Base::PreRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FloatToDword(m_fSize));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDword(0.0f));

	// control the size of the particle relative to distance
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FloatToDword(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FloatToDword(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FloatToDword(1.0f));

	// use alpha from texture
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void cParticle_Base::PostRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}