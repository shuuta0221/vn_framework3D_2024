#pragma once


class SceneGroundTest : public vnScene
{
private:
	vnModel* pPlayer;
	vnModel* pShadow;
	
	vnModel* pGround;
	vnModel* pSky;

	//’¼ü‚Ìî•ñ
	XMVECTOR LinePos;
	XMVECTOR LineDir;

	//•½–Ê‚Ìî•ñ
	//•½–Ê•û’ö® ax + by + cz = d
	XMVECTOR PlaneNormal;	//(a,b,c)
	float PlaneDistance;	//d


	//‹ÉÀ•W‚Ìî•ñ
	float radius;	//”¼Œa
	float theta;	//•½–ÊŠp
	float phi;		//‹ÂŠp

public:
	//‰Šú‰»
	bool initialize();
	//I—¹
	void terminate();

	//ˆ—
	void execute();
	//•`‰æ
	void render();
};
