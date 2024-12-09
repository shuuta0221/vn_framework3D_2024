//--------------------------------------------------------------//
//	"vn_billboard.h"											//
//		�r���{�[�h�|���S��										//
//													2024/10/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnBillboard : public vnObject
{
protected:
	//��{���W
	XMVECTOR BasePos[4];

	//���_�f�[�^
	vnVertex3D	*vtx;

	//���_�o�b�t�@
	ID3D12Resource* vertBuff;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

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
	//�R���X�g���N�^
	vnBillboard() {}
	vnBillboard(float width, float height, const WCHAR* texture_file, float left_u = 0.0f, float right_u = 1.0f, float top_v = 0.0f, float bottom_v = 1.0f);

	//�f�X�g���N�^
	virtual ~vnBillboard();

	//����
	virtual void execute();

	//�`��
	virtual void render();

	//�g�U�F�̐ݒ�
	void setDiffuse(float r, float g, float b, float a);
};