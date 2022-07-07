#include <d3dx9.h>
#include "main.h"
#include "input.h"
#include "Polygon.h"
#include "Scene.h"
#include "result.h"
#include "UI.h"
#include "score2.h"
#include "coin.h"
#include "title.h"
#include "sound.h"

static int coin = 0;//�R�C���p�ϐ�

static int count = 0;//���y�p�ϐ�
static int count1 = 0;//���y�p�ϐ�

//�e�N�X�`���s�R�s�R
bool g_bPress = true;//�^�C�g���s�R�s�R
int g_nCnt = 0;//
//�e�N�X�`���s�R�s�R

void InitResult(void)
{
	count = 0;//���y�p�ϐ��̏�����
	count1 = 0;//���y�p�ϐ��̏�����
	InitSound(NULL);//���y��������

}
void UninitResult(void)
{
	coin = 0;//�R�C��������
	//�e�N�X�`���s�R�s�R
	g_bPress = true;//�^�C�g���s�R�s�R
	g_nCnt = 0;//
	//�e�N�X�`���s�R�s�R


	UninitSound();//���y�̏I������
}
void UpdateResult(void)
{

	if (count == 0)
	{
		PlaySound(SOUND_LABEL_BGM008);//���y���Ȃ炷
	}
	count++;


	//�V�[���؂�ւ�
	if (GetKeyboardTrigger(DIK_RETURN))
	{

		if (count1 == 0)
		{
			PlaySound(SOUND_LABEL_BGM004);//���y���Ȃ炷ok
		}
		count1++;

		ChangeScene(SCENE_TITLE);
	}
}

void DrawResult(void)
{
	SetUIColor(D3DCOLOR_RGBA(255,255,255,255));
	DrawUI(TEX_3, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1000, 500);


	//�e�N�X�`���s�R�s�R
	g_nCnt++;
	if (g_nCnt % 60 == 0)
	{
		switch (g_bPress)
		{
		case false:
			g_bPress = true;
			break;
		case true:
			g_bPress = false;
			break;
		}
	}

	if (g_bPress == true)
	{
		SetUIColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		DrawScore2(coin, 3, 300.0f, 300.0f, true);
	}
	//�e�N�X�`���s�R�s�R
}

//coin.h���琔���������Ă���֐�
void getcoin1(int coinnum)
{
	coin = coinnum;//coin.h���琔���������Ă���coin�ɓ���鏈��
}