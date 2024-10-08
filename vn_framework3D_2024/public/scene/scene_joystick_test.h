//--------------------------------------------------------------//
//	"scene_joystick_test.h"										//
//		�W���C�X�e�B�b�N�e�X�g									//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class SceneJoystickTest : public vnScene
{
private:
	bool testXInput;				//DirectInput / XInput �؂�ւ�
	bool vibrate[XUSER_MAX_COUNT];	//XInput:�U���̑���

	void executeDInput();
	void executeXInput();

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
