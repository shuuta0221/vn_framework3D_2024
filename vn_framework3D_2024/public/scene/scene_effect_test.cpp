#include "../../framework.h"
#include "../../framework/vn_environment.h"

//‰Šú‰»ŠÖ”
bool SceneEffectTest::initialize()
{

	pEmitter = new vnEmitter;
	registerObject(pEmitter);

	radius = 10.0f;	//”¼Œa
	theta = 0.0f;// -90.0f / 180.0f * 3.141592f;	//•½–ÊŠp
	phi = 30.0f / 180.0f * 3.141592f;		//‹ÂŠp

	return true;
}

//I—¹ŠÖ”
void SceneEffectTest::terminate()
{
	deleteObject(pEmitter);
}

//ˆ—ŠÖ”
void SceneEffectTest::execute()
{

	//ƒ}ƒEƒX‘€ì‚ÅƒJƒƒ‰‰“‹ß
	if (vnMouse::onR())
	{
		radius += vnMouse::getDY() / 100.0f;
	}
	//ZŽ²
	//ƒm[ƒ}ƒ‹
	if (vnMouse::getX() && vnMouse::onL()) {
		theta += vnMouse::getDX() / 180.0f;
	}

	//YŽ²
	//ƒm[ƒ}ƒ‹
	if (vnMouse::getY() && vnMouse::onL()) {
		phi += vnMouse::getDY() / 180.0f;
		if (phi > 80) phi = 80.0f;
		if (phi < -80) phi = -80.0f;
	}

	//ƒJƒƒ‰À•W‚ÌŒvŽZ
	//‰ñ“]‚µ‚Ä‚¢‚È‚¢Šî€‚Æ‚È‚éŽ²‚ÌƒxƒNƒgƒ‹
	XMVECTOR comv = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
	//‰ñ“]ƒ}ƒgƒŠƒNƒX‚ðì¬
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

	//Šî€Ž²(v)‚ÌƒxƒNƒgƒ‹‚ð‰ñ“]‚³‚¹‚é(‰ñ“]ƒ}ƒgƒŠƒNƒX‚ðæŽZ‚·‚é
	comv = XMVector3TransformNormal(comv, rotate);

	//‹ÂŠp(ƒÓ)‚ÌŒvŽZ
	XMVECTOR CamPos = comv;// XMVectorSet(px, py, pz, 0.0f);

	vnCamera::setPosition(&CamPos);

	//ƒfƒoƒbƒN•`‰æ

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

//•`‰æŠÖ”
void SceneEffectTest::render()
{
	vnScene::render();
}