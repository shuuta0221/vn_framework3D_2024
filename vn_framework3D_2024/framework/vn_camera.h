//--------------------------------------------------------------//
//	"vn_camera.h"												//
//		�J����													//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnCamera
{
private:
	static XMVECTOR			Position;	//�J�����̍��W
	static XMVECTOR			Target;		//�J�����̒����_
	static XMVECTOR			Up;			//�J�����̏����

	static float			FovY;		//����p
	static float			Aspect;		//�A�X�y�N�g��
	static float			zNear;		//�O���N���b�s���O�ʂ̋���
	static float			zFar;		//����N���b�s���O�ʂ̋���

	static XMMATRIX			View;		//�r���[�s��
	static XMMATRIX			Proj;		//�v���W�F�N�V�����s��
	static XMMATRIX			Screen;		//View*Proj
	
	static D3D11_VIEWPORT	Viewport;	//�r���[�|�[�g

public:
	static bool initialize();
	static void terminate();

	static XMMATRIX *getView();
	static XMMATRIX *getProj();
	static XMMATRIX *getScreen();
	
	static D3D11_VIEWPORT *getViewport();
	
	//�擾
	static const XMVECTOR *getPosition();
	static float getPositionX();
	static float getPositionY();
	static float getPositionZ();
	
	static const XMVECTOR *getTarget();
	static float getTargetX();
	static float getTargetY();
	static float getTargetZ();
	
	static const XMVECTOR *getUp();
	static float getUpX();
	static float getUpY();
	static float getUpZ();
	
	static float getFovY();
	static float getAspect();
	
	//�ݒ�
	static void setPosition(float x, float y, float z);
	static void setPosition(const XMVECTOR *v);
	static void setPositionX(float value);
	static void setPositionY(float value);
	static void setPositionZ(float value);
	
	static void setTarget(float x, float y, float z);
	static void setTarget(const XMVECTOR *v);
	static void setTargetX(float value);
	static void setTargetY(float value);
	static void setTargetZ(float value);
	
	static void setUp(float x, float y, float z);
	static void setUp(const XMVECTOR *v);
	static void setUpX(float value);
	static void setUpY(float value);
	static void setUpZ(float value);
	
	static void setFovY(float value);
	static void setAspect(float value);
	
	//���Z
	static void addPosition(float x, float y, float z);
	static void addPosition(const XMVECTOR *v);
	static void addPositionX(float value);
	static void addPositionY(float value);
	static void addPositionZ(float value);
	
	static void addTarget(float x, float y, float z);
	static void addTarget(const XMVECTOR *v);
	static void addTargetX(float value);
	static void addTargetY(float value);
	static void addTargetZ(float value);
	
	static void addFovY(float value);
	static void addAspect(float value);

};