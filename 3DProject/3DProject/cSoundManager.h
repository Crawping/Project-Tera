#pragma once
#include "Singleton.h"
#include "inc\fmod.hpp"

#pragma comment(lib, "lib/fmodex_vc.lib")

#define EXTRA_SOUND_CHANNEL 5
#define SOUNDBUFFER 10

#define TOTAL_SOUND_CHANNEL (EXTRA_SOUND_CHANNEL + SOUNDBUFFER)

#define SOUNDMANAGER cSoundManager::Get()


using namespace FMOD;


class cSoundManager :
	public TSingleton<cSoundManager>
{
public:
	cSoundManager();
	virtual ~cSoundManager();

	//���� �߰�
	void addSound(std::string keyName, std::string soundName, bool background = false, bool loop = false);

	int a = 0;

	//���
	void play(std::string keyName, float volume);

	//����
	void stop(std::string keyName);

	//�Ͻ� ����
	void pause(std::string keyName);

	//�ٽ� ���
	void resume(std::string keyName);

	//�����ִ�?
	bool isPauseSound(std::string keyName);

	//������̴�?
	bool isPlaySound(std::string keyName);

	//fmod update
	void update();

private:
	typedef std::map<std::string, Sound**> m_mapSound;
	typedef std::map<std::string, Sound**>::iterator m_mapSoundIter;
	typedef std::map<std::string, Channel**> m_mapChannel;
	typedef std::map<std::string, Channel**>::iterator m_mapChannelter;

private:
	System* _system;
	Sound** _sound;

	Channel** _channel;

	m_mapSound _mTotalSounds;
};

