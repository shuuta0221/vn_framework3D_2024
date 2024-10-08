//--------------------------------------------------------------//
//	"scene_boot.cpp"											//
//		�V�[��(�N��/�V�[���Z���N�g)								//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

#define CURSOR_X		(80.0f)		//�J�[�\����X���W
#define SCENE_NAME_X	(100.0f)	//�V�[������X���W
#define SCENE_NAME_Y	(100.0f)	//�V�[������Y���W
#define SCENE_NAME_LINE	(20.0f)		//�V�[�����̉��s��

extern WCHAR SceneName[(int)eSceneTable::SceneMax][32];

//�������֐�
bool SceneBoot::initialize()
{
	Cursor = 0;
	return true;
}

//�I���֐�
void SceneBoot::terminate()
{
}

//�����֐�
void SceneBoot::execute()
{
	if (vnKeyboard::trg(DIK_UP) && --Cursor < 0)
	{	//���L�[�ŃJ�[�\������Ɉړ�
		Cursor = (int)eSceneTable::SceneMax - 1;
	}
	else if (vnKeyboard::trg(DIK_DOWN) && ++Cursor >= (int)eSceneTable::SceneMax)
	{	//���L�[�ŃJ�[�\�������Ɉړ�
		Cursor = 0;
	}
	else if (vnKeyboard::trg(DIK_SPACE))
	{	//SPACE�L�[�ŃV�[����؂�ւ�
		switchScene((eSceneTable)Cursor);
	}

	//�J�[�\���̕\��
	vnFont::print(CURSOR_X, SCENE_NAME_Y + SCENE_NAME_LINE * (float)Cursor, 0xffffff00, L"��");
	//�V�[�����̕\��
	for (int i = 0; i < (int)eSceneTable::SceneMax; i++)
	{
		DWORD col = i == Cursor ? 0xffffff80 : 0xffffffff;
		vnFont::print(SCENE_NAME_X, SCENE_NAME_Y + SCENE_NAME_LINE * (float)i, col, SceneName[i]);
	}

	vnScene::execute();
}

//�`��֐�
void SceneBoot::render()
{
	vnScene::render();
}

