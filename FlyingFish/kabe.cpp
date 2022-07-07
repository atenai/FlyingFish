#include "coin.h"
#include "main.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "polygon.h"
#include "camera.h"
#include "input.h"
#include "judge.h"
#include "xmodel.h"
#include "kabe.h"
#include <time.h>

//X���f���\���悤�֐��̏�����
static LPD3DXBUFFER   g_pCoinMaterialBuffer = NULL;
static DWORD          g_nCoinMaterialNum = 0;
static LPD3DXMESH     g_pCoinMesh = NULL;
static LPD3DXMATERIAL pCoinMaterial;
static LPD3DXMESH     pCoinTemMesh;


typedef struct {
	D3DXVECTOR3 pos;//���W
	D3DXMATRIX mtxworld;//
	D3DXMATRIX mtxworldS;//�g�k�p�ϐ�
	D3DXMATRIX mtxworldR;//�g�k�p�ϐ�
	D3DXMATRIX mtxworldT;//�ړ��p�֐�
	bool hihixyouzikabe;//�{���x�̕\����\��
}KABE_DATA;

static KABE_DATA Kabe[KABE_NUM];//�\���̂ŃA�C�e����������邩��錾

							//�\���̂��񂷁i���[�v�j�����邽�߂̕ϐ�
static int Ui = 0;//�\���̂����[�v�����邽�߂̕ϐ�

void Initkabe(void)
{

	for (Ui = 0; Ui < KABE_NUM; Ui++)//�\���̂̐������� 
	{
		Kabe[Ui].pos.x = -500 + rand() % 1000;
		Kabe[Ui].pos.y = 50 + rand() % 500;
		Kabe[Ui].pos.z = 50 + rand() % 2800;
	}

	

	//������
	LPD3DXBUFFER  pAdjacencyBuffer;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//���f�����O�f�[�^�̏ꏊ
	HRESULT hr;

	hr = D3DXLoadMeshFromX("resource\\xmodel\\aokane.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		&pAdjacencyBuffer, &g_pCoinMaterialBuffer, NULL,
		&g_nCoinMaterialNum, &g_pCoinMesh);


	if (FAILED(hr))
	{
		MessageBox(NULL, "kane�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
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
void Uninitkabe(void)
{
	g_pCoinMaterialBuffer->Release();
};
void Updatekabe(void)
{
	for (Ui = 0; Ui < KABE_NUM; Ui++)//�\���̂̐������� 
	{
		atarikabe(Ui, Kabe[Ui].pos.x, Kabe[Ui].pos.y, Kabe[Ui].pos.z);//�X�V�����{���x�̐��ƍ��W�������蔻��ɑ���

																		//SRT
		D3DXMatrixIdentity(&Kabe[Ui].mtxworld);

		//D3DXMatrixScaling(&Kabe[Ui].mtxworldS, 10.0f, 10.0f, 1.0f);     //�g��֐�
		//D3DXMatrixMultiply(&Kabe[Ui].mtxworld, &Kabe[Ui].mtxworld, &Kabe[Ui].mtxworldS);//����  

		D3DXMatrixTranslation(&Kabe[Ui].mtxworldT, Kabe[Ui].pos.x, Kabe[Ui].pos.y, Kabe[Ui].pos.z);//���s�ړ��֐�
		D3DXMatrixMultiply(&Kabe[Ui].mtxworld, &Kabe[Ui].mtxworld, &Kabe[Ui].mtxworldT);//����  
	}
};
void Drawkabe(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (Ui = 0; Ui < KABE_NUM; Ui++)//�\���̂̐������� 
	{



		for (unsigned int i = 0; i < g_nCoinMaterialNum; i++)
		{
			pCoinMaterial[i].MatD3D.Ambient.r = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.g = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.b = 0.3f;
			pCoinMaterial[i].MatD3D.Ambient.a = 1.0f;

			pDevice->SetMaterial(&pCoinMaterial[i].MatD3D);

			pDevice->SetTransform(D3DTS_WORLD, &Kabe[Ui].mtxworld);

			g_pCoinMesh->DrawSubset(i);
		}
	}
};
