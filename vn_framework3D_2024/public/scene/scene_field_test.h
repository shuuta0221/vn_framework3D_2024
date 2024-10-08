#pragma once

#define FENCE_NUM		(32)	//�t�F���X���f��

#define FENCE_RADIUS	(37.5f)	//�t�F���X��z�u����~���̔��a

#define FIELD_RADIUS	(36.5f)	//�t�F���X�̓����蔻������~���̔��a

class SceneFieldTest : public vnScene
{
private:
	vnModel* pFence[FENCE_NUM];

	vnModel* pPlayer;
	vnModel* pShadow;

	vnModel* pNPC;
	vnModel* pNPCShadow;
	vnModel* pDebugFan;

	vnModel* pGround;
	vnModel* pSky;
	vnModel* pHouse;


	//�ɍ��W�̏��
	float radius;	//���a
	float theta;	//���ʊp
	float phi;		//�p

public:
	//������
	bool initialize();
	//�I��
	void terminate();

	//����
	void execute();
	//�`��
	void render();
};
