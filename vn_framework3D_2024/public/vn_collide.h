//--------------------------------------------------------------//
//	"vn_collide.h"												//
//		�Փˉ��Z�̃C���^�[�t�F�[�X								//
//													2024/10/28	//
//														Onoe	//
//--------------------------------------------------------------//
#pragma once

#define EPSILON (1.0e-6)	//�����l

#define ISZERO(x) (fabsf(x)<EPSILON)	//�}�N��(1�s�֐�)

namespace vnCollide
{
	//�`��:����
	struct stLine
	{
		XMVECTOR Pos;
		XMVECTOR Dir;

		//2�_���璼���̏����쐬
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2);
	};

	//�`��:����
	struct stPlane
	{
		XMVECTOR Normal;
		float Distance;

		//3�_���畽�ʂ̏����쐬
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3);
	};

	//�`��:�O�p�`
	struct stTriangle
	{
		stPlane plane;	//����
		XMVECTOR v[3];

		//3�_���畽�ʂ̏����쐬
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3);
	};

	//�`��:��
	struct stSphere
	{
		XMVECTOR Pos;
		float Radius;
	};

	//�����Ɠ_�̍ŋߖT�������������W��̓_�����߂�
	//[return] void
	//[Argument]
	//[out]XMVECTOR* rat : ���߂���W���i�[����A�h���X
	//[in]const stLine* line : �����̏��
	//[in]const XMVECTOR* pos : �_�̍��W
	void closestPoint(XMVECTOR* ret, const stLine* line, const XMVECTOR* pos);

	//�����ƕ��ʂ̌�_�𒲂ׂ�
	//[return] bool true : �Փ˂��Ă���, false : �Փ˂��Ă��Ȃ�
	//[Argument]
	//[out] XMVECTOR *hit : �Փˍ��W���i�[����A�h���X
	//[in] const stLine* line : �����̏��
	//[in] const stPlane* plane : �O�p�`�̏��
	bool isCollide(XMVECTOR *hit, const stLine* line, const stPlane* plane);

	//�����ƎO�p�`�̌�_�𒲂ׂ�
	//[return] bool true : �Փ˂��Ă���, false : �Փ˂��Ă��Ȃ�
	//[Argument]
	//[out] XMVECTOR *hit : �Փˍ��W���i�[����A�h���X
	//[in] const stLine* line : �����̏��
	//[in] const stTriangle* tri : �O�p�`�̏��
	bool isCollide(XMVECTOR *hit, const stLine* line, const stTriangle* tri);

	//�����Ƌ��̌�_�𒲂ׂ�
	//[return] int ��_�̐�
	//[Argument]
	//[out] XMVECTOR *hit : �Փˍ��W���i�[����z��̐擪�A�h���X
	//[in] const stLine* line : �����̏��
	//[in] const stSphere* sphere : ���̏��
	int isCollide(XMVECTOR *hit, const stLine* line, const stSphere* sphere);
}