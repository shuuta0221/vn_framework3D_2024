#include "../../framework.h"
#include "../../framework/vn_environment.h"

#define MODEL_NUM_X (5)
#define MODEL_NUM_Y (5)
#define MODEL_NUM (MODEL_NUM_X*MODEL_NUM_Y)

#define MODEL_INTERVAL (2.5f)

//�������֐�
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


	radius = 10.0f;	//���a
	theta = 0.0f;// -90.0f / 180.0f * 3.141592f;	//���ʊp
	phi = 30.0f / 180.0f * 3.141592f;		//�p

	return true;
}

//�I���֐�
void ScenePickTest::terminate()
{
	for (int i = 0; i < MODEL_NUM; i++) deleteObject(pSphere[i]);
}

//�����֐�
void ScenePickTest::execute()
{
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


	//if(vnMouse::on())
	{
		vnFont::print(10.0f, 10.0f, L"�}�E�X���W(%d,%d)", vnMouse::getX(),vnMouse::getY());

		//�}�E�X���W(�X�N���[�����W)->�r���[�|�[�g���W
		float viewport_x = (float)vnMouse::getX() / (float)vnMainFrame::screenWidth * 2.0f - 1.0f;
		float viewport_y = (float)vnMouse::getY() / (float)vnMainFrame::screenHeight * -2.0f + 1.0f;

		vnFont::print(10.0f, 30.0f, L"�r���[�|�[�g���W(%d,%d)", viewport_x, viewport_y);

		//�}�E�X���W(�r���[�|�[�g���W)��Far�v���[���ɒu��()
		XMVECTOR v = XMVectorSet(viewport_x, viewport_y, 1.0f, 1.0f);

		//�r���[�|�[�g���W->���[���h���W
		XMMATRIX iVP = *vnCamera::getScreen();
		//�t�s����v�Z
		iVP = XMMatrixInverse(NULL, iVP);
		//Determint ; �s��(���l)
		//�t�s�񂪌v�Z�ł�����(�G���[�`�F�b�N)
		//�ϊ���̑̐ϔ�

		XMVECTOR wld = XMVector3Transform(v, iVP);

		vnFont::print(10.0f, 50.0f, L"���[���h���W(%3f, %3f, %3f, %3f)", XMVectorGetX(wld), XMVectorGetY(wld), XMVectorGetZ(wld), XMVectorGetW(wld));

		//w���Z(w������1�ɂ���)
		XMVECTOR w = XMVectorSplatW(wld);	// xyzw -> wwww
		wld = XMVectorDivide(wld, w);	// X = x / w,  Y = y / w,  Z = z / w,  W = w / w

		vnFont::print(10.0f, 70.0f, L"���[���h���W(%3f, %3f, %3f, %3f)", XMVectorGetX(wld), XMVectorGetY(wld), XMVectorGetZ(wld), XMVectorGetW(wld));

		LinePos = *vnCamera::getPosition();
		LineDir = wld - LinePos;
		LineDir = XMVector3Normalize(LineDir);

		for (int i = 0; i < MODEL_NUM; i++) {
			SphereCenter = *pSphere[i]->getPosition();
			SphereRadius = 0.5f;

			/*�Փ˂̌v�Z*/
			//�ŋߖT�����������������1�_�����߂�
			XMVECTOR v2 = SphereCenter - LinePos;
			XMVECTOR vDot = XMVector3Dot(v2, LineDir);
			float dot = XMVectorGetX(vDot);
				
			XMVECTOR closestPoint = LinePos + LineDir * dot;
			//pSphere[i]->setPosition(&closestPoint);	//�f�o�b�O���f��

			//���̒��S���璼���֐����ɍ~�����̃f�o�b�O�`��
			//vnDebugDraw::Line(&SphereCenter, &closestPoint, 0xffffffff);

			//�ŋߖT�����̌v�Z�ƕ\��
			XMVECTOR vDist = SphereCenter - closestPoint;
			XMVECTOR vLength = XMVector3Length(vDist);
			float dist = XMVectorGetX(vLength);

			if (dist <= SphereRadius) {
				//�ŐV�����̍X�V�`�F�b�N
				//if (dot < nearest) {
				//	nearest = dot;	//�V�����ŐV�����̊m��
				//	id = i;		//�ŋ�
				//}

				pSphere[i]->setDiffuse(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else {
				pSphere[i]->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		////�����Ƌ��̏Փˍ��W�����߂�
		//{
		//	int n = 0;	//�Փˍ��W�̐�
		//	XMVECTOR hit[2];	//�Փˍ��W

		//	XMVECTOR v = LinePos - SphereCenter;
		//	XMVECTOR nv = XMVector3Normalize(v);
		//	XMVECTOR vdt = XMVector3Dot(nv, LineDir);
		//	XMVECTOR vdl = XMVector3Length(v);
		//	
		//	float dt = XMVectorGetX(vdt);
		//	float dl = XMVectorGetX(vdl);
		//	float dr = dl * sqrtf(1.0f - dt * dt);	//�����S�ƒ����̋���
		//	
		//	if (dr <= SphereRadius)
		//	{	//�����S�ƒ����̋��������a�ȓ��Ȃ�Փ�
		//		float dq = sqrtf(SphereRadius * SphereRadius - dr * dr);
		//		float ds = -dl * dt;
		//		float dp = ds + dq;
		//		float dm = ds - dq;

		//		hit[n++] = LinePos + LineDir * dm;
		//		if (dq != 0.0f)
		//		{	//��_(�Փˍ��W)������݂���
		//			hit[n++] = LinePos + LineDir * dp;
		//		}
		//	}
		//}
	}




	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	vnScene::execute();
}

//�`��֐�
void ScenePickTest::render()
{
	vnScene::render();
}