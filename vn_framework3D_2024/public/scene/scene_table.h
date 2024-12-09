//--------------------------------------------------------------//
//	"scene_table.h"												//
//		�V�[���e�[�u��											//
//													2024/04/01	//
//														Ichii	//
//--------------------------------------------------------------//
#pragma once

//�V�[���̎��
enum eSceneTable
{
	Boot,
	EffectTest,
	PickTest,
	CollisionTest2,
	GroundTest,
	CollisionTest,
	FieldTest,
	ModelTest,
	CubeTest,
	SpriteTest,
	JoystickTest,
	SeTest,
	FontTest,
	SceneMax,
};

//�V�[���N���X��`�t�@�C��
#include "vn_scene.h"
#include "scene_effect_test.h"
#include "scene_boot.h"
#include "scene_pick_test.h"
#include "scene_collision_test2.h"
#include "scene_ground_test.h"
#include "scene_collision_test.h"
#include "scene_field_test.h"
#include "scene_model_test.h"
#include "scene_cube_test.h"
#include "scene_sprite_test.h"
#include "scene_joystick_test.h"
#include "scene_se_test.h"
#include "scene_font_test.h"

//�V�[���؂�ւ��֐�(�\��)
void switchScene(eSceneTable scene);
//���݂̃V�[���擾(������)
void switchScene();
