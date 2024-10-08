#pragma once

#include "../object/cube.h"

class SceneCollisionTest : public vnScene
{
private:
	cCube* pCube;

	cCube* pTheta;
	cCube* pPhi;


	XMVECTOR Point[5];

	//デバッグ用交点表示モデル
	vnModel* pPoint;

	//直線の情報
	XMVECTOR LinePos;
	XMVECTOR LineDir;

	//平面の情報
	//平面方程式 ax + by + cz = d
	XMVECTOR PlaneNormal;	//(a,b,c)
	float PlaneDistance;	//d

	//極座標の情報
	float radius;	//半径
	float theta;	//平面角
	float phi;		//仰角

	//操作の種類
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
	//初期化
	bool initialize();
	//終了
	void terminate();

	//処理
	void execute();
	//描画
	void render();
};
