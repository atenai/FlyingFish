#pragma once
#pragma once
//=============================================================================
//  
// UI�`��
//
//�@      
//=============================================================================

#ifndef _UI_H_
#define _UI_H_

typedef struct
{
	char fileName[256];
	int width;
	int height;
}TEX;

//�e�N�X�`���񋓌^(0)
typedef enum
{
	TEX_1 = 0,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,
	TEX_8,
	TEX_9,
	TEX_10,
	TEX_11,
	TEX_MAX
}TEXTURE_NUM;
//�e�N�X�`���̎w��(1)

bool InitUI(void);//�|���S���̏���������
void UninitUI(void);//�|���S���̏I������
void UpdateUI(void);//�|���S���̍X�V����
void SetUIColor(D3DCOLOR color);//�|���S���J���[�̐ݒ�
void DrawUI(//�摜��\��
	int texNum,//�g���e�N�X�`��,
	float dx, //�`��ʒuX��
	float dy, //�`��ʒuY��
	int dw, //�e�N�X�`���̕\�����鉡�̒���
	int dh, //�e�N�X�`���̕\������c�̒���
	int tcx, //���̉摜�̐؂���J�n�ꏊ�i���j�i�摜�̍����琔����j
	int tcy, //���̉摜�̐؂���J�n�ꏊ�i�c�j�i�摜�̏ォ�琔����j
	int tcw, //���̉摜�̐؂��肽����
	int tch //���̉摜�̐؂��肽������
);//�|���S���̕`�揈��

#endif // _UI_H_


