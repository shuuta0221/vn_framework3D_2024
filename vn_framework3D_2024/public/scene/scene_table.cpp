//--------------------------------------------------------------//
//	"scene_table.cpp"											//
//		�V�[���e�[�u��											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

//�N�����̃V�[��
eSceneTable initialScene = eSceneTable::Boot;
//���݂̃V�[��
eSceneTable currentScene = (eSceneTable)-1;
//�؂�ւ��\��̃V�[��
eSceneTable reserveScene = initialScene;


//�e�V�[���̖��O
WCHAR SceneName[(int)eSceneTable::SceneMax][32] =
{
	L"Boot",
	L"Ground Test",
	L"Collision Test",
	L"Field Test",
	L"Model Test",
	L"Cube Test",
	L"Sprite Test",
	L"Joystick Test",
	L"SE Test",
	L"Font Test",
};

//�X�e�[�W�؂�ւ��֐�(�\��)
void switchScene(eSceneTable scene)
{
	reserveScene = scene;
}

//���݂̃X�e�[�W�擾(������)
void switchScene()
{
	if(currentScene==reserveScene)
	{
		return;
	}
	
	vnMainFrame::terminateScene();
	
	vnScene *pScene=NULL;
	switch(reserveScene)
	{
	case Boot:			pScene = new SceneBoot();			break;
	case GroundTest:	pScene = new SceneGroundTest();		break;
	case CollisionTest:	pScene = new SceneCollisionTest();	break;
	case FieldTest:		pScene = new SceneFieldTest();		break;
	case ModelTest:		pScene = new SceneModelTest();		break;
	case CubeTest:		pScene = new SceneCubeTest();		break;
	case SpriteTest:	pScene = new SceneSpriteTest();		break;
	case JoystickTest:	pScene = new SceneJoystickTest();	break;
	case SeTest:		pScene = new SceneSeTest();			break;
	case FontTest:		pScene = new SceneFontTest();		break;
	}
	
	vnMainFrame::initializeScene(pScene);
	
	currentScene = reserveScene;
}
