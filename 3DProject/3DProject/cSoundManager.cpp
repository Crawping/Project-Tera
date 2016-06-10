#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
{
	//���� �ý��� ����
	System_Create(&_system);

	//ä�� �� ���� �� �ʱ�ȭ
	_system->init(TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);


	//ä�ΰ� ���带 �����Ҵ�
	//�޸� ���� �� ���� ����
	_sound = new Sound*[TOTAL_SOUND_CHANNEL];
	_channel = new Channel*[TOTAL_SOUND_CHANNEL];

	//�ʱ�ȭ go
	memset(_sound, 0, sizeof(Sound*) * (TOTAL_SOUND_CHANNEL));
	memset(_channel, 0, sizeof(Channel*) * (TOTAL_SOUND_CHANNEL));

}


cSoundManager::~cSoundManager()
{
	//���� ����
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTAL_SOUND_CHANNEL; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != NULL)
			{
				if (_sound[i] != NULL) _sound[i]->release();
			}
		}
	}

	//�޸� �����
	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	//�ý��� �ݱ�
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}
}

//���� �߰�
void cSoundManager::addSound(std::string keyName, std::string soundName, bool background /*= false*/, bool loop /*= false*/)
{
	int a = 0;

	//���� ����?
	if (loop)
	{
		//��������̴�?
		if (background)
		{
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
		}
	}
	//���� �ƴ�
	else
	{
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� �־��ش�
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

//���
void cSoundManager::play(std::string keyName, float volume)
{
	m_mapSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &_channel[count]);

			//���� ����
			_channel[count]->setVolume(volume);
			break;
		}
	}
}

//����
void cSoundManager::stop(std::string keyName)
{
	m_mapSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

//�Ͻ� ����
void cSoundManager::pause(std::string keyName)
{
	m_mapSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

//�ٽ� ���
void cSoundManager::resume(std::string keyName)
{
	m_mapSoundIter iter = _mTotalSounds.begin();

	int count = 0;
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

//�����ִ�?
bool cSoundManager::isPauseSound(std::string keyName)
{
	bool isPause;
	int count = 0;
	m_mapSoundIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->getPaused(&isPause);
			break;
		}
	}
	return isPause;
}

//������̴�?
bool cSoundManager::isPlaySound(std::string keyName)
{
	bool isPlay;
	int count = 0;
	m_mapSoundIter iter = _mTotalSounds.begin();

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}

//fmod update
void cSoundManager::update()
{
	_system->update();

	//������ �ٲ�ų� ����� ���� ���带 ä�ο��� ������ ���� �پ��� �۾��� �ڵ����� ����
}