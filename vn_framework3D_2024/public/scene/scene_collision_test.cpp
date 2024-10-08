#include "../../framework.h"
#include "../../framework/vn_environment.h"
#include "scene_collision_test.h"

bool SceneCollisionTest::initialize()
{
	Point[0] = XMVectorSet(-2.0f, 4.0f, -2.0f, 0.0f);
	Point[1] = XMVectorSet(2.0f, 4.0f, 2.0f, 0.0f);
	Point[2] = XMVectorSet(-1.0f, 0.0f, 1.0f, 0.0f);
	Point[3] = XMVectorSet(-1.0f, 0.0f, -1.0f, 0.0f);
	Point[4] = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);

	pTheta = new cCube;
	pTheta->setScaleX(0.2f);
	pTheta->setScaleY(0.2f);
	pTheta->setScaleZ(0.2f);

	pPhi = new cCube();
	pPhi->setScaleX(0.2f);
	pPhi->setScaleY(0.2f);
	pPhi->setScaleZ(0.2f);

	//デバッグ用交点表示モデル
	pPoint = new vnModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint->setScale(0.1f, 0.1f, 0.1f);
	registerObject(pPoint);

	radius = 5.0f;
	theta = 0.0f;
	phi = 0.0f;

	Cursor = 0;

	return true;
}

void SceneCollisionTest::terminate()
{
	delete pTheta;
	delete pPhi;
	delete[] Point;
	deleteObject(pPoint);
}

void SceneCollisionTest::execute()
{
	float value_t = 0.01f;
	float value_r = 1.0f * 3.141592f / 180.0f;
	float value_s = 0.01f;
	float value_p0 = 0.1f;
	float value_p1 = 0.1f;
	float value_p2 = 0.1f;
	float value_p3 = 0.1f;
	float value_p4 = 0.1f;

	//マウス操作でカメラ遠近
	if (vnMouse::onR())
	{
		radius += vnMouse::getDY() / 100.0f;
	}
	//マウス操作でカメラ回転
	if (vnMouse::onL())
	{
		theta += vnMouse::getDX() / 180.0f;
		phi += vnMouse::getDY() / 180.0f;
	}

	//カメラの回転
	if (vnKeyboard::on(DIK_UP))
	{
		phi -= 1.0f * 3.141592f / 180;
	}
	if (vnKeyboard::on(DIK_DOWN))
	{
		phi += 1.0f * 3.141592f / 180;
	}
	if (vnKeyboard::on(DIK_LEFT))
	{
		theta -= 1.0f * 3.141592f / 180;
	}
	if (vnKeyboard::on(DIK_RIGHT))
	{
		theta += 1.0f * 3.141592f / 180;
	}

	//平面角θの計算
	float tx = cosf(theta) * radius;
	float tz = sinf(theta) * radius;
	pTheta->setPositionX(tx);
	pTheta->setPositionZ(tz);

	//仰角Φの計算
	float px = cosf(theta) * cosf(phi) * radius;
	float py = sinf(phi) * radius;
	float pz = sinf(theta) * cosf(phi) * radius;
	pPhi->setPositionX(px);
	pPhi->setPositionY(py);
	pPhi->setPositionZ(pz);

	//カメラの座標
	vnCamera::setPosition(px, py, pz);


	//直線の情報を作る
	LinePos = Point[0];
	LineDir = Point[1] - Point[0];
	LineDir = XMVector3Normalize(LineDir);

	//平面の情報を作る
	XMVECTOR v1, v2;
	v1 = Point[3] - Point[2];
	v2 = Point[4] - Point[2];
	PlaneNormal = XMVector3Cross(v1, v2);
	PlaneNormal = XMVector3Normalize(PlaneNormal);

	XMVECTOR vDot = XMVector3Dot(PlaneNormal, Point[2]);
	PlaneDistance = -XMVectorGetX(vDot);//内積を代入

	//直線と平面が平行か調べる(直線と平面の法線が垂直)
	float w = XMVectorGetX(XMVector3Dot(LineDir, PlaneNormal));
	if (w != 0.0f)
	{
		//直線と平面の衝突座標を求める
		float s, d;
		XMVECTOR vS = XMVector3Dot(PlaneNormal, LinePos);
		s = XMVectorGetX(vS);
		s += PlaneDistance;
		XMVECTOR vD = XMVector3Dot(PlaneNormal, LinePos + LineDir);
		d = XMVectorGetX(vD);
		d += PlaneDistance;

		//直線と平面が衝突したときの判定
		XMVECTOR hit = LinePos + LineDir * (s / (s - d));

		//デバッグ用交点表示モデル
		pPoint->setPosition(&hit);

		XMVECTOR e0, e1, e2;	//方向ベクトル
		XMVECTOR h0, h1, h2;	//各頂点から衝突点へのベクトル
		XMVECTOR c0, c1, c2;	//外積ベクトル
		float d0, d1, d2;		//内積値
		bool including = true;	//内包判定

		//①頂点座標同士の引き算
		e0 = Point[3] - Point[2];
		//②頂点座標と衝突座標の引き算
		h0 = hit - Point[2];
		//Ⅰ①と②のベクトルの外積ベクトル
		c0 = XMVector3Cross(e0, h0);
		//Ⅱ三角形(平面)の法線ベクトル(PlaneNormal)
		//ⅠとⅡの内積値を求める
		XMVECTOR v_Dot0 = XMVector3Dot(c0, PlaneNormal);
		d0 = XMVectorGetX(v_Dot0);
		if (d0 < 0.0f)including = false;

		e1 = Point[4] - Point[3];
		h1 = hit - Point[3];
		c1 = XMVector3Cross(e1, h1);
		XMVECTOR v_Dot1 = XMVector3Dot(c1, PlaneNormal);
		d1 = XMVectorGetX(v_Dot1);
		if (d1 < 0.0f)including = false;

		e2 = Point[2] - Point[4];
		h2 = hit - Point[4];
		c2 = XMVector3Cross(e2, h2);
		XMVECTOR v_Dot2 = XMVector3Dot(c2, PlaneNormal);
		d2 = XMVectorGetX(v_Dot2);
		if (d2 < 0.0f)including = false;

		//3つの内積値が全てプラスまたはすべてマイナス
		//衝突座標は三角形の内側にある
		if (including == true)
		{
			//三角形の内側に入っている
			pPoint->setDiffuse(1, 0, 0, 1);//赤にする
		}
		else
		{
			pPoint->setDiffuse(1, 1, 1, 1);//白にする
		}



		if (vnKeyboard::trg(DIK_UP) && --Cursor < 0)
		{
			Cursor = OperationMax - 1;
		}
		else if (vnKeyboard::trg(DIK_DOWN) && ++Cursor >= OperationMax)
		{
			Cursor = 0;
		}
		else if (vnKeyboard::on(DIK_RIGHT))
		{
			switch (Cursor)
			{
			case Point0PosX: Point[0] = XMVectorSetX(Point[0], value_p0);
			case Point0PosY: Point[0] = XMVectorSetY(Point[0], value_p0);
			case Point0PosZ: Point[0] = XMVectorSetZ(Point[0], value_p0);
			}
		}



		int line = 0;
		vnFont::print(80.0f, (float)(100 + Cursor * 16), L"→");
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"Point0PosX : %.3f", XMVectorGetX(Point[0]));
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"CamPosX : %.3f", vnCamera::getPositionX());
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"CamPosY : %.3f", vnCamera::getPositionY());
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"CamPosZ : %.3f", vnCamera::getPositionZ());
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"CamTrgX : %.3f", vnCamera::getTargetX());
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"CamTrgY : %.3f", vnCamera::getTargetY());
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"CamTrgZ : %.3f", vnCamera::getTargetZ());
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"Radius : %.3f", radius);
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"Theta : %.3f", theta);
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"Phi : %.3f", phi);
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"d0 : %.3f", d0);
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"d1 : %.3f", d1);
		vnFont::print(100.0f, 100.0f + (float)(line++ * 16), L"d2 : %.3f", d2);
	}

	//画像のでバッグ描画
	vnDebugDraw::Line(&Point[0],&Point[1], 0xffffff00);		//カラーは16進数のABGRの順で入力する
	//三角形のデバッグ描画
	vnDebugDraw::Line(&Point[2], &Point[3], 0xffff00ff);
	vnDebugDraw::Line(&Point[3], &Point[4], 0xffff00ff);
	vnDebugDraw::Line(&Point[4], &Point[2], 0xffff00ff);
	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

void SceneCollisionTest::render()
{
	pTheta->render();
	pPhi->render();

	vnScene::render();
}
