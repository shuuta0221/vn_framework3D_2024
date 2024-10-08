//--------------------------------------------------------------//
//	"vn_light.h"												//
//		���C�g													//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class vnLight
{
private:
	static XMVECTOR	Direction;	//���s�����̕���
	static XMVECTOR	iDirection;	//���s�����̋t����
	
	static XMVECTOR	LightColor;	//���s�����̐F
	
	static XMVECTOR	Ambient;	//�A���r�G���g�̐F
	
public:
	static bool initialize();
	static void terminate();

	static XMVECTOR *getILightDir();
	static XMVECTOR *getLightColor();
	static XMVECTOR *getAmbient();
	
	//�擾
	static float getLightDirX();
	static float getLightDirY();
	static float getLightDirZ();
	
	static float getLightColR();
	static float getLightColG();
	static float getLightColB();
	
	static float getAmbientR();
	static float getAmbientG();
	static float getAmbientB();
	
	//�ݒ�
	static void setLightDir(float x, float y, float z);
	static void setLightDirX(float value);
	static void setLightDirY(float value);
	static void setLightDirZ(float value);
	
	static void setLightCol(float r, float g, float b);
	static void setLightColR(float value);
	static void setLightColG(float value);
	static void setLightColB(float value);
	
	static void setAmbient(float r, float g, float b);
	static void setAmbientR(float value);
	static void setAmbientG(float value);
	static void setAmbientB(float value);
	
	//���Z
	static void addLightDir(float x, float y, float z);
	static void addLightDirX(float value);
	static void addLightDirY(float value);
	static void addLightDirZ(float value);
	
	static void addLightCol(float r, float g, float b);
	static void addLightColR(float value);
	static void addLightColG(float value);
	static void addLightColB(float value);
	
	static void addAmbient(float r, float g, float b);
	static void addAmbientR(float value);
	static void addAmbientG(float value);
	static void addAmbientB(float value);
	
};
