//--------------------------------------------------------------//
//	"vn_collide.h"												//
//		衝突演算のインターフェース								//
//													2024/10/28	//
//														Onoe	//
//--------------------------------------------------------------//
#pragma once

#define EPSILON (1.0e-6)	//微小値

#define ISZERO(x) (fabsf(x)<EPSILON)	//マクロ(1行関数)

namespace vnCollide
{
	//形状:直線
	struct stLine
	{
		XMVECTOR Pos;
		XMVECTOR Dir;

		//2点から直線の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2);
	};

	//形状:平面
	struct stPlane
	{
		XMVECTOR Normal;
		float Distance;

		//3点から平面の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3);
	};

	//形状:三角形
	struct stTriangle
	{
		stPlane plane;	//内包
		XMVECTOR v[3];

		//3点から平面の情報を作成
		void fromPoints(const XMVECTOR* p1, const XMVECTOR* p2, const XMVECTOR* p3);
	};

	//形状:球
	struct stSphere
	{
		XMVECTOR Pos;
		float Radius;
	};

	//直線と点の最近傍距離を示す座標上の点を求める
	//[return] void
	//[Argument]
	//[out]XMVECTOR* rat : 求める座標を格納するアドレス
	//[in]const stLine* line : 直線の情報
	//[in]const XMVECTOR* pos : 点の座標
	void closestPoint(XMVECTOR* ret, const stLine* line, const XMVECTOR* pos);

	//直線と平面の交点を調べる
	//[return] bool true : 衝突している, false : 衝突していない
	//[Argument]
	//[out] XMVECTOR *hit : 衝突座標を格納するアドレス
	//[in] const stLine* line : 直線の情報
	//[in] const stPlane* plane : 三角形の情報
	bool isCollide(XMVECTOR *hit, const stLine* line, const stPlane* plane);

	//直線と三角形の交点を調べる
	//[return] bool true : 衝突している, false : 衝突していない
	//[Argument]
	//[out] XMVECTOR *hit : 衝突座標を格納するアドレス
	//[in] const stLine* line : 直線の情報
	//[in] const stTriangle* tri : 三角形の情報
	bool isCollide(XMVECTOR *hit, const stLine* line, const stTriangle* tri);

	//直線と球の交点を調べる
	//[return] int 交点の数
	//[Argument]
	//[out] XMVECTOR *hit : 衝突座標を格納する配列の先頭アドレス
	//[in] const stLine* line : 直線の情報
	//[in] const stSphere* sphere : 球の情報
	int isCollide(XMVECTOR *hit, const stLine* line, const stSphere* sphere);
}