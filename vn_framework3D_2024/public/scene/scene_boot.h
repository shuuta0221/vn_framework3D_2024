//--------------------------------------------------------------//
//	"scene_boot.h"												//
//		�V�[��(�N��/�V�[���Z���N�g)								//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class SceneBoot : public vnScene
{
private:
	int Cursor;
	
public:
	bool initialize();
	void terminate();
	
	void execute();
	void render();
};
