#include <d3dx9.h>
#include "main.h"
#include "input.h"
#include "Polygon.h"
#include "Scene.h"
#include "title.h"
#include"lighting.h"
#include "UI.h"
#include "sound.h"

static float migi = -958;//
static int count = 0;//���y�p�ϐ�
static int count1 = 0;//���y�p�ϐ�

void InitTitle(void)
{
	count = 0;//���y�p�ϐ��̏�����
	count1 = 0;//���y�p�ϐ��̏�����
	InitSound(NULL);//���y��������
}

void UninitTitle(void)
{
	UninitSound();//���y�̏I������
}

void UpdateTitle(void)
{
	if (count == 0)
	{	
		PlaySound(SOUND_LABEL_BGM000);//���y���Ȃ炷
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
		
		ChangeScene(SCENE_GAME);
	}
	
	//�����@�ړ�
	migi = migi + 3;
	if (migi > 1500)
	{
		migi = -958;
	}

}

void DrawTitle(void)
{
	//SetUIColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	DrawUI(TEX_4, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1052, 727);//�^�C�g�����

	
		DrawUI(
			TEX_11,//�g���e�N�X�`��,
			migi, //x���W,
			550,//y���W,
			958,//�e�N�X�`���̕\�����鉡�̒���,//���̉摜�̉��̒�����32
			110,//�e�N�X�`���̕\������c�̒���,//���̉摜�̏c�̒�����32
			0,//���̉摜�̐؂���J�n�ꏊ�i���j�i�摜�̍����琔����j
			0,//���̉摜�̐؂���J�n�ꏊ�i�c�j�i�摜�̏ォ�琔����j
			958,//���̉摜�̐؂��肽����
			110 //���̉摜�̐؂��肽������
		);


}