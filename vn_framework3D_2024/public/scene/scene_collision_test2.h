#pragma once

class SceneCollisionTest2 : public vnScene
{
private:

	XMVECTOR Point[5];

	vnModel* pSphere;
	vnModel* pHit[2];

	//�f�o�b�O�p���f��(�ŋߖT���������߂钼�����1�_)
	vnModel* pPoint;

	//�����̏��
	XMVECTOR LinePos;
	XMVECTOR LineDir;

	//���̏��
	XMVECTOR SphereCenter;
	float	 SphereRadius;

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