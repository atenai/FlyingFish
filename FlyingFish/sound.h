//=============================================================================
//
// �T�E���h���� [sound.h]
//
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,//�^�C�g��
	SOUND_LABEL_BGM001,//�Q�[���v���C
	SOUND_LABEL_BGM002,//�ǁE�L���[�u
	SOUND_LABEL_BGM003,//�ԁE�E���R�C��
	SOUND_LABEL_BGM004,//����I��
	SOUND_LABEL_BGM005,//�A�C�e��
	SOUND_LABEL_BGM006,//�R�N�[���^���[
	SOUND_LABEL_BGM007,//���ˉ�
	SOUND_LABEL_BGM008,//���U���g
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);//����̉��y�������~�߂�
void StopSound(void);//�S�Ẳ��y���~�߂�

#endif
