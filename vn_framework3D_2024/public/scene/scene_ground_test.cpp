#include "../../framework.h"
#include "../../framework/vn_environment.h"

//初期化関数
bool SceneGroundTest::initialize()
{
	pPlayer = new vnModel(L"data/model/bear/", L"bear.vnm");

	pShadow = new vnModel(L"data/model/", L"shadow.vnm");
	pShadow->setTransparent(true);	//半透明描画
	//Z Fighting回避のため少し上にあげる
	pShadow->setPositionY(0.01f);

	//今のプログラムではpGroundのRotation,Position,Scaleを変更すると当たり判定がおかしくなる
	//pGround = new vnModel(L"data/model/", L"noised_ground.vnm");
	pGround = new vnModel(L"data/model/", L"noised_ground.vnm");
	//pGround->setScale(2.0f, 2.0f, 2.0);
	pSky = new vnModel(L"data/model/", L"skydome.vnm");
	pSky->setLighting(false);	//ライティング無


	//シーンの基底クラスへ登録
	registerObject(pPlayer);
	registerObject(pGround);
	registerObject(pSky);
	
	
	//半透明のオブジェクトは不透明のオブジェクトの後に描画する必要がある
	registerObject(pShadow);

	radius = 10.0f;	//半径
	theta = 0.0f; //-90.0f/180*3.141592f;	//平面角
	phi = 30.0f/180*3.141592f;		//仰角

	return true;
}

//終了関数
void SceneGroundTest::terminate()
{
	//基底クラスの登録から削除
	
	deleteObject(pPlayer);
	deleteObject(pShadow);
	deleteObject(pGround);
	deleteObject(pSky);
}

//処理関数
void SceneGroundTest::execute()
{
	//入力があるか示すフラグ
	bool input = false;

	//移動スピード
	float speed = 0.1f;

	//移動ベクトル
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//移動
	if (vnKeyboard::on(DIK_W))
	{
		vMove = XMVectorSetZ(vMove, 1.0f);
		input = true;
		//pPlayer->addPositionZ(0.1f);
	}
	if (vnKeyboard::on(DIK_S))
	{
		vMove = XMVectorSetZ(vMove, -1.0f);
		input = true;
		//pPlayer->addPositionZ(-0.1f);
	}
	if (vnKeyboard::on(DIK_A))
	{
		vMove = XMVectorSetX(vMove, -1.0f);
		input = true;
		//pPlayer->addPositionX(-0.1f);
	}
	if (vnKeyboard::on(DIK_D))
	{
		vMove = XMVectorSetX(vMove, 1.0f);
		input = true;
		//pPlayer->addPositionX(0.1f);
	}

	//移動ベクトルにスピードを適用(長さを変える)
	vMove = XMVectorScale(vMove, speed);
	//vMove = vMove*speed;
	//vMove *= speed;

	//視線ベクトルを計算
	XMVECTOR cam_pos = *vnCamera::getPosition();
	XMVECTOR cam_trg = *vnCamera::getTarget();
	XMVECTOR eye = XMVectorSubtract(cam_trg,cam_pos);
	//XMVECTOR eye = cam_trg - cam_pos;

	//Y軸周りの回転値を計算
	float cam_rotY = atan2f(XMVectorGetX(eye),XMVectorGetZ(eye));

	//カメラのY軸周り(theta)の回転値
	XMMATRIX mRotY = XMMatrixRotationY(cam_rotY/*theta*/);
	//移動ベクトルを回転させる
	vMove = XMVector3TransformNormal(vMove, mRotY);

	//移動したときのみ処理
	if (input == true)
	{
		//進行方向(方向ベクトル:vMove.x,)
		float rotY = atan2f(XMVectorGetX(vMove), XMVectorGetZ(vMove));
		//元の角度
		float srcY = pPlayer->getRotationY();
		//先の角度
		float dstY = rotY;
		//オイラー角からマトリクスに変換
		XMMATRIX mSrc = XMMatrixRotationRollPitchYaw(0.0f, srcY, 0.0f);
		XMMATRIX mDst = XMMatrixRotationRollPitchYaw(0.0f, dstY, 0.0f);
		//マトリクスからクォータニオン
		XMVECTOR qSrc = XMQuaternionRotationMatrix(mSrc);
		XMVECTOR qDst = XMQuaternionRotationMatrix(mDst);
		//球面線形保管で中間の姿勢を計算
		XMVECTOR qAns = XMQuaternionSlerp(qSrc, qDst, 0.1f);
		//マトリクスに変換
		XMMATRIX mAns = XMMatrixRotationQuaternion(qAns);
		//マトリクスからY軸回転値を計算(Y軸回転のみの場合)
		rotY = atan2f(-XMVectorGetZ(mAns.r[0]), XMVectorGetZ(mAns.r[2]));

		pPlayer->setRotationY(rotY);
	}


	//移動ベクトルをプレイヤーの座標に加算
	pPlayer->addPosition(&vMove);

	pShadow->setPosition(pPlayer->getPosition());	//影を追従させる
	pShadow->setPositionY(0.01f);


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

	//カメラ座標の計算
	//回転していない基準となる軸のベクトル
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
	//回転マトリクスを作成
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi,theta,0.0f); //x,y,zそれぞれ何度回すか

	//基準軸(v)のベクトルを回転させる(回転マトリクスを乗算する)
	v = XMVector3TransformNormal(v, rotate);


	XMVECTOR CamPos = v; //XMVectorSet(px, py, pz, 0.0f);

	//カメラの座標にプレイヤーの座標を加算
	CamPos = XMVectorAdd(CamPos, *pPlayer->getPosition());//関数で書いたときの足し算
	//CamPos = CamPos + (*pPlayer->getPosition());
	//CamPos += (*pPlayer->getPosition());

	//少しだけ上に上げる
	XMVECTOR CamBias = XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f);
	CamPos = XMVectorAdd(CamPos,CamBias);

	//カメラの座標
	vnCamera::setPosition(&CamPos);	//FPS視点setTarget

	//カメラの注視点をプレイヤーの座標に設定
	XMVECTOR CamTrg = *pPlayer->getPosition();
	CamTrg = XMVectorAdd(CamTrg, CamBias);
	vnCamera::setTarget(&CamTrg); //FPS視点setPosition


	//モデルデータから三角形を取得
	int vnum = pGround->getVertexNum();//365
	int inum = pGround->getIndexNum();//1944 648

	vnVertex3D* vtx = pGround->getVertex();//頂点配列
	unsigned short* idx = pGround->getIndex();//インデックス

	XMMATRIX world = *pGround->getWorld(); //ワールドマトリクス

	for (int i = 0; i < inum; i += 3) {
		//三角形の頂点座標(ローカル座標)
		XMVECTOR v1 = XMVectorSet(
			vtx[idx[i + 0]].x,
			vtx[idx[i + 0]].y,
			vtx[idx[i + 0]].z,
			0.0f
		);
		XMVECTOR v2 = XMVectorSet(
			vtx[idx[i + 1]].x,
			vtx[idx[i + 1]].y,
			vtx[idx[i + 1]].z,
			0.0f
		);
		XMVECTOR v3 = XMVectorSet(
			vtx[idx[i + 2]].x,
			vtx[idx[i + 2]].y,
			vtx[idx[i + 2]].z,
			0.0f
		);

		//ワールドマトリクスを適用
		v1 = XMVector3TransformCoord(v1, world);
		v2 = XMVector3TransformCoord(v2, world);
		v3 = XMVector3TransformCoord(v3, world);

		//直線の情報作成
		LinePos = *pPlayer->getPosition();
		LineDir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

		//平面の情報作成
		XMVECTOR w1, w2;
		w1 = v2 - v1;
		w2 = v3 - v1;
		PlaneNormal = XMVector3Cross(w1, w2);
		PlaneNormal = XMVector3Normalize(PlaneNormal);

		XMVECTOR vDot = XMVector3Dot(PlaneNormal, v1);
		PlaneDistance = -XMVectorGetX(vDot);

		//直線と平面が平行か調べる(直線と平面の法線が垂直)
		float w = XMVectorGetX(XMVector3Dot(LineDir, PlaneNormal));
		if (w == 0.0f)continue;	//衝突座標を求める計算をしない

		//直線と平面の衝突座標を求める
		float d, s;
		XMVECTOR vS = XMVector3Dot(PlaneNormal, LinePos);
		s = XMVectorGetX(vS);
		s += PlaneDistance;
		XMVECTOR vD = XMVector3Dot(PlaneNormal, LinePos + LineDir);
		d = XMVectorGetX(vD);
		d += PlaneDistance;

		XMVECTOR hit = LinePos + LineDir * (s / (s - d));

		//衝突座標が三角形の内側にあるかどうか
		XMVECTOR e0, e1, e2;//三角形のエッジ(方向ベクトル)
		XMVECTOR h0, h1, h2;//各頂点から衝突点へのベクトル(方向ベクトル)
		XMVECTOR c0, c1, c2;//外積ベクトル
		float d0, d1, d2;//内積値
		bool including = true;//内包判定

		e0 = v2 - v1;
		h0 = hit - v1;
		c0 = XMVector3Cross(e0, h0);
		d0 = XMVectorGetX(XMVector3Dot(c0, PlaneNormal));
		if (d0 < 0.0f)including = false;

		e1 = v3 - v2;
		h1 = hit - v2;
		c1 = XMVector3Cross(e1, h1);
		d1 = XMVectorGetX(XMVector3Dot(c1, PlaneNormal));
		if (d1 < 0.0f)including = false;

		e2 = v1 - v3;
		h2 = hit - v3;
		c2 = XMVector3Cross(e2, h2);
		d2 = XMVectorGetX(XMVector3Dot(c2, PlaneNormal));
		if (d2 < 0.0f)including = false;


		if (including == true) {
			//三角形の内側にいる
			//三角形の高さにプレイヤーを合わせる
			/*if (isnan(XMVectorGetY(hit)) == true)
			{
				assert(false);
			}*/
			pPlayer->setPositionY(XMVectorGetY(hit));
			//三角形の辺をデバック描画
			vnDebugDraw::Line(&v1, &v2, 0xff0000ff);
			vnDebugDraw::Line(&v2, &v3, 0xff0000ff);
			vnDebugDraw::Line(&v3, &v1, 0xff0000ff);
		}
		else {
			//三角形の辺をデバック描画
			vnDebugDraw::Line(&v1, &v2, 0xffff0000);
			vnDebugDraw::Line(&v2, &v3, 0xffff0000);
			vnDebugDraw::Line(&v3, &v1, 0xffff0000);
		}
	}


	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	//デバッグ情報
	vnFont::print(20.0f,20.0f,L"Player : (%.3f,%.3f,%.3f)",
								pPlayer->getPositionX(),
								pPlayer->getPositionY(),
								pPlayer->getPositionZ()
								);
	vnFont::print(20.0f, 40.0f, L"Radius : %.3f", radius);
	vnFont::print(20.0f, 60.0f, L"Theta : %.3f", theta * 180/3.141592f);
	vnFont::print(20.0f, 80.0f, L"Phi : %.3f", phi * 180 / 3.141592f);
	vnFont::print(20.0f, 100.0f, L"Player_RY : %.3f", pPlayer->getRotationY()*180.0f/3.141592f);

	vnScene::execute();
}

//描画関数
void SceneGroundTest::render()
{
	vnScene::render();
}
