#pragma once


class SceneGroundTest : public vnScene
{
private:
	vnModel* pPlayer;
	vnModel* pShadow;
	
	vnModel* pGround;
	vnModel* pSky;

	//重力情報
	XMVECTOR velocity;
	XMVECTOR gravity;

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
