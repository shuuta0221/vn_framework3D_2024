#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�������֐�
bool SceneGroundTest::initialize()
{
	pPlayer = new vnModel(L"data/model/bear/", L"bear.vnm");

	pShadow = new vnModel(L"data/model/", L"shadow.vnm");
	pShadow->setTransparent(true);	//�������`��
	//Z Fighting����̂��ߏ�����ɂ�����
	pShadow->setPositionY(0.01f);

	//���̃v���O�����ł�pGround��Rotation,Position,Scale��ύX����Ɠ����蔻�肪���������Ȃ�
	//pGround = new vnModel(L"data/model/", L"noised_ground.vnm");
	pGround = new vnModel(L"data/model/", L"noised_ground.vnm");
	//pGround->setScale(2.0f, 2.0f, 2.0);
	pSky = new vnModel(L"data/model/", L"skydome.vnm");
	pSky->setLighting(false);	//���C�e�B���O��


	//�V�[���̊��N���X�֓o�^
	registerObject(pPlayer);
	registerObject(pGround);
	registerObject(pSky);
	
	
	//�������̃I�u�W�F�N�g�͕s�����̃I�u�W�F�N�g�̌�ɕ`�悷��K�v������
	registerObject(pShadow);

	radius = 10.0f;	//���a
	theta = 0.0f; //-90.0f/180*3.141592f;	//���ʊp
	phi = 30.0f/180*3.141592f;		//�p

	return true;
}

//�I���֐�
void SceneGroundTest::terminate()
{
	//���N���X�̓o�^����폜
	
	deleteObject(pPlayer);
	deleteObject(pShadow);
	deleteObject(pGround);
	deleteObject(pSky);
}

//�����֐�
void SceneGroundTest::execute()
{
	//���͂����邩�����t���O
	bool input = false;

	//�ړ��X�s�[�h
	float speed = 0.1f;

	//�ړ��x�N�g��
	XMVECTOR vMove = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//�ړ�
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

	//�ړ��x�N�g���ɃX�s�[�h��K�p(������ς���)
	vMove = XMVectorScale(vMove, speed);
	//vMove = vMove*speed;
	//vMove *= speed;

	//�����x�N�g�����v�Z
	XMVECTOR cam_pos = *vnCamera::getPosition();
	XMVECTOR cam_trg = *vnCamera::getTarget();
	XMVECTOR eye = XMVectorSubtract(cam_trg,cam_pos);
	//XMVECTOR eye = cam_trg - cam_pos;

	//Y������̉�]�l���v�Z
	float cam_rotY = atan2f(XMVectorGetX(eye),XMVectorGetZ(eye));

	//�J������Y������(theta)�̉�]�l
	XMMATRIX mRotY = XMMatrixRotationY(cam_rotY/*theta*/);
	//�ړ��x�N�g������]������
	vMove = XMVector3TransformNormal(vMove, mRotY);

	//�ړ������Ƃ��̂ݏ���
	if (input == true)
	{
		//�i�s����(�����x�N�g��:vMove.x,)
		float rotY = atan2f(XMVectorGetX(vMove), XMVectorGetZ(vMove));
		//���̊p�x
		float srcY = pPlayer->getRotationY();
		//��̊p�x
		float dstY = rotY;
		//�I�C���[�p����}�g���N�X�ɕϊ�
		XMMATRIX mSrc = XMMatrixRotationRollPitchYaw(0.0f, srcY, 0.0f);
		XMMATRIX mDst = XMMatrixRotationRollPitchYaw(0.0f, dstY, 0.0f);
		//�}�g���N�X����N�H�[�^�j�I��
		XMVECTOR qSrc = XMQuaternionRotationMatrix(mSrc);
		XMVECTOR qDst = XMQuaternionRotationMatrix(mDst);
		//���ʐ��`�ۊǂŒ��Ԃ̎p�����v�Z
		XMVECTOR qAns = XMQuaternionSlerp(qSrc, qDst, 0.1f);
		//�}�g���N�X�ɕϊ�
		XMMATRIX mAns = XMMatrixRotationQuaternion(qAns);
		//�}�g���N�X����Y����]�l���v�Z(Y����]�݂̂̏ꍇ)
		rotY = atan2f(-XMVectorGetZ(mAns.r[0]), XMVectorGetZ(mAns.r[2]));

		pPlayer->setRotationY(rotY);
	}


	//�ړ��x�N�g�����v���C���[�̍��W�ɉ��Z
	pPlayer->addPosition(&vMove);

	pShadow->setPosition(pPlayer->getPosition());	//�e��Ǐ]������
	pShadow->setPositionY(0.01f);


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

	//�J�������W�̌v�Z
	//��]���Ă��Ȃ���ƂȂ鎲�̃x�N�g��
	XMVECTOR v = XMVectorSet(0.0f, 0.0f, -radius, 0.0f);
	//��]�}�g���N�X���쐬
	XMMATRIX rotate = XMMatrixRotationRollPitchYaw(phi,theta,0.0f); //x,y,z���ꂼ�ꉽ�x�񂷂�

	//���(v)�̃x�N�g������]������(��]�}�g���N�X����Z����)
	v = XMVector3TransformNormal(v, rotate);


	XMVECTOR CamPos = v; //XMVectorSet(px, py, pz, 0.0f);

	//�J�����̍��W�Ƀv���C���[�̍��W�����Z
	CamPos = XMVectorAdd(CamPos, *pPlayer->getPosition());//�֐��ŏ������Ƃ��̑����Z
	//CamPos = CamPos + (*pPlayer->getPosition());
	//CamPos += (*pPlayer->getPosition());

	//����������ɏグ��
	XMVECTOR CamBias = XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f);
	CamPos = XMVectorAdd(CamPos,CamBias);

	//�J�����̍��W
	vnCamera::setPosition(&CamPos);	//FPS���_setTarget

	//�J�����̒����_���v���C���[�̍��W�ɐݒ�
	XMVECTOR CamTrg = *pPlayer->getPosition();
	CamTrg = XMVectorAdd(CamTrg, CamBias);
	vnCamera::setTarget(&CamTrg); //FPS���_setPosition


	//���f���f�[�^����O�p�`���擾
	int vnum = pGround->getVertexNum();//365
	int inum = pGround->getIndexNum();//1944 648

	vnVertex3D* vtx = pGround->getVertex();//���_�z��
	unsigned short* idx = pGround->getIndex();//�C���f�b�N�X

	XMMATRIX world = *pGround->getWorld(); //���[���h�}�g���N�X

	for (int i = 0; i < inum; i += 3) {
		//�O�p�`�̒��_���W(���[�J�����W)
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

		//���[���h�}�g���N�X��K�p
		v1 = XMVector3TransformCoord(v1, world);
		v2 = XMVector3TransformCoord(v2, world);
		v3 = XMVector3TransformCoord(v3, world);

		//�����̏��쐬
		LinePos = *pPlayer->getPosition();
		LineDir = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);

		//���ʂ̏��쐬
		XMVECTOR w1, w2;
		w1 = v2 - v1;
		w2 = v3 - v1;
		PlaneNormal = XMVector3Cross(w1, w2);
		PlaneNormal = XMVector3Normalize(PlaneNormal);

		XMVECTOR vDot = XMVector3Dot(PlaneNormal, v1);
		PlaneDistance = -XMVectorGetX(vDot);

		//�����ƕ��ʂ����s�����ׂ�(�����ƕ��ʂ̖@��������)
		float w = XMVectorGetX(XMVector3Dot(LineDir, PlaneNormal));
		if (w == 0.0f)continue;	//�Փˍ��W�����߂�v�Z�����Ȃ�

		//�����ƕ��ʂ̏Փˍ��W�����߂�
		float d, s;
		XMVECTOR vS = XMVector3Dot(PlaneNormal, LinePos);
		s = XMVectorGetX(vS);
		s += PlaneDistance;
		XMVECTOR vD = XMVector3Dot(PlaneNormal, LinePos + LineDir);
		d = XMVectorGetX(vD);
		d += PlaneDistance;

		XMVECTOR hit = LinePos + LineDir * (s / (s - d));

		//�Փˍ��W���O�p�`�̓����ɂ��邩�ǂ���
		XMVECTOR e0, e1, e2;//�O�p�`�̃G�b�W(�����x�N�g��)
		XMVECTOR h0, h1, h2;//�e���_����Փ˓_�ւ̃x�N�g��(�����x�N�g��)
		XMVECTOR c0, c1, c2;//�O�σx�N�g��
		float d0, d1, d2;//���ϒl
		bool including = true;//�����

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
			//�O�p�`�̓����ɂ���
			//�O�p�`�̍����Ƀv���C���[�����킹��
			/*if (isnan(XMVectorGetY(hit)) == true)
			{
				assert(false);
			}*/
			pPlayer->setPositionY(XMVectorGetY(hit));
			//�O�p�`�̕ӂ��f�o�b�N�`��
			vnDebugDraw::Line(&v1, &v2, 0xff0000ff);
			vnDebugDraw::Line(&v2, &v3, 0xff0000ff);
			vnDebugDraw::Line(&v3, &v1, 0xff0000ff);
		}
		else {
			//�O�p�`�̕ӂ��f�o�b�N�`��
			vnDebugDraw::Line(&v1, &v2, 0xffff0000);
			vnDebugDraw::Line(&v2, &v3, 0xffff0000);
			vnDebugDraw::Line(&v3, &v1, 0xffff0000);
		}
	}


	vnDebugDraw::Grid();
	vnDebugDraw::Axis();

	//�f�o�b�O���
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

//�`��֐�
void SceneGroundTest::render()
{
	vnScene::render();
}
