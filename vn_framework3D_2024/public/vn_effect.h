///--------------------------------------------------------------//
//	"vn_effect.h"												//
//		�G�t�F�N�g(�p�[�e�B�N��)�N���X							//
//													2024/12/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//�p�[�e�B�N���N���X
class vnParticle
{
public:
	float Life;	//����

	XMVECTOR Pos;	//�ʒu
	XMVECTOR Vel;	//���x
};

//�G�~�b�^�[�N���X
class vnEmitter : public vnObject
{
private:
	//�p�[�e�B�N���z��
	vnParticle* pParticle;


	//���_�f�[�^
	vnVertex3D* vtx;

	//�C���f�b�N�X�f�[�^
	WORD* idx;

	//���_�o�b�t�@
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//�C���f�b�N�X�o�b�t�@
	ID3D12Resource* pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	//�e�N�X�`��
	ID3D12Resource* texbuff;
	//�萔�o�b�t�@
	ID3D12Resource* constBuff;
	ID3D12DescriptorHeap* basicDescHeap;
	stConstantBuffer* pConstBuffer;

	//�}�e���A���֘A
	XMVECTOR	Diffuse;	//�g�U�F
	XMVECTOR	Ambient;	//���F
	XMVECTOR	Specular;	//���ʔ��ːF

	//���_�f�[�^�ւ̊e����̐ݒ�
	virtual void setVertexPosition();

public:
	vnEmitter();
	virtual ~vnEmitter();

	virtual void execute();

	virtual void render();
};