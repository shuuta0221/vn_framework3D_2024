#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�������֐�
bool SceneEffectTest::initialize()
{

	pEmitter = new vnEmitter;
	registerObject(pEmitter);

	radius = 10.0f;	//���a
	theta = 0.0f;// -90.0f / 180.0f * 3.141592f;	//���ʊp
	phi = 30.0f / 180.0f * 3.141592f;		//�p

	return true;
}

//�I���֐�
void SceneEffectTest::terminate()
{
	deleteObject(pEmitter);
}

//�����֐�
void SceneEffectTest::execute()
{

	//�}�E�X����ŃJ��������
	if (vnMouse::onR())
	{
		radius += vnMouse::getDY() / 100.0f;
	}
	//Z��
	//�m�[�}��
	if (vnMouse::getX() && vnMouse::onL()) {
		theta += vnMouse::getDX() / 180.0f;
	}

	//Y��
	//�m�[�}��
	if (vnMouse::getY() && vnMouse::onL()) {
		phi += vnMouse::getDY() / 180.0f;
		if (phi > 80) phi = 80.0f;
		if (phi < -80) phi = -80.0f;
	}

	//�J�������W�̌v�Z
	//��]���Ă��Ȃ���ƂȂ鎲�̃x�N�g��
	XMVECTOR comv = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
	//��]�}�g���N�X���쐬
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi, theta, 0.0f);

	//���(v)�̃x�N�g������]������(��]�}�g���N�X����Z����
	comv = XMVector3TransformNormal(comv, rotate);

	//�p(��)�̌v�Z
	XMVECTOR CamPos = comv;// XMVectorSet(px, py, pz, 0.0f);

	vnCamera::setPosition(&CamPos);

	//�f�o�b�N�`��

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

//�`��֐�
void SceneEffectTest::render()
{
	vnScene::render();
}