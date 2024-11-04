//--------------------------------------------------------------//
//	"vn_collide.cpp"											//
//		�Փˉ��Z�̃C���^�[�t�F�[�X								//
//													2024/10/28	//
//														Onoe	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/vn_environment.h"

using namespace vnCollide;

//2�_���璼���̏����쐬
void stLine::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2)
{
	Pos = *p1;
	Dir = *p2 - *p1;
	Dir = XMVector3Normalize(Dir);
}

//3�_���畽�ʂ̏����쐬
void stPlane::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3)
{
	XMVECTOR v, w;
	v = *p3 - *p1;
	w = *p2 - *p1;
	Normal = XMVector3Cross(v, w);
	Normal = XMVector3Normalize(Normal);
	Distance = XMVectorGetX(XMVector3Dot(Normal, *p1));
}

//3�_���畽�ʂ̏����쐬
void stTriangle::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3)
{
	plane.fromPoints(p1, p2, p3);
	v[0] = *p1;
	v[1] = *p2;
	v[2] = *p3;
}

//�����Ɠ_�̍ŋߖT�������������W��̓_�����߂�
void vnCollide::closestPoint(XMVECTOR* ret, const stLine* line, const XMVECTOR* pos)
{
	XMVECTOR v = *pos - line->Pos;

	//�[���x�N�g���̃`�F�b�N
	float vx = XMVectorGetX(v);
	float vy = XMVectorGetY(v);
	float vz = XMVectorGetZ(v);
	if (ISZERO(vx) && ISZERO(vy) && ISZERO(vz)) {
		//Line�n�_�Ɠ_����v
		*ret = line->Pos;
		return;
	}

	XMVECTOR vDot = XMVector3Dot(v, line->Dir);
	float dot = XMVectorGetX(vDot);

	*ret = line->Pos + line->Dir * dot;
}

//�����ƕ��ʂ̌�_�𒲂ׂ�
bool vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stPlane* plane)
{
	//�����ƕ��ʂ����s�����ׂ�(�����ƕ��ʂ̖@��������)
	float w = XMVectorGetX(XMVector3Dot(line->Dir, plane->Normal));
	if (ISZERO(w)) //�������p
	{
		return false;
	}

	//�����ƕ��ʂ̏Փˍ��W�����߂�
	float s, d;
	XMVECTOR vS = XMVector3Dot(plane->Normal, line->Pos);
	s = XMVectorGetX(vS);
	s += plane->Distance;
	XMVECTOR vD = XMVector3Dot(plane->Normal, line->Pos + line->Dir);
	d = XMVectorGetX(vD);
	d += plane->Distance;

	if (ISZERO(s - d)) {
		return false;
	}

	//�����ƕ��ʂ��Փ˂����Ƃ��̔���
	*hit = line->Pos + line->Dir * (s / (s - d));
	
	return true;
}

//�����ƎO�p�`�̌�_�𒲂ׂ�
bool vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stTriangle* tri)
{
	if (isCollide(hit, line, &tri->plane) == false) {
		return false;
	}

	//�����ƕ��ʂ̌�_���O�p�`���ɂ��邩�`�F�b�N
	XMVECTOR e, h, c;
	float d;
	e = tri->v[1] - tri->v[0];
	h = *hit - tri->v[0];
	c = XMVector3Cross(e, h);
	d = XMVectorGetX(XMVector3Dot(c, tri->plane.Normal));
	if (d < 0.0f)return false;	//�O�p�`����͂ݏo�Ă���

	return true;
}

//�����Ƌ��̌�_�𒲂ׂ�
int vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stSphere* sphere)
{
	int n = 0;	//�Փˍ��W�̐�
	XMVECTOR v = line->Pos - sphere->Pos;
	
	//�[���x�N�g���̃`�F�b�N
	float vx = XMVectorGetX(v);
	float vy = XMVectorGetY(v);
	float vz = XMVectorGetZ(v);
	if (ISZERO(vx) && ISZERO(vy) && ISZERO(vz)) {
		hit[n++] = line->Pos + line->Dir * sphere->Radius;
		hit[n++] = line->Pos - line->Dir * sphere->Radius;
		return n;
	}
	
	XMVECTOR nv = XMVector3Normalize(v);
	XMVECTOR vdt = XMVector3Dot(nv, line->Dir);
	XMVECTOR vdl = XMVector3Length(v);
	float dt = XMVectorGetX(vdt);
	float dl = XMVectorGetX(vdl);
	if (dt >= 1.0f)dt = 1.0f;
	float dr = dl * sqrtf(1.0f - dt * dt);	//�����S�ƒ����̋���
	if (dr <= sphere->Radius)
	{	//�����S�ƒ����̋��������a�ȓ��Ȃ�Փ�
		float dq = sqrtf(sphere->Radius * sphere->Radius - dr * dr);
		float ds = -dl * dt;
		float dp = ds + dq;
		float dm = ds - dq;

		hit[n++] = line->Pos + line->Dir * dm;
		if (ISZERO(dq) == false)
		{	//��_(�Փˍ��W)������݂���
			hit[n++] = line->Pos + line->Dir * dp;
		}
	}

	return n;
}
