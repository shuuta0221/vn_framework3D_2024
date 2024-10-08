//--------------------------------------------------------------//
//	"vn_XInput.h"												//
//		XInput�Ǘ�												//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnXInput
{
private:
	static constexpr float triggerMax = 1.0f / 255.0f;
	static constexpr float analogMax = 1.0f / 32768.0f;

	static constexpr int connectionCheckInterval = 60;	//�ڑ��`�F�b�N���s���Ԋu(�t���[��)

	static int connectionCheckCount;

	struct stState
	{
		DWORD			enable;
		XINPUT_STATE	state;
		XINPUT_STATE	past;
		float			LeftTrigger;
		float			RightTrigger;
		float			LeftX;
		float			LeftY;
		float			RightX;
		float			RightY;
	};
	static stState state[XUSER_MAX_COUNT];	//�ő�4

public:
	//[�V�X�e���֐�
	static HRESULT initialize();
	static void terminate();
	static void KeyManager();
	//]

	static bool isValid(int id = 0);			//�R���g���[���[���L�������ׂ�

	static bool on(WORD button, int id = 0);	//�{�^����������Ă��邩���ׂ�
	static bool trg(WORD button, int id = 0);	//�{�^���������ꂽ�u�Ԃ����ׂ�
	static bool rel(WORD button, int id = 0);	//�{�^���������ꂽ�u�Ԃ����ׂ�

	static float leftX(int id = 0);				//���X�e�B�b�N�̍��E���(-1.0�`1.0)
	static float leftY(int id = 0);				//���X�e�B�b�N�̏㉺���(-1.0�`1.0)

	static float rightX(int id = 0);			//�E�X�e�B�b�N�̍��E���(-1.0�`1.0)
	static float rightY(int id = 0);			//�E�X�e�B�b�N�̏㉺���(-1.0�`1.0)

	static float leftTrigger(int id = 0);		//���g���K�[�̉������(0.0�`1.0)
	static float rightTrigger(int id = 0);		//�E�g���K�[�̉������(0.0�`1.0)

	static void vibration(WORD speed = 0, int id = 0);			//�U��(���E�̃��[�^�[���� / 0�Œ�~)
	static void vibration(WORD left, WORD right, int id = 0);	//�U��(���E�̃��[�^�[�Ɨ� / 0�Œ�~)

};

//Document :
//https://docs.microsoft.com/ja-jp/windows/win32/xinput/getting-started-with-xinput

//Reference :
//https://docs.microsoft.com/ja-jp/windows/win32/api/xinput/ns-xinput-xinput_gamepad

/*
�f�o�C�X�{�^��

XINPUT_GAMEPAD_DPAD_UP			0x0001
XINPUT_GAMEPAD_DPAD_DOWN		0x0002
XINPUT_GAMEPAD_DPAD_LEFT		0x0004
XINPUT_GAMEPAD_DPAD_RIGHT		0x0008
XINPUT_GAMEPAD_START			0x0010
XINPUT_GAMEPAD_BACK				0x0020
XINPUT_GAMEPAD_LEFT_THUMB		0x0040
XINPUT_GAMEPAD_RIGHT_THUMB		0x0080
XINPUT_GAMEPAD_LEFT_SHOULDER	0x0100
XINPUT_GAMEPAD_RIGHT_SHOULDER	0x0200
XINPUT_GAMEPAD_A				0x1000
XINPUT_GAMEPAD_B				0x2000
XINPUT_GAMEPAD_X				0x4000
XINPUT_GAMEPAD_Y				0x8000

*/