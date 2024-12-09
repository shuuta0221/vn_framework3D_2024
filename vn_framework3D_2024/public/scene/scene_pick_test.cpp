#include "../../framework.h"
#include "../../framework/vn_environment.h"

#define MODEL_NUM_X (5)
#define MODEL_NUM_Y (5)
#define MODEL_NUM (MODEL_NUM_X*MODEL_NUM_Y)

#define MODEL_INTERVAL (2.5f)

//初期化関数
bool ScenePickTest::initialize()
{
	for (int i = 0; i < MODEL_NUM; i++) {
		pSphere[i] = new vnModel(L"data/model/primitive/", L"sphere.vnm");
		registerObject(pSphere[i]);
	}

	pSphere[0]->setPositionX(-5.0f); pSphere[0]->setPositionZ(-5.0f);
	pSphere[1]->setPositionX(-2.5f); pSphere[1]->setPositionZ(-5.0f);
	pSphere[2]->setPositionX(0.0f); pSphere[2]->setPositionZ(-5.0f);
	pSphere[3]->setPositionX(2.5f); pSphere[3]->setPositionZ(-5.0f);
	pSphere[4]->setPositionX(5.0f); pSphere[4]->setPositionZ(-5.0f);

	pSphere[5]->setPositionX(-5.0f); pSphere[5]->setPositionZ(-2.5f);
	pSphere[6]->setPositionX(-2.5f); pSphere[6]->setPositionZ(-2.5f);
	pSphere[7]->setPositionX(0.0f); pSphere[7]->setPositionZ(-2.5f);
	pSphere[8]->setPositionX(2.5f); pSphere[8]->setPositionZ(-2.5f);
	pSphere[9]->setPositionX(5.0f); pSphere[9]->setPositionZ(-2.5f);

	pSphere[10]->setPositionX(-5.0f); pSphere[10]->setPositionZ(0.0f);
	pSphere[11]->setPositionX(-2.5f); pSphere[11]->setPositionZ(0.0f);
	pSphere[12]->setPositionX(0.0f); pSphere[12]->setPositionZ(0.0f);
	pSphere[13]->setPositionX(2.5f); pSphere[13]->setPositionZ(0.0f);
	pSphere[14]->setPositionX(5.0f); pSphere[14]->setPositionZ(0.0f);

	pSphere[15]->setPositionX(-5.0f); pSphere[15]->setPositionZ(2.5f);
	pSphere[16]->setPositionX(-2.5f); pSphere[16]->setPositionZ(2.5f);
	pSphere[17]->setPositionX(0.0f); pSphere[17]->setPositionZ(2.5f);
	pSphere[18]->setPositionX(2.5f); pSphere[18]->setPositionZ(2.5f);
	pSphere[19]->setPositionX(5.0f); pSphere[19]->setPositionZ(2.5f);

	pSphere[20]->setPositionX(-5.0f); pSphere[20]->setPositionZ(5.0f);
	pSphere[21]->setPositionX(-2.5f); pSphere[21]->setPositionZ(5.0f);
	pSphere[22]->setPositionX(0.0f); pSphere[22]->setPositionZ(5.0f);
	pSphere[23]->setPositionX(2.5f); pSphere[23]->setPositionZ(5.0f);
	pSphere[24]->setPositionX(5.0f); pSphere[24]->setPositionZ(5.0f);

	//for (int i = 0; i < MODEL_NUM; i++) {
	//	if (i <= 4) {
	//		pSphere[i]->setPositionZ(-5);
	//		pSphere[i]->setPositionX(-5 + MODEL_INTERVAL);
	//	}
	//}


	radius = 10.0f;	//半径
	theta = 0.0f;// -90.0f / 180.0f * 3.141592f;	//平面角
	phi = 30.0f / 180.0f * 3.141592f;		//仰角

	return true;
}

//終了関数
void ScenePickTest::terminate()
{
	for (int i = 0; i < MODEL_NUM; i++) deleteObject(pSphere[i]);
}

//処理関数
void ScenePickTest::execute()
{
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


	//if(vnMouse::on())
	{
		vnFont::print(10.0f, 10.0f, L"マウス座標(%d,%d)", vnMouse::getX(),vnMouse::getY());

		//マウス座標(スクリーン座標)->ビューポート座標
		float viewport_x = (float)vnMouse::getX() / (float)vnMainFrame::screenWidth * 2.0f - 1.0f;
		float viewport_y = (float)vnMouse::getY() / (float)vnMainFrame::screenHeight * -2.0f + 1.0f;

		vnFont::print(10.0f, 30.0f, L"ビューポート座標(%d,%d)", viewport_x, viewport_y);

		//マウス座標(ビューポート座標)をFarプレーンに置く()
		XMVECTOR v = XMVectorSet(viewport_x, viewport_y, 1.0f, 1.0f);

		//ビューポート座標->ワールド座標
		XMMATRIX iVP = *vnCamera::getScreen();
		//逆行列を計算
		iVP = XMMatrixInverse(NULL, iVP);
		//Determint ; 行列式(数値)
		//逆行列が計算できたか(エラーチェック)
		//変換後の体積比

		XMVECTOR wld = XMVector3Transform(v, iVP);

		vnFont::print(10.0f, 50.0f, L"ワールド座標(%3f, %3f, %3f, %3f)", XMVectorGetX(wld), XMVectorGetY(wld), XMVectorGetZ(wld), XMVectorGetW(wld));

		//w除算(w成分を1にする)
		XMVECTOR w = XMVectorSplatW(wld);	// xyzw -> wwww
		wld = XMVectorDivide(wld, w);	// X = x / w,  Y = y / w,  Z = z / w,  W = w / w

		vnFont::print(10.0f, 70.0f, L"ワールド座標(%3f, %3f, %3f, %3f)", XMVectorGetX(wld), XMVectorGetY(wld), XMVectorGetZ(wld), XMVectorGetW(wld));

		LinePos = *vnCamera::getPosition();
		LineDir = wld - LinePos;
		LineDir = XMVector3Normalize(LineDir);

		for (int i = 0; i < MODEL_NUM; i++) {
			SphereCenter = *pSphere[i]->getPosition();
			SphereRadius = 0.5f;

			/*衝突の計算*/
			//最近傍距離を示す直線上の1点を求める
			XMVECTOR v2 = SphereCenter - LinePos;
			XMVECTOR vDot = XMVector3Dot(v2, LineDir);
			float dot = XMVectorGetX(vDot);
				
			XMVECTOR closestPoint = LinePos + LineDir * dot;
			//pSphere[i]->setPosition(&closestPoint);	//デバッグモデル

			//球の中心から直線へ垂直に降りる線のデバッグ描画
			//vnDebugDraw::Line(&SphereCenter, &closestPoint, 0xffffffff);

			//最近傍距離の計算と表示
			XMVECTOR vDist = SphereCenter - closestPoint;
			XMVECTOR vLength = XMVector3Length(vDist);
			float dist = XMVectorGetX(vLength);

			if (dist <= SphereRadius) {
				//最新距離の更新チェック
				//if (dot < nearest) {
				//	nearest = dot;	//新しい最新距離の確保
				//	id = i;		//最近
				//}

				pSphere[i]->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else {
				pSphere[i]->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		////直線と球の衝突座標を求める
		//{
		//	int n = 0;	//衝突座標の数
		//	XMVECTOR hit[2];	//衝突座標

		//	XMVECTOR v = LinePos - SphereCenter;
		//	XMVECTOR nv = XMVector3Normalize(v);
		//	XMVECTOR vdt = XMVector3Dot(nv, LineDir);
		//	XMVECTOR vdl = XMVector3Length(v);
		//	
		//	float dt = XMVectorGetX(vdt);
		//	float dl = XMVectorGetX(vdl);
		//	float dr = dl * sqrtf(1.0f - dt * dt);	//球中心と直線の距離
		//	
		//	if (dr <= SphereRadius)
		//	{	//球中心と直線の距離が半径以内なら衝突
		//		float dq = sqrtf(SphereRadius * SphereRadius - dr * dr);
		//		float ds = -dl * dt;
		//		float dp = ds + dq;
		//		float dm = ds - dq;

		//		hit[n++] = LinePos + LineDir * dm;
		//		if (dq != 0.0f)
		//		{	//交点(衝突座標)が二つ存在する
		//			hit[n++] = LinePos + LineDir * dp;
		//		}
		//	}
		//}
	}




	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

//描画関数
void ScenePickTest::render()
{
	vnScene::render();
}