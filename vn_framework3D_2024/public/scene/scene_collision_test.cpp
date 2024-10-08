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

	//�f�o�b�O�p��_�\�����f��
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

	//�}�E�X����ŃJ��������
	if (vnMouse::onR())
	{
		radius += vnMouse::getDY() / 100.0f;
	}
	//�}�E�X����ŃJ������]
	if (vnMouse::onL())
	{
		theta += vnMouse::getDX() / 180.0f;
		phi += vnMouse::getDY() / 180.0f;
	}

	//�J�����̉�]
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

	//���ʊp�Ƃ̌v�Z
	float tx = cosf(theta) * radius;
	float tz = sinf(theta) * radius;
	pTheta->setPositionX(tx);
	pTheta->setPositionZ(tz);

	//�p���̌v�Z
	float px = cosf(theta) * cosf(phi) * radius;
	float py = sinf(phi) * radius;
	float pz = sinf(theta) * cosf(phi) * radius;
	pPhi->setPositionX(px);
	pPhi->setPositionY(py);
	pPhi->setPositionZ(pz);

	//�J�����̍��W
	vnCamera::setPosition(px, py, pz);


	//�����̏������
	LinePos = Point[0];
	LineDir = Point[1] - Point[0];
	LineDir = XMVector3Normalize(LineDir);

	//���ʂ̏������
	XMVECTOR v1, v2;
	v1 = Point[3] - Point[2];
	v2 = Point[4] - Point[2];
	PlaneNormal = XMVector3Cross(v1, v2);
	PlaneNormal = XMVector3Normalize(PlaneNormal);

	XMVECTOR vDot = XMVector3Dot(PlaneNormal, Point[2]);
	PlaneDistance = -XMVectorGetX(vDot);//���ς���

	//�����ƕ��ʂ����s�����ׂ�(�����ƕ��ʂ̖@��������)
	float w = XMVectorGetX(XMVector3Dot(LineDir, PlaneNormal));
	if (w != 0.0f)
	{
		//�����ƕ��ʂ̏Փˍ��W�����߂�
		float s, d;
		XMVECTOR vS = XMVector3Dot(PlaneNormal, LinePos);
		s = XMVectorGetX(vS);
		s += PlaneDistance;
		XMVECTOR vD = XMVector3Dot(PlaneNormal, LinePos + LineDir);
		d = XMVectorGetX(vD);
		d += PlaneDistance;

		//�����ƕ��ʂ��Փ˂����Ƃ��̔���
		XMVECTOR hit = LinePos + LineDir * (s / (s - d));

		//�f�o�b�O�p��_�\�����f��
		pPoint->setPosition(&hit);

		XMVECTOR e0, e1, e2;	//�����x�N�g��
		XMVECTOR h0, h1, h2;	//�e���_����Փ˓_�ւ̃x�N�g��
		XMVECTOR c0, c1, c2;	//�O�σx�N�g��
		float d0, d1, d2;		//���ϒl
		bool including = true;	//�����

		//�@���_���W���m�̈����Z
		e0 = Point[3] - Point[2];
		//�A���_���W�ƏՓˍ��W�̈����Z
		h0 = hit - Point[2];
		//�T�@�ƇA�̃x�N�g���̊O�σx�N�g��
		c0 = XMVector3Cross(e0, h0);
		//�U�O�p�`(����)�̖@���x�N�g��(PlaneNormal)
		//�T�ƇU�̓��ϒl�����߂�
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

		//3�̓��ϒl���S�ăv���X�܂��͂��ׂă}�C�i�X
		//�Փˍ��W�͎O�p�`�̓����ɂ���
		if (including == true)
		{
			//�O�p�`�̓����ɓ����Ă���
			pPoint->setDiffuse(1, 0, 0, 1);//�Ԃɂ���
		}
		else
		{
			pPoint->setDiffuse(1, 1, 1, 1);//���ɂ���
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
		vnFont::print(80.0f, (float)(100 + Cursor * 16), L"��");
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

	//�摜�̂Ńo�b�O�`��
	vnDebugDraw::Line(&Point[0],&Point[1], 0xffffff00);		//�J���[��16�i����ABGR�̏��œ��͂���
	//�O�p�`�̃f�o�b�O�`��
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
