#include "../../framework.h"
#include "../../framework/vn_environment.h"

//初期化関数
bool SceneCollisionTest2::initialize()
{
	pSphere=new vnModel(L"data/model/primitive/", L"sphere.vnm");
	registerObject(pSphere);

	Point[0] = XMVectorSet(-2.0f, -3.0f, -2.0f, 0.0f);
	Point[1] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	Point[2] = XMVectorSet(2.0f, 3.0f, 2.0f, 0.0f);
	Point[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 2; i++) {
		pHit[i] = new vnModel(L"data/model/primitive/", L"sphere.vnm");
		registerObject(pHit[i]);
		pHit[i]->setRenderEnable(false);
	}

	//デバッグ用交点表示モデル
	pPoint = new vnModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint->setScale(0.1f, 0.1f, 0.1f);
	registerObject(pPoint);

	radius = 10.0f;	//半径
	theta = 0.0f;// -90.0f / 180.0f * 3.141592f;	//平面角
	phi = 30.0f / 180.0f * 3.141592f;		//仰角

	return true;
}

//終了関数
void SceneCollisionTest2::terminate()
{
	deleteObject(pPoint);
	deleteObject(pSphere);
}

//処理関数
void SceneCollisionTest2::execute()
{
	//入力があるか示すフラグ
	bool input = false;
	//移動スピード
	float Speed = 0.1f;
	//移動ベクトル
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//移動入力
	if (vnKeyboard::on(DIK_W)) {
		vMove = XMVectorSetZ(vMove, 1.0f);
		input = true;
	}
	if (vnKeyboard::on(DIK_S)) {
		vMove = XMVectorSetZ(vMove, -1.0f);
		input = true;
	}
	if (vnKeyboard::on(DIK_A)) {
		vMove = XMVectorSetX(vMove, -1.0f);
		input = true;
	}
	if (vnKeyboard::on(DIK_D)) {
		vMove = XMVectorSetX(vMove, 1.0f);
		input = true;
	}
	//移動ベクトルにスピードを適用(長さを変える)
	vMove = XMVectorScale(vMove, Speed);

	//移動ベクトルをpPointに適応
	pPoint->addPosition(&vMove);

	//直線のデバッグ描画					0xAABBGGRR
	vnDebugDraw::Line(&Point[0], &Point[1], 0xffffff00);	//水色
	vnDebugDraw::Line(&Point[1], &Point[2], 0xffff00ff);	//紫
	vnDebugDraw::Line(&Point[0], &Point[3], 0xffff00ff);	//紫

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

	pSphere->setPosition(&Point[2]);
	pSphere->setPosition(&Point[3]);

	//直線の情報(自分の環境から設定)
	LinePos = Point[0];
	LineDir = Point[1] - Point[0];
	LineDir = XMVector3Normalize(LineDir);

	//球の情報(自分の環境から設定)
	SphereCenter = Point[2];
	SphereRadius = XMVectorGetX(Point[3]);

	{
		//最近傍距離を示す直線上の1点を求める
		XMVECTOR v = SphereCenter - LinePos;
		XMVECTOR vDot = XMVector3Dot(v, LineDir);
		float dot = XMVectorGetX(vDot);

		XMVECTOR closestPoint = LinePos + LineDir * dot;
		pPoint->setPosition(&closestPoint);	//デバッグモデル

		//球の中心から直線へ垂直に降りる線のデバッグ描画
		vnDebugDraw::Line(&SphereCenter, &closestPoint, 0xffffffff);

		//最近傍距離の計算と表示
		XMVECTOR vDist = SphereCenter - closestPoint;
		vDist = XMVector3Length(vDist);
		float dist = XMVectorGetX(vDist);

		vnFont::print(10.0f, 300.0f, L"直線と球中心の距離 : %f", dist);

		if (dist <= SphereRadius)
		{
			vnFont::print(10.0f, 320.0f, L"当たっている");
		}
		else
		{
			vnFont::print(10.0f, 320.0f, L"当たっていない");
		}
	}



	//直線と球の衝突座標を求める
	{
		int n = 0;	//衝突座標の数
		XMVECTOR hit[2];	//衝突座標
		XMVECTOR v = LinePos - SphereCenter;
		XMVECTOR nv = XMVector3Normalize(v);
		XMVECTOR vdt = XMVector3Dot(nv, LineDir);
		XMVECTOR vdl = XMVector3Length(v);
		float dt = XMVectorGetX(vdt);
		float dl = XMVectorGetX(vdl);
		float dr = dl * sqrtf(1.0f - dt * dt);	//球中心と直線の距離
		if (dr <= SphereRadius)
		{	//球中心と直線の距離が半径以内なら衝突
			float dq = sqrtf(SphereRadius * SphereRadius - dr * dr);
			float ds = -dl * dt;
			float dp = ds + dq;
			float dm = ds - dq;

			hit[n++] = LinePos + LineDir * dm;
			if (dq != 0.0f)
			{	//交点(衝突座標)が二つ存在する
				hit[n++] = LinePos + LineDir * dp;
			}
		}

		//デバッグ表示
		for (int i = 0; i < 2; i++)
		{
			if (i < n)
			{
				pHit[i]->setPosition(&hit[i]);
				pHit[i]->setRenderEnable(true);
			}
			else
			{
				pHit[i]->setRenderEnable(false);
			}
		}

	}


	//デバック描画

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

//描画関数
void SceneCollisionTest2::render()
{

	vnScene::render();
}