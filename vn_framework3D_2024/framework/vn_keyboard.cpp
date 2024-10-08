//--------------------------------------------------------------//
//	"vn_keyboard.cpp"											//
//		�L�[�{�[�h����(DirectInput)								//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

bool vnKeyboard::on(BYTE key)
{
	BYTE *cur = vnDirectInput::getCurrentKeyboardState();
	return (cur[key]!=0);
}

bool vnKeyboard::trg(BYTE key)
{
	BYTE *cur = vnDirectInput::getCurrentKeyboardState();
	BYTE *old = vnDirectInput::getOldKeyboardState();
	return (cur[key]!=0 && old[key]==0);
}

bool vnKeyboard::rel(BYTE key)
{
	BYTE *cur = vnDirectInput::getCurrentKeyboardState();
	BYTE *old = vnDirectInput::getOldKeyboardState();
	return (cur[key]==0 && old[key]!=0);
}
