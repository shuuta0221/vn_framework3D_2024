//--------------------------------------------------------------//
//	"vn_object.h"												//
//		�I�u�W�F�N�g���N���X									//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

#include "../framework/vn_modelData.h"

//�|���S�����_�\����
struct vnVertex3D
{
	float x,y,z;		//���W
	float nx,ny,nz;		//�@��
	float r,g,b,a;		//�J���[
	float u,v;			//UV
};

class vnObject
{
protected:
	//�ړ��l
	XMVECTOR	Position;
	
	//��]�l
	XMVECTOR	Rotation;
	
	//�g��l
	XMVECTOR	Scale;

	//�ϊ��s��
	XMMATRIX	Local;
	XMMATRIX	World;

	//���s��Ԃ̊Ǘ�
	bool executeEnable;

	//�`���Ԃ̊Ǘ�
	bool renderEnable;

	//���C�e�B���O�̗L��/����
	bool lighting;

	//�������̗L��/����
	bool transparent;

	//�K�w�\���̐e
	vnObject* pParent;

	//�}�g���N�X�̌v�Z
	void calculateLocalMatrix();
	void calculateWorldMatrix();


	//�ÓI���ʃf�[�^
	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[];	//���_�v�f
	static ID3D12PipelineState		*pPipelineState;		//�p�C�v���C���X�e�[�g
	static ID3D12PipelineState		*pPipelineState_NL;		//�p�C�v���C���X�e�[�g(���C�e�B���O����)
	static ID3D12PipelineState      *pPipelineState_Alpha;		//������
	static ID3D12PipelineState		*pPipelineState_Alpha_NL;	//������(���C�e�B���O����)

	//�R���X�^���g�o�b�t�@�\����
	struct stConstantBuffer
	{
		XMFLOAT4X4	WVP;		//World*View*Proj
		XMFLOAT4X4	World;		//World
		XMFLOAT4	LightDir;	//���s������(�t)����
		XMFLOAT4	LightCol;	//���s�����̐F
		XMFLOAT4	LightAmb;	//�����̐F
		XMFLOAT4	CameraPos;	//�J�����̃��[���h���W
		XMFLOAT4	Diffuse;	//�}�e���A���̊g�U�F
		XMFLOAT4	Ambient;	//�}�e���A���̊��F
		XMFLOAT4	Specular;	//�}�e���A���̋��ʔ��ˌ�
	};

public:
	//�ÓI���ʃf�[�^������
	static bool initializeCommon();

	//�ÓI���ʃf�[�^�폜
	static void terminateCommon();

	//�R���X�g���N�^
	vnObject();

	//�f�X�g���N�^
	virtual ~vnObject();
	
	//�V�X�e���֐�
	virtual void execute();
	virtual void render();

	//�ړ��l�̐ݒ�
	void setPosition(float x, float y, float z);
	void setPosition(const XMVECTOR *v);
	void setPositionX(float value);
	void setPositionY(float value);
	void setPositionZ(float value);
	void addPosition(float x, float y, float z);
	void addPosition(const XMVECTOR *v);
	void addPositionX(float value);
	void addPositionY(float value);
	void addPositionZ(float value);

	//�ړ��l�̎擾
	XMVECTOR *getPosition();
	float getPositionX(void);
	float getPositionY(void);
	float getPositionZ(void);


	//��]�l�̐ݒ�
	void setRotation(float x, float y, float z);
	void setRotation(const XMVECTOR *v);
	void setRotationX(float radian);
	void setRotationY(float radian);
	void setRotationZ(float radian);
	void addRotation(float x, float y, float z);
	void addRotation(const XMVECTOR *v);
	void addRotationX(float radian);
	void addRotationY(float radian);
	void addRotationZ(float radian);

	//��]�l�̎擾
	XMVECTOR *getRotation();
	float getRotationX(void);
	float getRotationY(void);
	float getRotationZ(void);
	
	
	//�g��l�̐ݒ�
	void setScale(float x, float y, float z);
	void setScale(const XMVECTOR *v);
	void setScaleX(float value);
	void setScaleY(float value);
	void setScaleZ(float value);
	void addScale(float x, float y, float z);
	void addScale(const XMVECTOR *v);
	void addScaleX(float value);
	void addScaleY(float value);
	void addScaleZ(float value);

	//�g��l�̎擾
	XMVECTOR *getScale();
	float getScaleX(void);
	float getScaleY(void);
	float getScaleZ(void);


	//���[���h�s��̎擾
	XMMATRIX *getWorld();

	//���[���h���W�̎擾
	void getWorldPosition(XMVECTOR *v);
	float getWorldPositionX();
	float getWorldPositionY();
	float getWorldPositionZ();

	//���[���h�s��̐ݒ�
	void setWorld(XMMATRIX *m);


	//���s��Ԃ̐ݒ�
	void setExecuteEnable(bool flag);

	//�`���Ԃ̐ݒ�
	void setRenderEnable(bool flag);

	//���s��Ԃ̎擾
	bool isExecuteEnable();

	//�`���Ԃ̎擾
	bool isRenderEnable();

	//���C�e�B���O�̗L��/�����̐ݒ�
	void setLighting(bool flag);

	//���C�e�B���O�̗L��/�����̎擾
	bool getLighting();

	//�������̗L�� / �����̐ݒ�
	void setTransparent(bool flag);

	//�������̗L��/�����̎擾
	bool getTransparent();

	//�K�w�\���̐e�̐ݒ�
	void setParent(vnObject* p);

	//�K�w�\���̐e�̎擾
	vnObject* getParent(void);
};
