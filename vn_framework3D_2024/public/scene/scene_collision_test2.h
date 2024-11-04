#pragma once

class SceneCollisionTest2 : public vnScene
{
private:

	XMVECTOR Point[5];

	vnModel* pSphere;
	vnModel* pHit[2];

	//デバッグ用モデル(最近傍距離を求める直線上の1点)
	vnModel* pPoint;

	//直線の情報
	XMVECTOR LinePos;
	XMVECTOR LineDir;

	//球の情報
	XMVECTOR SphereCenter;
	float	 SphereRadius;

	//平面の情報
	//平面方程式 ax + by + cz = d
	XMVECTOR PlaneNormal;	//(a,b,c)
	float PlaneDistance;	//d

	//極座標の情報
	float radius;	//半径
	float theta;	//平面角
	float phi;		//仰角

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