#include "taihou.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include"camera.h"
#include"input.h"

//X���f���\���p�ϐ�
static LPD3DXMESH pMesh = NULL;
static DWORD dwNumMaterials = 0;
static D3DMATERIAL9* pMeshMaterials = NULL;
static LPDIRECT3DTEXTURE9* pMeshTextures = NULL;


static D3DXMATRIX g_mtxWorld;
static D3DXMATRIX g_mtxWorldS;
static D3DXMATRIX g_mtxWorldT;
static D3DXMATRIX g_mtxWorldR;

static D3DXVECTOR3 g_Pos(0.0f, 0.0f, 0.0f);   //xmod pos

static float g_fAngle = 0.0f;	//���ˎ��_�̍��E�p�x


//�e�N�X�`��

void InitTaihou(void)
{

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//X�t�@�C�����烁�b�V�������[�h����
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX("./resource/xmodel/taihou.x", D3DXMESH_SYSTEMMEM,
		pDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&dwNumMaterials, &pMesh)))
	{
		MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
		return;
	}

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];//�}�e���A���̃������̈���}�e���A���̐������m�ۂ�pMeshMaterials�ɓ����
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];//�e�N�X�`���ɕK�v�ȃ������̈���m�ۂ�pMeshTextures�ɓ����

														   //��Ŋm�ۂ����������̈�ɒ��g�����Ă���
	for (DWORD i = 0; i < dwNumMaterials; i++) //�}�e���A���̐��������[�v����
	{
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;//�}�e���A���f�[�^������
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;//Ambient��Diffuse�Ɠ����l�ɂ���

															  //pTextureFilename�����ƂɃe�N�X�`���I�u�W�F�N�g�̐���
		pMeshTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL && lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				d3dxMaterials[i].pTextureFilename,
				&pMeshTextures[i])))
			{
				MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();//�������̊J��
};

void UninitTaihou(void)
{
	g_fAngle = 0.0f;	//���ˎ��_�̍��E�p�x
	if (pMesh != NULL)
	{
		pMesh->Release();//����ŊJ�������
		pMesh = NULL;//�A�h���X���c���Ă���̂�NULL�ŏ㏑��
	}
};

void UpdateTaihou(void)
{

	//���ˎ��_�̍��E����
	if (GetKeyboardPress(DIK_Q) && g_Pos.y == 0.0f)
	{
		g_fAngle -= 0.4f;

		if (g_fAngle <= -10)
		{
			g_fAngle = -10;
		}
	}

	if (GetKeyboardPress(DIK_E) && g_Pos.y == 0.0f)
	{
		g_fAngle += 0.4f;

		if (g_fAngle >= 10)
		{
			g_fAngle = 10;
		}
	}

	D3DXMatrixRotationY(&g_mtxWorldR, D3DXToRadian(g_fAngle));		//rotation�AY����]

	D3DXMatrixScaling(&g_mtxWorldS, 0.35f, 0.35f, 0.35f);     //scale

	D3DXMatrixTranslation(&g_mtxWorldT, 0, 0, 0);   //move

	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorldS, &g_mtxWorldT);       //move+world = world
	D3DXMatrixMultiply(&g_mtxWorld, &g_mtxWorld, &g_mtxWorldR);

};
void DrawTaihou(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//���[���h�s��
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorld);

	//Direct3D�ɂ�����u���b�V���v�́u�����_�����O�v�́u�}�e���A���v����ɂ��Ă��܂��B
	//Direct3D�ɂ�����u�I�u�W�F�N�g�f�[�^�v�́u�`��v�́u�\�ʓI�������v����ɂ��Ă��܂��B
	for (DWORD i = 0; i < dwNumMaterials; i++) //�}�e���A���̐��������[�v����
	{
		pDevice->SetMaterial(&pMeshMaterials[i]);//�}�e���A����ݒ肷��
		pDevice->SetTexture(0, pMeshTextures[i]);//�e�N�X�`����ݒ肷��
		pMesh->DrawSubset(i);//�����_�����O����
	}
};
