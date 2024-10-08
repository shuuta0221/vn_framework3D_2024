//--------------------------------------------------------------//
//	"vn_sound.cpp"												//
//		WAVE�t�@�C���Đ��N���X(XAudio2)							//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

vnSound::vnSound(const WCHAR *path)
{
	HRESULT hr;
    MMCKINFO mSrcWaveFile;
    MMCKINFO mSrcWaveFmt;
    MMCKINFO mSrcWaveData;
    LPWAVEFORMATEX wf;

	pSourceVoice = NULL;

	// WAV�t�@�C�������[�h
    HMMIO hSrc;
    hSrc = mmioOpen( (LPWSTR)path, NULL,MMIO_ALLOCBUF|MMIO_READ|MMIO_COMPAT );
	if (!hSrc)
	{
		//    DEBUG( "WAV�t�@�C�����[�h�G���[\n","" );
		return;
	}

    // 'WAVE'�`�����N�`�F�b�N
    ZeroMemory( &mSrcWaveFile,sizeof(mSrcWaveFile) );
	hr = mmioDescend( hSrc,&mSrcWaveFile,NULL,MMIO_FINDRIFF );
    if( mSrcWaveFile.fccType!=mmioFOURCC('W','A','V','E') ) 
	{
    //    DEBUG( "WAVE�`�����N�`�F�b�N�G���[\n","" );
        mmioClose( hSrc,0 );
        return;
    }

    // 'fmt '�`�����N�`�F�b�N
    ZeroMemory( &mSrcWaveFmt,sizeof(mSrcWaveFmt) );
	hr = mmioDescend( hSrc,&mSrcWaveFmt,&mSrcWaveFile,MMIO_FINDCHUNK );
    if( mSrcWaveFmt.ckid!=mmioFOURCC('f','m','t',' ') )
	{
    //    DEBUG( "fmt �`�����N�`�F�b�N�G���[\n","" );
        mmioClose( hSrc,0 );
        return;
    }

    // �w�b�_�T�C�Y�̌v�Z
    int iSrcHeaderSize = mSrcWaveFmt.cksize;
    if( iSrcHeaderSize<sizeof(WAVEFORMATEX) )
        iSrcHeaderSize=sizeof(WAVEFORMATEX);

    // �w�b�_�������m��
    wf = (LPWAVEFORMATEX)new BYTE[iSrcHeaderSize];
    if( !wf )
	{
    //    DEBUG( "�������m�ۃG���[\n","" );
        mmioClose( hSrc,0 );
        return;
    }
    ZeroMemory( wf,iSrcHeaderSize );

    // WAVE�t�H�[�}�b�g�̃��[�h
	hr = mmioRead(hSrc, (char*)wf, mSrcWaveFmt.cksize);
    if( FAILED(hr) )
	{
     //   DEBUG( "WAVE�t�H�[�}�b�g���[�h�G���[\n","" );
        delete[] wf;
        mmioClose( hSrc,0 );
        return;
    }


    // fmt�`�����N�ɖ߂�
    mmioAscend( hSrc,&mSrcWaveFmt,0 );

    // data�`�����N��T��
    while(1)
	{
        // ����
		hr = mmioDescend( hSrc,&mSrcWaveData,&mSrcWaveFile,0 );
        if( FAILED(hr) )
		{
         //   DEBUG( "data�`�����N��������Ȃ�\n","" );
            delete[] wf;
            mmioClose( hSrc,0 );
            return;
        }
        if( mSrcWaveData.ckid==mmioStringToFOURCC(L"data",0) )
            break;
        // ���̃`�����N��
		hr = mmioAscend( hSrc,&mSrcWaveData,0 );
    }

	vnFont::output(L"%s\n", path);
	vnFont::output(L"�`�����l����       = %d\n", wf->nChannels);
	vnFont::output(L"�T���v�����O���[�g = %d\n", wf->nSamplesPerSec);
	vnFont::output(L"�r�b�g���[�g       = %d\n", wf->wBitsPerSample);
	vnFont::output(L"�f�[�^�T�C�Y       = %d\n", mSrcWaveData.cksize);

	const BYTE* buffer = new BYTE[mSrcWaveData.cksize];
	mmioRead(hSrc, (char*)buffer, mSrcWaveData.cksize);

	//�\�[�X�{�C�X�̍쐬
	hr = vnXAudio2::CreateSourceVoice(&pSourceVoice, wf);
	assert(hr == S_OK);

	//�Đ�����g�`�f�[�^�̐ݒ�
	xbuf.pAudioData = (const BYTE*)buffer;
	xbuf.Flags = XAUDIO2_END_OF_STREAM;
	xbuf.AudioBytes = mSrcWaveData.cksize;

	hr = pSourceVoice->SubmitSourceBuffer(&xbuf);
	assert(hr == S_OK);

	lastSamplesPlayed = 0;
	lastSamplesStopped = 0;
	memcpy(&wfex, wf, sizeof(WAVEFORMATEX));

	// �w�b�_�p���������J��
	delete[] wf;

	// WAV�����
	mmioClose(hSrc, 0);

}

vnSound::~vnSound()
{
	pSourceVoice->DestroyVoice();
	if(xbuf.pAudioData)delete[] xbuf.pAudioData;
}

bool vnSound::play(bool start)
{
	if(pSourceVoice)
	{
		HRESULT hr;
		if (start)
		{
			hr = pSourceVoice->Stop(0, 0);
			//pSourceVoice->ExitLoop();
			hr = pSourceVoice->Discontinuity();
			hr = pSourceVoice->FlushSourceBuffers();
			hr = pSourceVoice->SubmitSourceBuffer(&xbuf);
		}
		XAUDIO2_VOICE_STATE stat;
		pSourceVoice->GetState(&stat);
		lastSamplesPlayed = stat.SamplesPlayed;

		hr = pSourceVoice->Start(0, 0);
		return hr == S_OK;
	}
	return false;
}

bool vnSound::stop()
{
	if (pSourceVoice)
	{
		HRESULT hr = pSourceVoice->Stop(0, 0);

		XAUDIO2_VOICE_STATE stat;
		pSourceVoice->GetState(&stat);
		lastSamplesStopped = stat.SamplesPlayed;

		return hr == S_OK;
	}
	return false;
}

bool vnSound::isPlaying(void)
{
	if (pSourceVoice)
	{
		XAUDIO2_VOICE_STATE stat;
		pSourceVoice->GetState(&stat);
		return stat.SamplesPlayed > 0 && stat.SamplesPlayed != lastSamplesStopped;
	}
	return false;
}

bool vnSound::isStopped()
{
	return !isPlaying();
}

bool vnSound::setVolume(float volume)
{
	if (pSourceVoice)
	{
		if (volume > XAUDIO2_MAX_VOLUME_LEVEL)
		{
			volume = XAUDIO2_MAX_VOLUME_LEVEL;
		}
		else if (volume < 0.0f/* -XAUDIO2_MAX_VOLUME_LEVEL*/)
		{
			volume = 0.0f/* -XAUDIO2_MAX_VOLUME_LEVEL*/;
		}
		HRESULT hr = pSourceVoice->SetVolume(volume);
		return hr == S_OK;
	}
	return false;
}

bool vnSound::setFrequencyRatio(float frequency)
{
	if (pSourceVoice)
	{
		HRESULT hr = pSourceVoice->SetFrequencyRatio(frequency);
		return hr == S_OK;
	}
	return false;
}

float vnSound::getVolume()
{
	float ret = 0.0f;
	if (pSourceVoice)
	{
		pSourceVoice->GetVolume(&ret);
	}
	return ret;
}

float vnSound::getFrequencyRatio()
{
	if (pSourceVoice)
	{
		float ret = 0.0f;
		pSourceVoice->GetFrequencyRatio(&ret);
		return ret;

	}
	return 0.0f;
}

float vnSound::getPlayingTime()
{
	if (pSourceVoice)
	{
		return convertSecond(xbuf.AudioBytes, wfex.nSamplesPerSec, wfex.wBitsPerSample, wfex.nChannels);
	}
	return 0.0f;
}

float vnSound::getCurrentPlayingPosition()
{
	if (pSourceVoice)
	{
		XAUDIO2_VOICE_STATE stat;
		pSourceVoice->GetState(&stat);
		UINT64 data = stat.SamplesPlayed >= lastSamplesPlayed ? (stat.SamplesPlayed - lastSamplesPlayed) : 0;
		return (float)data / (float)(wfex.nSamplesPerSec);
	}
	return 0.0f;
}



float vnSound::convertSecond(UINT64 data, int samplesPerSec, int bitsPerSample, int channels)
{
	return (float)data / (float)(samplesPerSec * (bitsPerSample / 8) * channels);
}
