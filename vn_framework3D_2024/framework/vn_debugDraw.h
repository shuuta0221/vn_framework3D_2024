//--------------------------------------------------------------//
//	"vn_debugDraw.h"											//
//		�f�o�b�O�`��											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//�|���S�����_�\����
struct vnVertex3D_Line
{
	float x,y,z;
	DWORD color;
};

//���_��
#define vnDebugDraw_VertexMax	(4096)

class vnDebugDraw
{
protected:
	//���_��
	static int						VertexNum;
	//���_�f�[�^
	static vnVertex3D_Line			*vtx;
	//�}�g���N�X
	static XMFLOAT4X4				*VP;

	//���_�o�b�t�@
	static ID3D12Resource* vertBuff;
	static D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	//�萔�o�b�t�@
	static ID3D12Resource* constBuff;
	static ID3D12DescriptorHeap* basicDescHeap;

	static D3D12_INPUT_ELEMENT_DESC inputElementDescs[];	//���_�v�f
	static ID3D12PipelineState* pPipelineState;				//�p�C�v���C���X�e�[�g

public:
	//[�V�X�e���Ǘ��֐�
	static bool initialize();
	static void terminate();
	static void render();
	//]
	
	static void Line(const XMVECTOR *start, const XMVECTOR *end, DWORD color=0xffffffff);
	static void Line(float start_x, float start_y, float start_z, float end_x, float end_y, float end_z, DWORD color = 0xffffffff);

	static void Grid(int num=10, float interval=1.0f, DWORD color=0x7fffffff);
	static void Axis(float length = 5.0f);
};
