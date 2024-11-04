#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�������֐�
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

	//�f�o�b�O�p��_�\�����f��
	pPoint = new vnModel(L"data/model/primitive/", L"sphere.vnm");
	pPoint->setScale(0.1f, 0.1f, 0.1f);
	registerObject(pPoint);

	radius = 10.0f;	//���a
	theta = 0.0f;// -90.0f / 180.0f * 3.141592f;	//���ʊp
	phi = 30.0f / 180.0f * 3.141592f;		//�p

	return true;
}

//�I���֐�
void SceneCollisionTest2::terminate()
{
	deleteObject(pPoint);
	deleteObject(pSphere);
}

//�����֐�
void SceneCollisionTest2::execute()
{
	//���͂����邩�����t���O
	bool input = false;
	//�ړ��X�s�[�h
	float Speed = 0.1f;
	//�ړ��x�N�g��
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//�ړ�����
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
	//�ړ��x�N�g���ɃX�s�[�h��K�p(������ς���)
	vMove = XMVectorScale(vMove, Speed);

	//�ړ��x�N�g����pPoint�ɓK��
	pPoint->addPosition(&vMove);

	//�����̃f�o�b�O�`��					0xAABBGGRR
	vnDebugDraw::Line(&Point[0], &Point[1], 0xffffff00);	//���F
	vnDebugDraw::Line(&Point[1], &Point[2], 0xffff00ff);	//��
	vnDebugDraw::Line(&Point[0], &Point[3], 0xffff00ff);	//��

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

	pSphere->setPosition(&Point[2]);
	pSphere->setPosition(&Point[3]);

	//�����̏��(�����̊�����ݒ�)
	LinePos = Point[0];
	LineDir = Point[1] - Point[0];
	LineDir = XMVector3Normalize(LineDir);

	//���̏��(�����̊�����ݒ�)
	SphereCenter = Point[2];
	SphereRadius = XMVectorGetX(Point[3]);

	{
		//�ŋߖT�����������������1�_�����߂�
		XMVECTOR v = SphereCenter - LinePos;
		XMVECTOR vDot = XMVector3Dot(v, LineDir);
		float dot = XMVectorGetX(vDot);

		XMVECTOR closestPoint = LinePos + LineDir * dot;
		pPoint->setPosition(&closestPoint);	//�f�o�b�O���f��

		//���̒��S���璼���֐����ɍ~�����̃f�o�b�O�`��
		vnDebugDraw::Line(&SphereCenter, &closestPoint, 0xffffffff);

		//�ŋߖT�����̌v�Z�ƕ\��
		XMVECTOR vDist = SphereCenter - closestPoint;
		vDist = XMVector3Length(vDist);
		float dist = XMVectorGetX(vDist);

		vnFont::print(10.0f, 300.0f, L"�����Ƌ����S�̋��� : %f", dist);

		if (dist <= SphereRadius)
		{
			vnFont::print(10.0f, 320.0f, L"�������Ă���");
		}
		else
		{
			vnFont::print(10.0f, 320.0f, L"�������Ă��Ȃ�");
		}
	}



	//�����Ƌ��̏Փˍ��W�����߂�
	{
		int n = 0;	//�Փˍ��W�̐�
		XMVECTOR hit[2];	//�Փˍ��W
		XMVECTOR v = LinePos - SphereCenter;
		XMVECTOR nv = XMVector3Normalize(v);
		XMVECTOR vdt = XMVector3Dot(nv, LineDir);
		XMVECTOR vdl = XMVector3Length(v);
		float dt = XMVectorGetX(vdt);
		float dl = XMVectorGetX(vdl);
		float dr = dl * sqrtf(1.0f - dt * dt);	//�����S�ƒ����̋���
		if (dr <= SphereRadius)
		{	//�����S�ƒ����̋��������a�ȓ��Ȃ�Փ�
			float dq = sqrtf(SphereRadius * SphereRadius - dr * dr);
			float ds = -dl * dt;
			float dp = ds + dq;
			float dm = ds - dq;

			hit[n++] = LinePos + LineDir * dm;
			if (dq != 0.0f)
			{	//��_(�Փˍ��W)������݂���
				hit[n++] = LinePos + LineDir * dp;
			}
		}

		//�f�o�b�O�\��
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


	//�f�o�b�N�`��

	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

//�`��֐�
void SceneCollisionTest2::render()
{

	vnScene::render();
}