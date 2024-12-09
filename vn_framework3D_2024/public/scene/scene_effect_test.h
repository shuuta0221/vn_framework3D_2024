#pragma once

class SceneEffectTest : public vnScene
{
private:



	vnEmitter* pEmitter;

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