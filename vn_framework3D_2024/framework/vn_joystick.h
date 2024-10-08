//--------------------------------------------------------------//
//	"vn_joystick.h"												//
//		�W���C�X�e�B�b�N����(DirectInput)						//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnJoystick
{
public:
	static bool isValid(int id=0);			//�W���C�X�e�B�b�N���L�������ׂ�
	
	static bool on(int button, int id=0);	//�{�^����������Ă��邩���ׂ�(0�`32)
	static bool trg(int button, int id=0);	//�{�^���������ꂽ�u�Ԃ����ׂ�(0�`32)
	static bool rel(int button, int id=0);	//�{�^���������ꂽ�u�Ԃ����ׂ�(0�`32)
	
	static float leftX(int id=0);			//���X�e�B�b�N�̍��E���(-1.0�`1.0)
	static float leftY(int id=0);			//���X�e�B�b�N�̏㉺���(-1.0�`1.0)
	
	static float rightX(int id=0);			//�E�X�e�B�b�N�̍��E���(-1.0�`1.0)
	static float rightY(int id=0);			//�E�X�e�B�b�N�̏㉺���(-1.0�`1.0)
	
	static int	pov(int id=0);				//�|�C���g / �r���[ (�n�b�g�X�C�b�`) 
};
