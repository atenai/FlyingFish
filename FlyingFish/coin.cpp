#include "coin.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include "camera.h"
#include "input.h"
#include "judge.h"
#include "xmodel.h"
#include <time.h>

//X���f���\���悤�֐��̏�����
static LPD3DXBUFFER   g_pCoinMaterialBuffer = NULL;
static DWORD          g_nCoinMaterialNum = 0;
static LPD3DXMESH     g_pCoinMesh = NULL;
static LPD3DXMATERIAL pCoinMaterial;
static LPD3DXMESH     pCoinTemMesh;
//�e�N�X�`��
static LPDIRECT3DTEXTURE9 g_pTextures[] = { NULL };


typedef struct {
	D3DXVECTOR3 pos;//���W
	D3DXMATRIX mtxworld;//
	D3DXMATRIX mtxworldS;//�g�k�p�ϐ�
	D3DXMATRIX mtxworldR;//�g�k�p�ϐ�
	D3DXMATRIX mtxworldT;//�ړ��p�֐�
	bool hihixyouzibonbe;//�{���x�̕\����\��
}COIN_DATA;

static COIN_DATA Coin[COIN_NUM];//�\���̂ŃA�C�e����������邩��錾

//�\���̂��񂷁i���[�v�j�����邽�߂̕ϐ�
static int Ui = 0;//�\���̂����[�v�����邽�߂̕ϐ�

void Initcoin(void)
{
	

	for (Ui = 0; Ui < COIN_NUM; Ui++)//�\���̂̐������� 
	{
		Coin[Ui].pos.x = -500 + rand() % 1000;
		Coin[Ui].pos.y = 50 + rand() % 500;
		Coin[Ui].pos.z = 50 + rand() % 2800;
	}


	//������
	LPD3DXBUFFER  pAdjacencyBuffer;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//���f�����O�f�[�^�̏ꏊ
	HRESULT hr;

	hr = D3DXLoadMeshFromX("resource\\xmodel\\kiirokane.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		&pAdjacencyBuffer, &g_pCoinMaterialBuffer, NULL,
		&g_nCoinMaterialNum, &g_pCoinMesh);


	if (FAILED(hr))
	{
		MessageBox(NULL, "coin2�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	}

	//�}�e���A���̎擾
	pCoinMaterial = (LPD3DXMATERIAL)g_pCoinMaterialBuffer->GetBufferPointer();

	

	hr = g_pCoinMesh->OptimizeInplace(D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacencyBuffer->GetBufferPointer(),
		NULL, NULL, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, "���b�V���̍œK���Ɂu���s���܂���", "�G���[", MB_OK);
	}



	if (pAdjacencyBuffer != NULL)
	{
		pAdjacencyBuffer->Release();
	}

	D3DVERTEXELEMENT9 Elements[32];

	g_pCoinMesh->GetDeclaration(Elements);

	hr = g_pCoinMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, Elements, pDevice, &pCoinTemMesh);

	g_pCoinMesh->Release();

	g_pCoinMesh = pCoinTemMesh;
};

void Uninitcoin(void)
{
	g_pCoinMaterialBuffer->Release();
};

void Updatecoin(void)
{
	for (Ui = 0; Ui < COIN_NUM; Ui++)//�\���̂̐������� 
	{
		ataricoin(Ui, Coin[Ui].pos.x, Coin[Ui].pos.y, Coin[Ui].pos.z);//�X�V�����{���x�̐��ƍ��W�������蔻��ɑ���

		//SRT
		D3DXMatrixIdentity(&Coin[Ui].mtxworld);

		//D3DXMatrixScaling(&Coin[Ui].mtxworldS, 1.0f, 1.0f, 1.0f);     //�g��֐�
		//D3DXMatrixMultiply(&Coin[Ui].mtxworld, &Coin[Ui].mtxworld, &Coin[Ui].mtxworldS);//����  

		D3DXMatrixTranslation(&Coin[Ui].mtxworldT, Coin[Ui].pos.x, Coin[Ui].pos.y, Coin[Ui].pos.z);//���s�ړ��֐�
		D3DXMatrixMultiply(&Coin[Ui].mtxworld, &Coin[Ui].mtxworld, &Coin[Ui].mtxworldT);//����  
	}
};

void Drawcoin(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (Ui = 0; Ui < COIN_NUM; Ui++)//�\���̂̐������� 
	{

		for (unsigned int i = 0; i < g_nCoinMaterialNum; i++)
		{
			pCoinMaterial[i].MatD3D.Ambient.r = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.g = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.b = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.a = 1.0f;

			pDevice->SetMaterial(&pCoinMaterial[i].MatD3D);


			pDevice->SetTexture(0, NULL);		//�e�N�X�`�����g���ꍇ�ɓ�Ԗڂ̈�����g_pTextures[i]�ɕύX�A�g��Ȃ��ꍇ��NULL�ɕύX

			pDevice->SetTransform(D3DTS_WORLD, &Coin[Ui].mtxworld);

			g_pCoinMesh->DrawSubset(i);
		}
	}
};
