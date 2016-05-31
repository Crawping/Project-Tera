#pragma once

struct Particle
{
	D3DXVECTOR3		pos;		//��ƼŬ�� ������ ��
	D3DCOLOR		color;		//��ƼŬ ����

	Particle() 
		:pos(0, 0, 0), color(D3DCOLOR_XRGB(0, 0, 0))
	{
	}

	Particle(D3DXVECTOR3 _pos, D3DCOLOR _color)
		:pos(_pos), color(_color)
	{
	}

	enum {FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE};
};


struct ParticleAttribute
{
	D3DXVECTOR3		pos;		//��ƼŬ ��ġ
	D3DXVECTOR3		velocity;	//����ġ
	float			lifeTime;	//��ƼŬ ����ð�
	float			age;		//��ƼŬ�� ����
	D3DXCOLOR		color;		//��ƼŬ ����
	bool			isAlive;	//��ƼŬ�� ���翩��

	ParticleAttribute()
		: pos(0, 0, 0)
		, velocity(0, 0, 0)
		, lifeTime(0.f)
		, age(0.f)
		, color(D3DCOLOR_XRGB(255, 255, 255))
		, isAlive(false)
	{
	}
};