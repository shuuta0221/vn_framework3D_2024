//--------------------------------------------------------------//
//	"vn_sound.h"												//
//		WAVE�t�@�C���Đ��N���X(XAudio2)							//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnSound
{
private:
	IXAudio2SourceVoice* pSourceVoice;
	XAUDIO2_BUFFER xbuf;

	UINT64			lastSamplesPlayed;
	UINT64			lastSamplesStopped;
	WAVEFORMATEX	wfex;
	
public:
	//�R���X�g���N�^
	vnSound(const WCHAR *path);
	//�f�X�g���N�^
	~vnSound();
	
	bool play(bool start=true);	//�Đ�	start=true : �擪����Đ�, start=false : �O��̒�~�ʒu����Đ�
	bool stop();				//��~
	
	bool isPlaying();	//�Đ��������ׂ�
	bool isStopped();	//��~�������ׂ�
	
	//�e��ݒ�
	bool setVolume(float volume);			//���� : DSBVOLUME_MAX (�����Ȃ�:0) ���� DSBVOLUME_MIN (����:-10,000)
	bool setFrequencyRatio(float frequency);//�I�[�f�B�I �o�b�t�@���Đ�����Ă�����g��(Hz �P��) / �f�t�H���g�l : DSBFREQUENCY_ORIGINAL
	
	//�e��擾
	float getVolume();
	float getFrequencyRatio();

	float getPlayingTime();

	float getCurrentPlayingPosition();

	static float convertSecond(UINT64 data, int samplesPerSec, int bitsPerSample, int channels);
};

