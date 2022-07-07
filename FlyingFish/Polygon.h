//������������������������������������������������
//�|���S���`��t�@�C��
//������������������������������������������������
#ifndef _POLYGON_H_
#define _POLYGON_H_



//������������������������������������������������
//�\���̐錾
//������������������������������������������������
typedef struct
{
	char fileName[256];
	float Width;
	float Height;
	float texWidth;
	float texHeight;
}TEXTURE;

enum
{
	TEXTURE_UNKNOW = -1,
	TEXTURE_HOROCUBE,//�L���[�u�p�e�N�X�`��
	TEXTURE_FIELD,//���b�V���t�B�[���h�p�e�N�X�`��
	TEXTURE_SHADOW,//�v���C���[�̉e�e�N�X�`��
	TEXTURE_MAX
};

static const  TEXTURE g_aTextures[] =
{
	{ "resource\\texture\\meisai.jpg",1024,1024,1024,1024 },//�L���[�u�p�e�N�X�`��
	{ "resource\\texture\\field.jpg",160,160,160,160 },//���b�V���t�B�[���h�p�e�N�X�`��
	{ "resource\\texture\\effect.jpg",80,80,80,80 },//�v���C���[�̉e�e�N�X�`��
};

//������������������������������������������������
//�v���g�^�C�v�錾
//������������������������������������������������
bool InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void SetTexture(int texNum);//�e�N�X�`���ԍ�
void SetScale(float fDouble);//�{�ɂ������l
void SetPolygonColor(D3DCOLOR color);
void PrimitiveDrawLine(float x0, float y0, float x1, float y1);

void SetPolygonRotation(float cx, float cy, float angle);			//��]���S���W, "" , �p�x
void DrawPolygon(float dx, float dy, int tcx, int tcy, int nTurn, bool bAffine = false); //���_���W��,���_���W��,��,����,�؂�����W��,�؂�����W��,���]�̌���0(��)1(�t), ��]���邩�ǂ���

#endif 