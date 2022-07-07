
//=============================================================================
//
//		�Q�[������ [game.cpp]
//
//=============================================================================
#include<d3d9.h>
#include<d3dx9.h>
#include"main.h"
#include"Polygon.h"
#include"Polygon3Dtest.h"
#include"camera.h"
#include"input.h"
#include"lighting.h"
#include"MeshField.h"
#include "xmodel.h"
#include "Shadow.h"
#include "game.h"
#include "scene.h"
#include "target.h"
#include "taihou.h"
#include "speedup.h"
#include "score.h"
#include "score2.h"
#include "number.h"
#include "number2.h"
#include "UI.h"
#include "coin.h"
#include "coin10.h"
#include "kabe.h"
#include <time.h>
#include "tatemono.h"
#include "sound.h"

static int coin = 0;//�R�C���p�ϐ�

static int count = 0;//���y�p�ϐ�

void InitGame(void)//����������
{
	count = 0;//���y�p�ϐ��̏�����
	InitSound(NULL);//���y��������

	coin = 0;
	srand((unsigned)time(0));//�����_���ϐ����g���ׂ�������

	InitSpeedUp();
	Initcoin();
	Initcoin10();
	Initkabe();
	Inittatemono();
	InitTarget();
	InitPolygon3D();
	InitModel();
	InitMeshField(1, 50, 1000.0f, 10000.0f);		   //Field�ݒ�
	InitShadow();
	InitTaihou();
}



void UninitGame(void)//�I������
{
	UninitPolygon3D();
	UninitSpeedUp();
	Uninitcoin();
	Uninitcoin10();
	Uninitkabe();
	Uninittatemono();
	UninitTarget();
	UninitMeshField();
	UninitShadow();
	UninitModel();
	UninitTaihou();
	UninitSound();//���y�̏I������
}



void UpdateGame(void)//�X�V����
{

	if (count == 0)
	{
		PlaySound(SOUND_LABEL_BGM001);//���y���Ȃ炷
	}
	count++;

	UpdatePolygon3D();
	UpdateSpeedUp();
	Updatecoin();
	Updatecoin10();
	Updatekabe();
	Updatetatemono();
	UpdateTarget();
	UpdateModel();
	UpdateMeshField();
	UpdateShadow();
	UpdateCamera();
	UpdateTaihou();

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		ChangeScene(SCENE_RESULT);
	}
}



void DrawGame(void)//�`�揈��
{
	DrawMeshField();
	DrawPolygon3D();
	Drawcoin();
	Drawcoin10();
	Drawkabe();
	Drawtatemono();
	DrawTarget();
	DrawSpeedUp();
	DrawModel();
	DrawShadow();
	DrawTaihou();
	DrawScore(coin, 3, 0.0f, 0.0f, true);
	DrawUI(TEX_1, 10, 530, 330, 225, 0, 0, 1000, 500);
}

//game.h���琔���������Ă���֐�
void getcoin(int coinnum)
{
	coin = coinnum;//main.h���琔���������Ă���coin�ɓ���鏈��
}
