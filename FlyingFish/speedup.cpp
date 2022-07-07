#include "speedup.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include"camera.h"
#include"input.h"
#include "xmodel.h"


//X���f���\���悤�֐��̏�����
static LPD3DXMESH     g_pMesh = NULL;
static LPD3DXMESH     pTemMesh;
static DWORD          g_nMaterialNum = 0;
static LPD3DXBUFFER   g_pMaterialBuffer = NULL;
static LPD3DXMATERIAL pMaterial;
static LPDIRECT3DTEXTURE9 g_pTextures[] = { NULL };


typedef struct {
	D3DXVECTOR3 pos;//���W
	D3DXMATRIX mtxworld;//
	D3DXMATRIX mtxworldS;//�g�k�p�ϐ�
	D3DXMATRIX mtxworldR;//�g�k�p�ϐ�
	D3DXMATRIX mtxworldT;//�ړ��p�֐�
	bool hihixyouzibonbe;//�{���x�̕\����\��
}ITEM_DATA;

static ITEM_DATA Item[SPEEDUP_NUM];//�\���̂ŃA�C�e����������邩��錾

//�\���̂��񂷁i���[�v�j�����邽�߂̕ϐ�
static int Ui = 0;//�\���̂����[�v�����邽�߂̕ϐ�

void InitSpeedUp(void)
{
	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//�\���̂̐������� 
	{
		Item[Ui].pos.x = -500 + rand() % 1000;
		Item[Ui].pos.y = 100 + rand() % 500;
		Item[Ui].pos.z = 50 + rand() % 1900;
	}



	//������
	LPD3DXBUFFER  pAdjacencyBuffer;

	HRESULT hr;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();
	//���f�����O�f�[�^�̏ꏊ
	hr = D3DXLoadMeshFromX("resource\\xmodel\\taru.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		&pAdjacencyBuffer, &g_pMaterialBuffer, NULL,
		&g_nMaterialNum, &g_pMesh);

	if (FAILED(hr))
	{
		MessageBox(NULL, "Xmodel�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	}

	//�}�e���A���̎擾
	pMaterial = (LPD3DXMATERIAL)g_pMaterialBuffer->GetBufferPointer();



	hr = g_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT |
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

	g_pMesh->GetDeclaration(Elements);

	hr = g_pMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, Elements, pDevice, &pTemMesh);

	g_pMesh->Release();

	g_pMesh = pTemMesh;
};
void UninitSpeedUp(void)
{
	g_pMaterialBuffer->Release();
};
void UpdateSpeedUp(void)
{
	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//�\���̂̐������� 
	{
		atariitem(Ui, Item[Ui].pos.x, Item[Ui].pos.y, Item[Ui].pos.z);//�X�V�����{���x�̐��ƍ��W�������蔻��ɑ���

																	  //SRT
		D3DXMatrixIdentity(&Item[Ui].mtxworld);

		//D3DXMatrixScaling(&Item[Ui].mtxworldS, 1.0f, 1.0f, 1.0f);     //�g��֐�
		//D3DXMatrixMultiply(&Item[Ui].mtxworld, &Item[Ui].mtxworld, &Item[Ui].mtxworldS);//����  

		D3DXMatrixTranslation(&Item[Ui].mtxworldT, Item[Ui].pos.x, Item[Ui].pos.y, Item[Ui].pos.z);//���s�ړ��֐�
		D3DXMatrixMultiply(&Item[Ui].mtxworld, &Item[Ui].mtxworld, &Item[Ui].mtxworldT);//����  
	}
};
void DrawSpeedUp(void)
{

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (Ui = 0; Ui < SPEEDUP_NUM; Ui++)//�\���̂̐������� 
	{

		pDevice->SetTransform(D3DTS_WORLD, &Item[Ui].mtxworld);

		for (unsigned int i = 0; i < g_nMaterialNum; i++)
		{
			pMaterial[i].MatD3D.Ambient.r = 0.3f;
			pMaterial[i].MatD3D.Ambient.g = 0.3f;
			pMaterial[i].MatD3D.Ambient.b = 0.3f;
			pMaterial[i].MatD3D.Ambient.a = 1.0f;

			pDevice->SetMaterial(&pMaterial[i].MatD3D);


			g_pMesh->DrawSubset(i);
		}
	}
};
