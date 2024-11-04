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

	//’¼ü‚Ìî•ñ
	XMVECTOR	LinePos;
	XMVECTOR	LineDir;

	//
	XMVECTOR	SphereCenter;
	float		SphereRadius;


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