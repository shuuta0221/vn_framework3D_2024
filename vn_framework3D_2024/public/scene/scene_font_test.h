//--------------------------------------------------------------//
//	"scene_font_test.h"											//
//		�V�[���F�t�H���g�e�X�g									//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

class SceneFontTest : public vnScene
{
private:
	int Cursor;		//�I��
	int FontNum;	//�g�p�ł���t�H���g�̐�

	IDWriteTextFormat** textFormat;	//�쐬���ꂽ�t�H���g
	
public:	
	//������
	bool initialize();

	//�I��
	void terminate();

	//����
	void execute();

	//�`��
	void render();
};
