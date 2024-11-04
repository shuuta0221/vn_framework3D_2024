//--------------------------------------------------------------//
//	"vn_collide.cpp"											//
//		衝突演算のインターフェース								//
//													2024/10/28	//
//														Onoe	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "../framework/vn_environment.h"

using namespace vnCollide;

//2点から直線の情報を作成
void stLine::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2)
{
	Pos = *p1;
	Dir = *p2 - *p1;
	Dir = XMVector3Normalize(Dir);
}

//3点から平面の情報を作成
void stPlane::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3)
{
	XMVECTOR v, w;
	v = *p3 - *p1;
	w = *p2 - *p1;
	Normal = XMVector3Cross(v, w);
	Normal = XMVector3Normalize(Normal);
	Distance = XMVectorGetX(XMVector3Dot(Normal, *p1));
}

//3点から平面の情報を作成
void stTriangle::fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3)
{
	plane.fromPoints(p1, p2, p3);
	v[0] = *p1;
	v[1] = *p2;
	v[2] = *p3;
}

//直線と点の最近傍距離を示す座標上の点を求める
void vnCollide::closestPoint(XMVECTOR* ret, const stLine* line, const XMVECTOR* pos)
{
	XMVECTOR v = *pos - line->Pos;

	//ゼロベクトルのチェック
	float vx = XMVectorGetX(v);
	float vy = XMVectorGetY(v);
	float vz = XMVectorGetZ(v);
	if (ISZERO(vx) && ISZERO(vy) && ISZERO(vz)) {
		//Line始点と点が一致
		*ret = line->Pos;
		return;
	}

	XMVECTOR vDot = XMVector3Dot(v, line->Dir);
	float dot = XMVectorGetX(vDot);

	*ret = line->Pos + line->Dir * dot;
}

//直線と平面の交点を調べる
bool vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stPlane* plane)
{
	//直線と平面が平行か調べる(直線と平面の法線が垂直)
	float w = XMVectorGetX(XMVector3Dot(line->Dir, plane->Normal));
	if (ISZERO(w)) //早期棄却
	{
		return false;
	}

	//直線と平面の衝突座標を求める
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

	//直線と平面が衝突したときの判定
	*hit = line->Pos + line->Dir * (s / (s - d));
	
	return true;
}

//直線と三角形の交点を調べる
bool vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stTriangle* tri)
{
	if (isCollide(hit, line, &tri->plane) == false) {
		return false;
	}

	//直線と平面の交点が三角形内にあるかチェック
	XMVECTOR e, h, c;
	float d;
	e = tri->v[1] - tri->v[0];
	h = *hit - tri->v[0];
	c = XMVector3Cross(e, h);
	d = XMVectorGetX(XMVector3Dot(c, tri->plane.Normal));
	if (d < 0.0f)return false;	//三角形からはみ出ている

	return true;
}

//直線と球の交点を調べる
int vnCollide::isCollide(XMVECTOR* hit, const stLine* line, const stSphere* sphere)
{
	int n = 0;	//衝突座標の数
	XMVECTOR v = line->Pos - sphere->Pos;
	
	//ゼロベクトルのチェック
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
	float dr = dl * sqrtf(1.0f - dt * dt);	//球中心と直線の距離
	if (dr <= sphere->Radius)
	{	//球中心と直線の距離が半径以内なら衝突
		float dq = sqrtf(sphere->Radius * sphere->Radius - dr * dr);
		float ds = -dl * dt;
		float dp = ds + dq;
		float dm = ds - dq;

		hit[n++] = line->Pos + line->Dir * dm;
		if (ISZERO(dq) == false)
		{	//交点(衝突座標)が二つ存在する
			hit[n++] = line->Pos + line->Dir * dp;
		}
	}

	return n;
}
