//--------------------------------------------------------------//
//	"vn_mainframe.h"											//
//		���C���t���[��											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnMainFrame
{
private:
	static vnScene* pScene;

public:
	static int screenWidth;		//��ʂ̕�
	static int screenHeight;	//��ʂ̍���
	static int screenCenterX;	//��ʂ̒���(��)
	static int screenCenterY;	//��ʂ̒���(�c)

	static float clearColor[4];	//��ʂ̃N���A�J���[(RGBA:0.0�`1.0)

	static void initializeScene(vnScene* p);	//�V�[���̏�����
	static void terminateScene();				//�V�[���̏I��

	//������
	static bool initialize(void* hWnd, void* hInst, int width, int height);

	//�I��
	static void terminate();

	//���s
	static void execute();
};
