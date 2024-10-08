//--------------------------------------------------------------//
//	"vn_DirectInput.cpp"										//
//		DirectInput�Ǘ�											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../vn_environment.h"

LPDIRECTINPUT8			vnDirectInput::pDI=NULL;				// DirectInput�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8	vnDirectInput::pDIKeyboard=NULL;		// keyboard
LPDIRECTINPUTDEVICE8	vnDirectInput::pDIMouse=NULL;			// mouse
LPDIRECTINPUTDEVICE8	vnDirectInput::pDIDev[vnJOYSTICK_MAX];	// joystick

DIMOUSESTATE2	vnDirectInput::diMouse;	// �}�E�X�̏��
int				vnDirectInput::Mouse_x;	// �}�E�X�̍��WX
int				vnDirectInput::Mouse_y;	// �}�E�X�̍��WY
BYTE			vnDirectInput::diKeyboard[256];	// �L�[�{�[�h�p
BYTE			vnDirectInput::oldKeyboard[256];	// 1�t���[���O�̃L�[�{�[�h�̏��
DIJOYSTATE		vnDirectInput::diJoystick[vnJOYSTICK_MAX];
DIJOYSTATE		vnDirectInput::oldJoystick[vnJOYSTICK_MAX];

int				vnDirectInput::JoystickButtonNum = sizeof(diJoystick[0].rgbButtons)/sizeof(BYTE);
int				vnDirectInput::JoystickAxisRange = 1000;
float			vnDirectInput::JoystickAxisRangeI = 1.0f/(float)JoystickAxisRange;

HRESULT vnDirectInput::initialize(void *hinst)
{
	HRESULT	hr;

	//IDirectInput8�C���^�[�t�F�C�X�̎擾
	hr = DirectInput8Create((HINSTANCE)hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDI, NULL);
	if( FAILED(hr) )
	{
		return hr;
	}
	
	//�W���C�X�e�B�b�N���̏�����
	for(int i=0; i<vnJOYSTICK_MAX; i++)
	{
		pDIDev[i]=NULL;
		ZeroMemory(&diJoystick[i],sizeof(DIJOYSTATE));
		ZeroMemory(&oldJoystick[i],sizeof(DIJOYSTATE));
	}
	
	//IDirectInputDevice8�C���^�[�t�F�C�X�̎擾
	//�}�E�X
    hr = pDI->CreateDevice(GUID_SysMouse, &pDIMouse, NULL);
	if( FAILED(hr) )
	{
		terminate();
		return E_FAIL;
    }
	hr = pDIMouse->SetDataFormat(&c_dfDIMouse2);//�f�[�^�t�H�[�}�b�g�̐ݒ�
	if( FAILED(hr) )
	{
		terminate();
		return E_FAIL;
	}
	
	//�L�[�{�[�h
	hr = pDI->CreateDevice(GUID_SysKeyboard, &pDIKeyboard, NULL);
	if( FAILED(hr) )
	{
		terminate();
		return E_FAIL;
	}
	hr = pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if( FAILED(hr) )
	{
		terminate();
		return E_FAIL;
	}
	
	//�W���C�X�e�B�b�N
	//�f�o�C�X�̗�
	hr = pDI->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoysticksCallback,NULL,DIEDFL_ATTACHEDONLY);
	if(FAILED(hr))
	{
		terminate();
		return E_FAIL;
	}
	//�f�[�^�`���̐ݒ�
	for(int i=0; i<vnJOYSTICK_MAX; i++)
	{
		if(pDIDev[i]==NULL)continue;
		
		hr = pDIDev[i]->SetDataFormat(&c_dfDIJoystick);
		if(FAILED(hr))
		{
			return E_FAIL;
		}
		//�t�H�A�O���E���h&��r�����[�h�̐ݒ�
	/*	hr = pDIDev[2]->SetCooperativeLevel(hwndApp,DISCL_NONEEXCLUSIVE | DISCL_FOREGROUND);
		if(FAILED(hr))
		{
			return E_FAIL;
		}*/
		//�R�[���o�b�N�֐����g���Ċe���̃��[�h��ݒ�
		hr = pDIDev[i]->EnumObjects(EnumAxisCallback,&i,DIDFT_AXIS);
		if(FAILED(hr))
		{
			return E_FAIL;
		}
		//�����[�h���Βl���[�h�ɐݒ�
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_ABS;
		hr = pDIDev[i]->SetProperty(DIPROP_AXISMODE, &diprop.diph);
		if(FAILED(hr))
		{
			return E_FAIL;
		}
		//�o�b�t�@�����O�f�[�^���擾���邽�߃o�b�t�@�T�C�Y��ݒ�
		diprop.dwData = 100;//DIDEVICE_BUFFERSIZE;
		hr = pDIDev[i]->SetProperty(DIPROP_BUFFERSIZE,&diprop.diph);
		if(FAILED(hr))
		{
			return E_FAIL;
		}
		//���͐���J�n
		pDIDev[i]->Acquire();
	}
	
	if( FAILED(hr) )	return hr;
	return	S_OK;
}

void vnDirectInput::terminate()
{
	if(pDIKeyboard)	pDIKeyboard->Unacquire();
	SAFE_RELEASE(pDIKeyboard)
	
	if(pDIMouse)	pDIMouse->Unacquire();
	SAFE_RELEASE(pDIMouse)
	
	for(int i=0; i<vnJOYSTICK_MAX; i++)
	{
		if(pDIDev[i])	pDIDev[i]->Unacquire();
		SAFE_RELEASE(pDIDev[i]);
	}
	SAFE_RELEASE( pDI );
}

//�W���C�X�e�B�b�N���
BOOL CALLBACK vnDirectInput::EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	DIDEVCAPS g_diDevCaps;	//�W���C�X�e�B�b�N�̔\��
	HRESULT hr;
	
	//�󂫂�T��
	int id=-1;
	for(int i=0; i<vnJOYSTICK_MAX; i++)
	{
		if(pDIDev[i]==NULL)
		{
			id = i;
			break;
		}
	}
	//�󂫂��Ȃ���ΏI���
	if(id<0)
	{
		return DIENUM_STOP;
	}
	
	//�񋓂��ꂽ�W���C�X�e�B�b�N�ւ̃C���^�[�t�F�C�X���擾
	hr = pDI->CreateDevice(pdidInstance->guidInstance, &pDIDev[id], NULL);
	if(FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}
	//�W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = pDIDev[id]->GetCapabilities(&g_diDevCaps);
	if(FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}
	if(id>=vnJOYSTICK_MAX)
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

//�W���C�X�e�B�b�N�̎����
BOOL CALLBACK vnDirectInput::EnumAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	int id = *((int*)pvRef);
	
	HRESULT hr;
	//���̒l�͈̔͂�ݒ�
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -JoystickAxisRange;
	diprg.lMax = +JoystickAxisRange;
	hr = pDIDev[id]->SetProperty(DIPROP_RANGE, &diprg.diph);
	if(FAILED(hr))
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

void vnDirectInput::setMousePosition(int x, int y)
{
	Mouse_x = x;
	Mouse_y = y;
}

void vnDirectInput::KeyManager()
{
	HRESULT hr;
	
	//�}�E�X
	hr = pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &diMouse);
	if (FAILED(hr))
	{
		hr = pDIMouse->Acquire();
	}
	
	//�L�[�{�[�h
	memcpy(oldKeyboard, diKeyboard, sizeof(oldKeyboard));	//�O�t���[���̃L�[��Ԃ�ۑ�
	hr = pDIKeyboard->GetDeviceState(256, diKeyboard);
	if (FAILED(hr))
	{
		hr = pDIKeyboard->Acquire();
	}
	
	//�W���C�X�e�B�b�N
	for(int i=0; i<vnJOYSTICK_MAX; i++)
	{
		if(pDIDev[i]==NULL)continue;
		memcpy(&oldJoystick[i],&diJoystick[i],sizeof(DIJOYSTATE));
		hr = pDIDev[i]->Poll();
		if(FAILED(hr))	hr = pDIDev[i]->Acquire();
		hr = pDIDev[i]->GetDeviceState(sizeof(DIJOYSTATE),&diJoystick[i]);
	//	if(FAILED(hr))	hr = pDIDev[i]->Acquire();
	}

	//�}�E�X�̏�Ԃ�ݒ�
	vnMouse::setMouseStatus();
}

int vnDirectInput::getMouseX()
{
	return Mouse_x;
}

int vnDirectInput::getMouseY()
{
	return Mouse_y;
}

DIMOUSESTATE2	*vnDirectInput::getMouseState()
{
	return &diMouse;
}

BYTE *vnDirectInput::getCurrentKeyboardState()
{
	return diKeyboard;
}

BYTE *vnDirectInput::getOldKeyboardState()
{
	return oldKeyboard;
}

DIJOYSTATE *vnDirectInput::getCurrentJoystick(int id)
{
	if(id<0 || id>=vnJOYSTICK_MAX)return NULL;
	return &diJoystick[id];
}

DIJOYSTATE *vnDirectInput::getOldJoystick(int id)
{
	if(id<0 || id>=vnJOYSTICK_MAX)return NULL;
	return &oldJoystick[id];
}

bool vnDirectInput::useJoystick(int id)
{
	if(id<0 || id>=vnJOYSTICK_MAX)return false;
	return pDIDev[id]!=NULL;
}

int vnDirectInput::getJoystickButtonNum()
{
	return JoystickButtonNum;
}

float vnDirectInput::getJoystickAxisRangeI()
{
	return JoystickAxisRangeI;
}
