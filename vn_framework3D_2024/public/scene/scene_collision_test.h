#pragma once

#include "../object/cube.h"

class SceneCollisionTest : public vnScene
{
private:
	cCube* pCube;

	cCube* pTheta;
	cCube* pPhi;


	XMVECTOR Point[5];

	//�f�o�b�O�p��_�\�����f��
	vnModel* pPoint;

	//�����̏��
	XMVECTOR LinePos;
	XMVECTOR LineDir;

	//���ʂ̏��
	//���ʕ����� ax + by + cz = d
	XMVECTOR PlaneNormal;	//(a,b,c)
	float PlaneDistance;	//d

	//�ɍ��W�̏��
	float radius;	//���a
	float theta;	//���ʊp
	float phi;		//�p

	//����̎��
	enum eOperation
	{
		Point0PosX,
		Point0PosY,
		Point0PosZ,
		Point1PosX,
		Point1PosY,
		Point1PosZ,
		Point2PosX,
		Point2PosY,
		Point2PosZ,
		Point3PosX,
		Point3PosY,
		Point3PosZ,
		Point4PosX,
		Point4PosY,
		Point4PosZ,
		CamPosX,
		CamPosY,
		CamPosZ,
		CamTrgX,
		CamTrgY,
		CamTrgZ,

		Radius,
		Theta,
		Phi,
		OperationMax,
	};

	XMVECTOR resetPos;
	XMVECTOR resetRot;
	XMVECTOR resetScl;

	XMVECTOR resetCamPos;
	XMVECTOR resetCamTrg;

	XMVECTOR resetRadius;
	XMVECTOR resetTheta;
	XMVECTOR resetPhi;

	int	Cursor;

public:
	//������
	bool initialize();
	//�I��
	void terminate();

	//����
	void execute();
	//�`��
	void render();
};
