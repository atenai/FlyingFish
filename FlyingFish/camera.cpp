#include "Camera.h"
#include "main.h"
#include "Xmodel.h"

//***************************************************************************************
//	�O���[�o���ϐ��錾
//***************************************************************************************

static D3DXMATRIX g_MtxView;		//�r���[�s��
static D3DXMATRIX g_MtxProjection;	//�v���W�F�N�V�����s��

static D3DXVECTOR3 g_Up(0, 1.f, 0);//��x�N�g��
static D3DXVECTOR3 g_Eye(0.0f, 3.0f, -3.0f);//�J�������W
static D3DXVECTOR3 g_At(0.0f, 0.0f, 0.0f);//���Ă�ꏊ(�����_)

static D3DXVECTOR3 g_Eye1(0.0f, 1.0f, -6.0f);//�J�������W



void UpdateCamera(void)
{

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();
	g_At = GetPos();
	g_Eye = GetPos() + g_Eye1;


	D3DXMatrixLookAtLH(&g_MtxView, &g_Eye, &g_At, &g_Up);
	//�v���W�F�N�V�����s��̍쐬
	D3DXMatrixPerspectiveFovLH(&g_MtxProjection, D3DX_PI / 3 , (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 3000.0f);

	//�e��ލs��̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_MtxView);
	pDevice->SetTransform(D3DTS_PROJECTION, &g_MtxProjection);
}
