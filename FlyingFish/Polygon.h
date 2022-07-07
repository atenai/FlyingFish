//━━━━━━━━━━━━━━━━━━━━━━━━
//ポリゴン描画ファイル
//━━━━━━━━━━━━━━━━━━━━━━━━
#ifndef _POLYGON_H_
#define _POLYGON_H_



//━━━━━━━━━━━━━━━━━━━━━━━━
//構造体宣言
//━━━━━━━━━━━━━━━━━━━━━━━━
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
	TEXTURE_HOROCUBE,//キューブ用テクスチャ
	TEXTURE_FIELD,//メッシュフィールド用テクスチャ
	TEXTURE_SHADOW,//プレイヤーの影テクスチャ
	TEXTURE_MAX
};

static const  TEXTURE g_aTextures[] =
{
	{ "resource\\texture\\meisai.jpg",1024,1024,1024,1024 },//キューブ用テクスチャ
	{ "resource\\texture\\field.jpg",160,160,160,160 },//メッシュフィールド用テクスチャ
	{ "resource\\texture\\effect.jpg",80,80,80,80 },//プレイヤーの影テクスチャ
};

//━━━━━━━━━━━━━━━━━━━━━━━━
//プロトタイプ宣言
//━━━━━━━━━━━━━━━━━━━━━━━━
bool InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void SetTexture(int texNum);//テクスチャ番号
void SetScale(float fDouble);//倍にしたい値
void SetPolygonColor(D3DCOLOR color);
void PrimitiveDrawLine(float x0, float y0, float x1, float y1);

void SetPolygonRotation(float cx, float cy, float angle);			//回転中心座標, "" , 角度
void DrawPolygon(float dx, float dy, int tcx, int tcy, int nTurn, bool bAffine = false); //頂点座標ｘ,頂点座標ｙ,幅,高さ,切り取る座標ｘ,切り取る座標ｙ,反転の向き0(正)1(逆), 回転するかどうか

#endif 