//--------------------------------------------------------------//
//	"vn_mainframe.cpp"											//
//		���C���t���[��											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

#include <iostream>
#include <string>

//#define USE_GPU (1)
// 

#define FIXED_FRAMERATE (1)

#if FIXED_FRAMERATE
//�Œ�t���[�����[�g�̂��߂̃p�t�H�[�}���X�J�E���^�[
bool performanceCounter;
LARGE_INTEGER freq, startCount, finishCount;
float count;
#endif

int frameCount = 0;
float framePerSecond = 0.0f;
float countTime = 0.0f;


extern eSceneTable initialScene;

//�ÓI�ϐ�
int vnMainFrame::screenWidth;	//��ʂ̕�
int vnMainFrame::screenHeight;	//��ʂ̍���
int vnMainFrame::screenCenterX;	//��ʂ̒���(��)
int vnMainFrame::screenCenterY;	//��ʂ̒���(�c)

float vnMainFrame::clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.0f };	//��ʂ̃N���A�J���[

vnScene* vnMainFrame::pScene = NULL;

//���C���t���[���̏�����
bool vnMainFrame::initialize(void* hWnd, void* hInst, int width, int height)
{
	//��ʃT�C�Y�̐ݒ�
	screenWidth = width;
	screenHeight = height;
	screenCenterX = width / 2;
	screenCenterY = height / 2;

#if FIXED_FRAMERATE
	//�p�t�H�[�}���X�J�E���^�̏�����
	memset(&freq, 0, sizeof(freq));
	memset(&startCount, 0, sizeof(startCount));
	memset(&finishCount, 0, sizeof(finishCount));
	count = 0.0f;

	performanceCounter = QueryPerformanceFrequency(&freq);
#endif

	vnDirect3D::initialize();

	vnDirect2D::initialize();

	vnDirectInput::initialize(hInst);

	vnXInput::initialize();

	vnXAudio2::initialize();

	vnShader::initialize();

	vnFont::initialize();

	vnCamera::initialize();

	vnLight::initialize();

	vnDebugDraw::initialize();

	vnObject::initializeCommon();

	vnSprite::initializeCommon();

	switchScene();

	return true;
}

//�I��
void vnMainFrame::terminate()
{
	terminateScene();

	vnSprite::terminateCommon();

	vnObject::terminateCommon();

	vnDebugDraw::terminate();

	vnLight::terminate();

	vnCamera::terminate();

	vnFont::terminate();

	vnShader::terminate();

	vnXAudio2::terminate();

	vnXInput::terminate();

	vnDirectInput::terminate();

	vnDirect2D::terminate();

	vnDirect3D::terminate();

}

//���s
void vnMainFrame::execute()
{
#if FIXED_FRAMERATE
	if (performanceCounter)
	{	//�t���[���̊J�n���Ԃ��擾
		QueryPerformanceCounter(&startCount);
	}
#endif

	HRESULT hr = S_OK;

	//���͊Ǘ�
	vnDirectInput::KeyManager();
	vnXInput::KeyManager();

	//[�ʏ폈���J�n

	//HOME�{�^���ŋN�����̃X�e�[�W�ɖ߂�B
	if (vnKeyboard::trg(DIK_HOME))
	{
		switchScene(initialScene);
	}

	pScene->execute();
	//]�ʏ폈���I��


	//[Direct3D�`��
	vnDirect3D::beginRender();

	pScene->render();

	vnDebugDraw::render();

	vnDirect3D::finishRender();
	//]


	//[Direct2D �`��
	vnDirect2D::render();
	//]

#if FIXED_FRAMERATE
	//�t���[�����[�g�̌Œ�
	if (performanceCounter)
	{	//�t���[���̏I�����Ԃ��擾
		static float frame_msec = 1.0f / 60.0f;
		QueryPerformanceCounter(&finishCount);
		count = ((float)(finishCount.QuadPart - startCount.QuadPart) / (float)freq.QuadPart);

		if (count < frame_msec)
		{	//1/60�b���o�܂ő҂�
			DWORD wait = (DWORD)((frame_msec - count) * 1000.0f - 0.5f);
			timeBeginPeriod(1);
			Sleep(wait);
			timeEndPeriod(1);
		}
	}
#endif

	//�v���[���e�[�V����
	vnDirect3D::present();

	//�X�e�[�W�̐؂�ւ�
	switchScene();
}

void vnMainFrame::initializeScene(vnScene* p)
{
	if (p == NULL)return;
	pScene = p;
	pScene->initialize();
}

void vnMainFrame::terminateScene()
{
	if (pScene != NULL)
	{
		pScene->terminate();
		delete pScene;
	}
	pScene = NULL;
}
