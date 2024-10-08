#pragma once


class SceneGroundTest : public vnScene
{
private:
	vnModel* pPlayer;
	vnModel* pShadow;
	
	vnModel* pGround;
	vnModel* pSky;

	//�����̏��
	XMVECTOR LinePos;
	XMVECTOR LineDir;

	//���ʂ̏��
	//���ʕ����� ax + by + cz = d
	XMVECTOR PlaneNormal;	//(a,b,c)
	float PlaneDistance;	//d


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
