//--------------------------------------------------------------//
//	"vn_sprite.h"												//
//		�X�v���C�g�N���X										//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//�|���S�����_�\����
struct vnVertex2D
{
	float x, y;			//�X�N���[�����W
	float r, g, b, a;	//���_�J���[
	float u, v;			//�e�N�X�`�����W
};

//�X�v���C�g�N���X
class vnSprite
{
public:
	//���W
	float posX;
	float posY;
	//�T�C�Y
	float sizeX;
	float sizeY;
	//�X�P�[��
	float scaleX;
	float scaleY;
	//��]
	float rot;

	//���_��
	static const int VertexNum = 4;
	//���_�f�[�^
	vnVertex2D	*vtx;

protected:
	//���_�f�[�^�ւ̊e����̐ݒ�
	void setVertexPosition();

private:

	//���s��Ԃ̊Ǘ�
	bool executeEnable;

	//�`���Ԃ̊Ǘ�
	bool renderEnable;

	//�`��D�揇��(�傫���قǎ�O�ɕ`�悳���)
	int renderPriority;

	//���Z����
	bool addition;

	//���_�o�b�t�@
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	ID3D12Resource* texbuff;
	ID3D12DescriptorHeap* basicDescHeap;

	//�ÓI���ʃf�[�^
	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[];	//���_�v�f
	static ID3D12PipelineState* pPipelineState;	//�p�C�v���C���X�e�[�g
	static ID3D12PipelineState* pPipelineState_Add;	//�p�C�v���C���X�e�[�g(���Z����)
	static ID3D12Resource* constBuff;	//�萔�o�b�t�@
	static float *scr;	//�X�v���C�g���X�N���[�����W�ň�����悤�ɂ��邽�߂̃V�F�[�_�[�萔

public:
	//�ÓI���ʃf�[�^������
	static bool initializeCommon();

	//�ÓI���ʃf�[�^�폜
	static void terminateCommon();

	//�R���X�g���N�^
	vnSprite() {}
	vnSprite(float x, float y, float width, float height, const WCHAR* texture_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);

	//�f�X�g���N�^
	virtual ~vnSprite();

	//����
	virtual void execute();

	//�`��
	virtual void render();

	//---- setter�֐� ----

	//���s��Ԃ̐ݒ�
	void setExecuteEnable(bool flag);

	//�`���Ԃ̐ݒ�
	void setRenderEnable(bool flag);

	//�`��D�揇�ʂ̐ݒ�
	void setRenderPriority(int value);

	//���Z�����̐ݒ�
	void setAddition(bool flag);

	//---- getter�֐� ----

	//���s��Ԃ̎擾
	bool isExecuteEnable();

	//�`���Ԃ̎擾
	bool isRenderEnable();

	//�`��D�揇�ʂ̎擾
	int getRenderPriority();

	//���Z�����̐ݒ�
	bool getAddition();
};