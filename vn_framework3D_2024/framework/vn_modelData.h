//--------------------------------------------------------------//
//	"vn_modelData.h"											//
//		���f���f�[�^��`(*.vnm / �o�C�i���t�@�C��)				//
//													2024/01/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//�o�[�W��������
#define vnMODEL_DATA_LATEST_VER (0x20240101)

struct vnModelData
{
	UINT Magic;
	UINT Date;
	UINT Version;
	UINT HeaderSize;

	UINT MaterialNum;
	UINT MeshNum;
	UINT VertexNum;
	UINT IndexNum;

	UINT MaterialAccess;
	UINT MeshAccess;
	UINT VertexAccess;
	UINT IndexAccess;

	UINT Align[4];
};

struct vnModel_MaterialData
{
	UINT StructSize;
	UINT TextureNum;
	UINT FloatNum;
	UINT Flag;
	char Texture[32];
	float Diffuse[4];
	float Ambient[4];
	float Specular[4];
};

struct vnModel_MeshData
{
	UINT MaterialID;
	UINT IndexNum;
	UINT StartIndex;
	UINT StartVertex;
};


//.bone�t�@�C���̒��g
struct vnModel_BoneData
{
	char Name[32];	//�{�[���̖��O
	int ParentID;	//�e�̔ԍ�(-1�͐e���Ȃ�)
	int Generation;	//����
	int StructSize;	//���̍\���̂̃T�C�Y(Byte)
	float pos[3];	//�W����Ԃł�Position(x,y,z)
	float rot[3];	//�W����Ԃł�Rotation(x,y,z)
	float scl[3];	//�W����Ԃł�Scale(x,y,z)
};
