//--------------------------------------------------------------//
//	"vn_scene.h"												//
//		�V�[���N���X											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

#define vnOBJECT2D_MAX (512)	//2D�I�u�W�F�N�g��o�^�ł���ő吔
#define vnOBJECT3D_MAX (512)	//3D�I�u�W�F�N�g��o�^�ł���ő吔

class vnScene
{
private:
	vnSprite *pObject2D_Array[vnOBJECT2D_MAX];	//2D�I�u�W�F�N�g��o�^���Ă����z��
	vnObject *pObject3D_Array[vnOBJECT3D_MAX];	//3D�I�u�W�F�N�g��o�^���Ă����z��

	//2D�I�u�W�F�N�g���\�[�g����K�v�����邩
	static bool UpdateRenderPriority;

public:
	vnScene();
	virtual ~vnScene();

	virtual bool initialize() = 0;
	virtual void terminate() = 0;
	
	virtual void execute();
	virtual void render();

	//�o�^���ꂽ2D�I�u�W�F�N�g�̕`��D�揇�ʂ��ύX���ꂽ�̂ŁA�z����\�[�g������K�v������
	static void setUpdateRenderPriority();

	//�I�u�W�F�N�g�̓o�^(�z��̋󂫂ɓ����)
	bool registerObject(vnSprite *p);

	//�I�u�W�F�N�g�̔j��(�z�񂩂�폜/�I�u�W�F�N�g���̂�delete����)
	void deleteObject(vnSprite *p);

	//�I�u�W�F�N�g�̓o�^(�z��̋󂫂ɓ����)
	bool registerObject(vnObject* p);	//3D�p

	//�I�u�W�F�N�g�̔j��(�z�񂩂�폜/�I�u�W�F�N�g���̂�delete����)
	void deleteObject(vnObject* p);		//3D�p
};
