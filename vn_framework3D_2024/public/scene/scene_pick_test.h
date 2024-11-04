#pragma once

class ScenePickTest : public vnScene
{
private:
	vnModel* pSphere[25];

	XMVECTOR resetPos;
	XMVECTOR resetRot;
	XMVECTOR resetScl;

	XMVECTOR resetCamPos;
	XMVECTOR resetCamTrg;

	//�����̏��
	XMVECTOR	LinePos;
	XMVECTOR	LineDir;

	//
	XMVECTOR	SphereCenter;
	float		SphereRadius;


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