#include "../../framework.h"
#include "../../framework/vn_environment.h"

//初期化関数
bool SceneEffectTest::initialize()
{

	pEmitter = new vnEmitter;
	registerObject(pEmitter);

	radius = 10.0f;	//半径
	theta = 0.0f;// -90.0f / 180.0f * 3.141592f;	//平面角
	phi = 30.0f / 180.0f * 3.141592f;		//仰角

	return true;
}

//終了関数
void SceneEffectTest::terminate()
{
	deleteObject(pEmitter);
}

//処理関数
void SceneEffectTest::execute()
{

	//マウス操作でカメラ遠近
	if (vnMouse::onR())
	{
		radius += vnMouse::getDY() / 100.0f;
	}
	//Z軸
	//ノーマル
	if (vnMouse::getX() && vnMouse::onL()) {
		theta += vnMouse::getDX() / 180.0f;
	}

	//Y軸
	//ノーマル
	if (vnMouse::getY() && vnMouse::onL()) {
		phi += vnMouse::getDY() / 180.0f;
		if (phi > 80) phi = 80.0f;
		if (phi < -80) phi = -80.0f;
	}

	//カメラ座標の計算
	//回転していない基準となる軸のベクトル
	XMVECTOR comv = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
	//回転マトリクスを作成
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

	//基準軸(v)のベクトルを回転させる(回転マトリクスを乗算する
	comv = XMVector3TransformNormal(comv, rotate);

	//仰角(φ)の計算
	XMVECTOR CamPos = comv;// XMVectorSet(px, py, pz, 0.0f);

	vnCamera::setPosition(&CamPos);

	//デバック描画

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

//描画関数
void SceneEffectTest::render()
{
	vnScene::render();
}