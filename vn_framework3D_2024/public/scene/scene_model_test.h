#pragma once


class SceneModelTest : public vnScene
{
private:
	vnModel* pModel;

	//�ɍ��W�̏��
	float radius;	//���a
	float theta;	//���ʊp
	float phi;		//�p

	//����̎��
	enum eOperation
	{
		PositionX,
		PositionY,
		PositionZ,
		RotateX,
		RotateY,
		RotateZ,
		ScaleX,
		ScaleY,
		ScaleZ,
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

	

	int	Cursor;

public:
	
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
