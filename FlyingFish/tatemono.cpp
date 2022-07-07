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
#include "tatemono.h"

//X���f���\���悤�֐��̏�����
static LPD3DXBUFFER   g_pTatemonoMaterialBuffer = NULL;
static DWORD          g_nTatemonoMaterialNum = 0;
static LPD3DXMESH     g_pTatemonoMesh = NULL;
static LPD3DXMATERIAL pTatemonoMaterial;
static LPD3DXMESH     pTatemonoTemMesh;


typedef struct {
	D3DXVECTOR3 pos;//���W
	D3DXMATRIX mtxworld;//
	D3DXMATRIX mtxworldS;//�g�k�p�ϐ�
	D3DXMATRIX mtxworldR;//�g�k�p�ϐ�
	D3DXMATRIX mtxworldT;//�ړ��p�֐�
	bool hihixyouzitatemono;//�\����\��
}TATEMONO_DATA;

static TATEMONO_DATA Tatemono[TATEMONO_NUM];//�\���̂ŃA�C�e����������邩��錾

								//�\���̂��񂷁i���[�v�j�����邽�߂̕ϐ�
static int Ui = 0;//�\���̂����[�v�����邽�߂̕ϐ�

void Inittatemono(void)
{

	for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//�\���̂̐������� 
	{
		Tatemono[Ui].pos.x = -500 + rand() % 1000;
		Tatemono[Ui].pos.y = 0;
		Tatemono[Ui].pos.z = 500 + rand() % 1500;
	}



	//������
	LPD3DXBUFFER  pAdjacencyBuffer;

	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	//���f�����O�f�[�^�̏ꏊ
	HRESULT hr;

	hr = D3DXLoadMeshFromX("resource\\xmodel\\biru2.x",
		D3DXMESH_SYSTEMMEM, pDevice,
		&pAdjacencyBuffer, &g_pTatemonoMaterialBuffer, NULL,
		&g_nTatemonoMaterialNum, &g_pTatemonoMesh);


	if (FAILED(hr))
	{
		MessageBox(NULL, "kane�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
	}

	//�}�e���A���̎擾
	pTatemonoMaterial = (LPD3DXMATERIAL)g_pTatemonoMaterialBuffer->GetBufferPointer();



	hr = g_pTatemonoMesh->OptimizeInplace(D3DXMESHOPT_COMPACT |
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

	g_pTatemonoMesh->GetDeclaration(Elements);

	hr = g_pTatemonoMesh->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, Elements, pDevice, &pTatemonoTemMesh);

	g_pTatemonoMesh->Release();

	g_pTatemonoMesh = pTatemonoTemMesh;
};
void Uninittatemono(void)
{
	g_pTatemonoMaterialBuffer->Release();
};
void Updatetatemono(void)
{
	for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//�\���̂̐������� 
	{
		ataritatemono(Ui, Tatemono[Ui].pos.x, Tatemono[Ui].pos.y, Tatemono[Ui].pos.z);//�X�V�����{���x�̐��ƍ��W�������蔻��ɑ���

																	  //SRT
		D3DXMatrixIdentity(&Tatemono[Ui].mtxworld);

		D3DXMatrixScaling(&Tatemono[Ui].mtxworldS, 3.0f, 3.0f, 3.0f);     //�g��֐�
		D3DXMatrixMultiply(&Tatemono[Ui].mtxworld, &Tatemono[Ui].mtxworld, &Tatemono[Ui].mtxworldS);//����  

		D3DXMatrixTranslation(&Tatemono[Ui].mtxworldT, Tatemono[Ui].pos.x, Tatemono[Ui].pos.y, Tatemono[Ui].pos.z);//���s�ړ��֐�
		D3DXMatrixMultiply(&Tatemono[Ui].mtxworld, &Tatemono[Ui].mtxworld, &Tatemono[Ui].mtxworldT);//����  
	}
};
void Drawtatemono(void)
{
	LPDIRECT3DDEVICE9 pDevice = getD3DDevice();

	for (Ui = 0; Ui < TATEMONO_NUM; Ui++)//�\���̂̐������� 
	{



		for (unsigned int i = 0; i < g_nTatemonoMaterialNum; i++)
		{
			pTatemonoMaterial[i].MatD3D.Ambient.r = 0.3f;
			pTatemonoMaterial[i].MatD3D.Ambient.g = 0.3f;
			pTatemonoMaterial[i].MatD3D.Ambient.b = 0.3f;
			pTatemonoMaterial[i].MatD3D.Ambient.a = 1.0f;

			pDevice->SetMaterial(&pTatemonoMaterial[i].MatD3D);

			pDevice->SetTransform(D3DTS_WORLD, &Tatemono[Ui].mtxworld);

			g_pTatemonoMesh->DrawSubset(i);
		}
	}
};
