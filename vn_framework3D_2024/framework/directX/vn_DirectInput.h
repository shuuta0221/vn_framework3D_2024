//--------------------------------------------------------------//
//	"vn_DirectInput.h"											//
//		DirectInput�Ǘ�											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

#define vnJOYSTICK_MAX	(4)	//�g�p�ł���W���C�X�e�B�b�N�̐�

class vnDirectInput
{
private:
	static LPDIRECTINPUT8		pDI;					// DirectInput�I�u�W�F�N�g
	static LPDIRECTINPUTDEVICE8	pDIKeyboard;			// �f�o�C�X:keyboard
	static LPDIRECTINPUTDEVICE8	pDIMouse;				// �f�o�C�X:mouse
	static LPDIRECTINPUTDEVICE8	pDIDev[vnJOYSTICK_MAX];	// �f�o�C�X:joystick
	
	static DIMOUSESTATE2	diMouse;			// �}�E�X�p
	static int				Mouse_x;			// �}�E�X�̍��W
	static int				Mouse_y;			// �}�E�X�̍��W
	static BYTE				diKeyboard[256];	// �L�[�{�[�h�p
	static BYTE				oldKeyboard[256];	// 1�t���[���O�̃L�[�{�[�h�̏��
	static DIJOYSTATE		diJoystick[vnJOYSTICK_MAX];
	static DIJOYSTATE		oldJoystick[vnJOYSTICK_MAX];
	
	static int				JoystickButtonNum;
	static int				JoystickAxisRange;
	static float			JoystickAxisRangeI;
	
	static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext);
	static BOOL CALLBACK EnumAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

public:
	static HRESULT initialize(void *hinst);
	static void terminate();
	
	static void setMousePosition(int x, int y);
	static void KeyManager();
	
	static int getMouseX();
	static int getMouseY();
	static DIMOUSESTATE2	*getMouseState();
	static BYTE				*getCurrentKeyboardState();
	static BYTE				*getOldKeyboardState();
	static DIJOYSTATE		*getCurrentJoystick(int id=0);
	static DIJOYSTATE		*getOldJoystick(int id=0);
	static bool				useJoystick(int id=0);
	
	static int		getJoystickButtonNum();
	static float	getJoystickAxisRangeI();
};
