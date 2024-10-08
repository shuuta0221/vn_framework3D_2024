//--------------------------------------------------------------//
//	"vn_scene.cpp"												//
//		�V�[���N���X											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#include "../../framework.h"
#include "../../framework/vn_environment.h"

bool vnScene::UpdateRenderPriority = false;

//�R���X�g���N�^
vnScene::vnScene()
{
	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		pObject2D_Array[i] = NULL;
	}
	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		pObject3D_Array[i] = NULL;
	}
	UpdateRenderPriority = false;
}

//�f�X�g���N�^
vnScene::~vnScene()
{
	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL)continue;
		delete pObject3D_Array[i];
		pObject3D_Array[i] = NULL;
	}
	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] == NULL)continue;
		delete pObject2D_Array[i];
		pObject2D_Array[i] = NULL;
	}
	UpdateRenderPriority = false;
}

//�����֐�
void vnScene::execute()
{
	//2D�I�u�W�F�N�g��execute�����s
	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] == NULL)continue;
		if (pObject2D_Array[i]->isExecuteEnable() == false)continue;
		pObject2D_Array[i]->execute();
	}
	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL)continue;
		if (pObject3D_Array[i]->isExecuteEnable() == false)continue;
		pObject3D_Array[i]->execute();
	}
}

//�`��֐�
void vnScene::render()
{
	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL)continue;
		if (pObject3D_Array[i]->isExecuteEnable() == false)continue;
		pObject3D_Array[i]->render();
	}

	if (UpdateRenderPriority == true)	//�\�[�g�̕K�v�����鎞�̂݃\�[�g����
	{
		//�`��D�揇�ʂɏ]���Ĕz����\�[�g
		for (int i = 0; i < vnOBJECT2D_MAX - 1; i++)
		{
			if (pObject2D_Array[i] == NULL)continue;
			for (int j = i + 1; j < vnOBJECT2D_MAX; j++)
			{
				if (pObject2D_Array[j] == NULL)continue;
				if (pObject2D_Array[i]->getRenderPriority() > pObject2D_Array[j]->getRenderPriority())
				{
					vnSprite* tmp = pObject2D_Array[i];
					pObject2D_Array[i] = pObject2D_Array[j];
					pObject2D_Array[j] = tmp;
				}
			}
		}
		UpdateRenderPriority = false;	//�\�[�g������������A���Ƀ\�[�g���K�v�ɂȂ�܂Ń\�[�g���Ȃ��悤�ɂ���
	}

	//2D�I�u�W�F�N�g��render�����s
	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] == NULL)continue;
		if (pObject2D_Array[i]->isRenderEnable() == false)continue;
		pObject2D_Array[i]->render();
	}
}

//�o�^���ꂽ2D�I�u�W�F�N�g�̕`��D�揇�ʂ��ύX���ꂽ�̂ŁA�z����\�[�g������K�v������
void vnScene::setUpdateRenderPriority()
{
	UpdateRenderPriority = true;
}

//�I�u�W�F�N�g�̓o�^(�z��̋󂫂ɓ����)
bool vnScene::registerObject(vnSprite *p)
{
	if (p == NULL)
	{
		return false;
	}

	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] != NULL)continue;
		pObject2D_Array[i] = p;
		return true;
	}
	return false;
}

//�I�u�W�F�N�g�̔j��(�z�񂩂�폜/�I�u�W�F�N�g���̂�delete����)
void vnScene::deleteObject(vnSprite *p)
{
	if (p == NULL)
	{
		return;
	}

	for (int i = 0; i < vnOBJECT2D_MAX; i++)
	{
		if (pObject2D_Array[i] == NULL)continue;
		if (pObject2D_Array[i] == p)
		{
			delete pObject2D_Array[i];
			pObject2D_Array[i] = NULL;
		}
	}
}

bool vnScene::registerObject(vnObject* p)
{
	if (p == NULL)
	{
		return false;
	}

	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] != NULL)continue;
		pObject3D_Array[i] = p;
		return true;
	}
	return false;
}

void vnScene::deleteObject(vnObject* p)
{
	if (p == NULL)
	{
		return;
	}

	for (int i = 0; i < vnOBJECT3D_MAX; i++)
	{
		if (pObject3D_Array[i] == NULL)continue;
		if (pObject3D_Array[i] == p)
		{
			delete pObject3D_Array[i];
			pObject3D_Array[i] = NULL;
		}
	}
}
