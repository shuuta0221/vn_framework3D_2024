#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�������֐�
bool SceneFieldTest::initialize()
{
	pPlayer = new vnModel(L"data/model/bear/", L"bear.vnm");

	pShadow = new vnModel(L"data/model/", L"shadow.vnm");
	pShadow->setTransparent(true);	//�������`��
	//Z Fighting����̂��ߏ�����ɂ�����
	pShadow->setPositionY(0.01f);

	pNPC = new vnModel(L"data/model/bear/", L"bear.vnm");
	pNPCShadow = new vnModel(L"data/model/", L"shadow.vnm");
	pDebugFan = new vnModel(L"data/model/", L"debug_fan.vnm");

	pNPCShadow->setTransparent(true);	//�������`��
	pDebugFan->setTransparent(true);	//�������`��
	

	pNPC->setPosition(5.0f, 0.0f, 5.0f);
	pDebugFan->setPosition(5.0f, 0.0f, 5.0f);
	pNPCShadow->setPosition(pNPC->getPosition());
	pNPCShadow->setPositionY(0.01f);

	pGround = new vnModel(L"data/model/", L"ground.vnm");
	pSky = new vnModel(L"data/model/", L"skydome.vnm");
	pSky->setLighting(false);	//���C�e�B���O����

	pHouse = new vnModel(L"data/model/", L"Cabin1.vnm");

	for (int i = 0; i < FENCE_NUM; i++) {
		pFence[i] = new vnModel(L"data/model/", L"fence.vnm");

		//�p�x
		float degree = (float)i * 360.0f / (float)FENCE_NUM;
		float radian = degree * 3.0141592f / 180;

		//���W�̌v�Z(�ɍ��W���������W)
		float x = cosf(radian) * FENCE_RADIUS;
		float z = sinf(radian) * FENCE_RADIUS;

		//�I�u�W�F�N�g�֐ݒ�
		pFence[i]->setPosition(z, 0.0f, x); //setPosition(x,y,z)����x���W��90�x�������Ă���z���W��0�x�Ȃ̂�x��z�����ւ���
		//���f���f�[�^�̓s�����x,z�����ւ���
		pFence[i]->setRotationY(radian);

	}

	pCrown = new vnModel(L"data/model/", L"crown.vnm");
	pCrown->setPositionY(2.8f);
	pCrown->setScale(0.3f, 0.3f, 0.3f);
	pCrown->setParent(pPlayer);

	//�V�[���̊��N���X�֓o�^
	for (int i = 0; i < FENCE_NUM; i++) {
		registerObject(pFence[i]);
	}
	registerObject(pPlayer);
	registerObject(pCrown);
	registerObject(pNPC);
	registerObject(pGround);
	registerObject(pSky);
	//registerObject(pHouse);
	
	
	//�������̃I�u�W�F�N�g�͕s�����̃I�u�W�F�N�g�̌�ɕ`�悷��K�v������
	registerObject(pShadow);
	registerObject(pDebugFan);
	registerObject(pNPCShadow);

	radius = 10.0f;	//���a
	theta = 0.0f; //-90.0f/180*3.141592f;	//���ʊp
	phi = 30.0f/180*3.141592f;		//�p

	return true;
}

//�I���֐�
void SceneFieldTest::terminate()
{
	//���N���X�̓o�^����폜
	
	deleteObject(pPlayer);
	deleteObject(pCrown);
	deleteObject(pShadow);
	deleteObject(pNPC);
	deleteObject(pDebugFan);
	deleteObject(pNPCShadow);
	deleteObject(pGround);
	deleteObject(pSky);
	deleteObject(pHouse);

	for (int i = 0; i < FENCE_NUM; i++) {
		deleteObject(pFence[i]);
	}
}

//�����֐�
void SceneFieldTest::execute()
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
		float rotY = atan2f(XMVectorGetX(vMove), XMVectorGetZ(vMove));	//��������������p�x

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

		/*XMVECTOR qSrc = XMQuaternionRotationRollPitchYaw(0.0f, qSrc, 0.0f);
		XMVECTOR qDst = XMQuaternionRotationRollPitchYaw(0.0f, qDst, 0.0f);*/

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


	//�t�F���X����͂ݏo���Ȃ��悤�ɍ��W�𐧌�����

	//�v���C���[�̌��_(�~�̒��S)����̋����𒲂ׂ�
	XMVECTOR vDist = XMVector3Length(*pPlayer->getPosition());	//�~�̒��S�����_�Ȃ̂�-0���ȗ�
	float dist = XMVectorGetX(vDist);

	//�������A�t�F���X�̔��a�𒴂��Ă��邩���ׂ�
	if (dist > FIELD_RADIUS) {
		//�����Ă����ꍇ�A
		//�v���C���[�̈ʒu(���W)�𔼌a���Ɏ��܂�Ƃ���ɐݒ�

		//���K��(������1�ɂ���)
		XMVECTOR v = XMVector3Normalize(*pPlayer->getPosition());

		//���l�������āA�ݒ肵���������̃x�N�g�������
		v *= FIELD_RADIUS;
		//v = XMVectorScale(v,FIELD_RSDIUS);

		//���a�̓����ɍĐ݌v���ꂽ���W�Ƀv���C���[���ړ�
		pPlayer->setPosition(&v);
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


	//NPC�̔F���͈�
	//�@�v���C���[�Ƃ̋�����10�ȓ�
	XMVECTOR NpcToPlayer = *pPlayer->getPosition() - *pNPC->getPosition();
	//XMVectorSubtract(*pPlayer->getPosition(), *pNPC->getPosition());
	vDist = XMVector3Length(NpcToPlayer);
	dist = XMVectorGetX(vDist);
	
	//�A�v���C���[�ւ̊p�x���O���}45���ȓ�
	//1.NPC�̑O���x�N�g��
	XMMATRIX NpcWorld = *pNPC->getWorld();	//���[���h�}�g���N�X���擾
	XMVECTOR NpcFront = NpcWorld.r[2];	//���[���h�}�g���N�X��3�s�ڂ��O�����x�N�g��
	//2.�v���C���[�֌������x�N�g��
	//NpcToPlayer

	//���K��(���ϒl��cos�ƂɂȂ�悤��)
	NpcFront = XMVector3Normalize(NpcFront);
	NpcToPlayer = XMVector3Normalize(NpcToPlayer);

	//����
	XMVECTOR vDot = XMVector3Dot(NpcFront, NpcToPlayer);
	float dot = XMVectorGetX(vDot);	//cos��
	if (dot > 1.0f)dot = 1.0f;	//�G���[�`�F�b�N(float�̃m�C�Y�J�b�g)
	float radian = acosf(dot);
	float degree = XMConvertToDegrees(radian);

	if (dist < 10.0f && degree < 45.0f)
	{	//�F���͈͂ɓ����Ă���

		//�i�s����(�v���C���[�̕���)�Ɍ�����
		float x = XMVectorGetX(NpcToPlayer);
		float z = XMVectorGetZ(NpcToPlayer);
		float rotY = atan2f(x, z);
		pNPC->setRotationY(rotY);

		//�v���C���[�Ɍ����Ĉړ�������(�v���C���[�̈ړ����x�̔���)
		NpcToPlayer *= speed * 0.5f;
		pNPC->addPosition(&NpcToPlayer);

		pNPCShadow->setPosition(pNPC->getPosition());
		pNPCShadow->setPositionY(0.01f);
		pDebugFan->setPosition(pNPC->getPosition());
		pDebugFan->setRotationY(pNPC->getRotationY());
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
	vnFont::print(20.0f, 120.0f, L"NPC�ƃv���C���[�̋��� : %.3f", dist);
	vnFont::print(20.0f, 140.0f, L"NPC�ƃv���C���[�̊p�x : %.3f", degree);

	vnFont::print(20.0f, 160.0f, L"Crown Local Pos : %.3f, %.3f, %.3f", pCrown->getPositionX(), pCrown->getPositionY(), pCrown->getPositionZ());

	XMVECTOR vv;
	pCrown->getWorldPosition(&vv);

	vnFont::print(20.0f, 180.0f, L"Crown Local Pos : %.3f, %.3f, %.3f", pCrown->getPositionX(), pCrown->getPositionY(), pCrown->getPositionZ());

	vnScene::execute();
}

//�`��֐�
void SceneFieldTest::render()
{
	vnScene::render();
}
