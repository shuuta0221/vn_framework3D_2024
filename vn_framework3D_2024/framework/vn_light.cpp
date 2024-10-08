//--------------------------------------------------------------//
//	"vn_light.h"												//
//		���C�g													//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../framework.h"
#include "vn_environment.h"

XMVECTOR	vnLight::Direction;		//���s�����̕���
XMVECTOR	vnLight::iDirection;	//���s�����̋t����

XMVECTOR	vnLight::LightColor;	//���s�����̐F
XMVECTOR	vnLight::Ambient;		//�A���r�G���g�̐F


bool vnLight::initialize()
{
	Direction = XMVectorSet(-3.0f, -5.0f, -2.0f, 0.0f);

	iDirection = XMVector3Normalize(Direction);	//���������x�N�g���̐��K��
	iDirection = XMVectorNegate(iDirection);	//���������x�N�g���̔��]
	
	LightColor = XMVectorSet(0.8f, 0.8f, 0.8f, 1.0f);
	
	Ambient = XMVectorSet(0.2f, 0.2f, 0.2f, 0.0f);
	
	return true;
}

void vnLight::terminate()
{
}

XMVECTOR * vnLight::getILightDir()
{
	iDirection = XMVector3Normalize(Direction);
	iDirection = XMVectorNegate(iDirection);
	return &iDirection;
}

XMVECTOR * vnLight::getLightColor()
{
	return &LightColor;
}

XMVECTOR * vnLight::getAmbient()
{
	return &Ambient;
}

//�擾
float vnLight::getLightDirX()
{
	return XMVectorGetX(Direction);
}

float vnLight::getLightDirY()
{
	return XMVectorGetY(Direction);
}

float vnLight::getLightDirZ()
{
	return XMVectorGetZ(Direction);
}

float vnLight::getLightColR()
{
	return XMVectorGetX(LightColor);
}

float vnLight::getLightColG()
{
	return XMVectorGetY(LightColor);
}

float vnLight::getLightColB()
{
	return XMVectorGetZ(LightColor);
}

float vnLight::getAmbientR()
{
	return XMVectorGetX(Ambient);
}

float vnLight::getAmbientG()
{
	return XMVectorGetY(Ambient);
}

float vnLight::getAmbientB()
{
	return XMVectorGetZ(Ambient);
}

//�ݒ�
void vnLight::setLightDir(float x, float y, float z)
{
	Direction = XMVectorSet(x, y, z, 0.0f);
}

void vnLight::setLightDirX(float value)
{
	Direction = XMVectorSetX(Direction, value);
}

void vnLight::setLightDirY(float value)
{
	Direction = XMVectorSetY(Direction, value);
}

void vnLight::setLightDirZ(float value)
{
	Direction = XMVectorSetZ(Direction, value);
}

void vnLight::setLightCol(float r, float g, float b)
{
	LightColor = XMVectorSet(r, g, b, 1.0f);
}

void vnLight::setLightColR(float value)
{
	LightColor = XMVectorSetX(LightColor, value);
}

void vnLight::setLightColG(float value)
{
	LightColor = XMVectorSetY(LightColor, value);
}

void vnLight::setLightColB(float value)
{
	LightColor = XMVectorSetZ(LightColor, value);
}

void vnLight::setAmbient(float r, float g, float b)
{
	Ambient = XMVectorSet(r, g, b, 0.0f);
}

void vnLight::setAmbientR(float value)
{
	Ambient = XMVectorSetX(Ambient, value);
}

void vnLight::setAmbientG(float value)
{
	Ambient = XMVectorSetY(Ambient, value);
}

void vnLight::setAmbientB(float value)
{
	Ambient = XMVectorSetZ(Ambient, value);
}

//���Z
void vnLight::addLightDir(float x, float y, float z)
{
	Direction = XMVectorAdd(Direction, XMVectorSet(x, y, z, 0.0f));
}

void vnLight::addLightDirX(float value)
{
	Direction = XMVectorAdd(Direction, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void vnLight::addLightDirY(float value)
{
	Direction = XMVectorAdd(Direction, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void vnLight::addLightDirZ(float value)
{
	Direction = XMVectorAdd(Direction, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

void vnLight::addLightCol(float r, float g, float b)
{
	LightColor = XMVectorAdd(LightColor, XMVectorSet(r, g, b, 0.0f));
}

void vnLight::addLightColR(float value)
{
	LightColor = XMVectorAdd(LightColor, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void vnLight::addLightColG(float value)
{
	LightColor = XMVectorAdd(LightColor, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void vnLight::addLightColB(float value)
{
	LightColor = XMVectorAdd(LightColor, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}

void vnLight::addAmbient(float r, float g, float b)
{
	Ambient = XMVectorAdd(Ambient, XMVectorSet(r, g, b, 0.0f));
}

void vnLight::addAmbientR(float value)
{
	Ambient = XMVectorAdd(Ambient, XMVectorSet(value, 0.0f, 0.0f, 0.0f));
}

void vnLight::addAmbientG(float value)
{
	Ambient = XMVectorAdd(Ambient, XMVectorSet(0.0f, value, 0.0f, 0.0f));
}

void vnLight::addAmbientB(float value)
{
	Ambient = XMVectorAdd(Ambient, XMVectorSet(0.0f, 0.0f, value, 0.0f));
}
