#include <d3dx9.h>
#include "Number2.h"
#include "Polygon.h"
#include "UI.h"
#include "scene.h"
//void DrawNumber(int 表したい数, float 表示するX座標, float 表示するY座標)
void DrawNumber2(int n, float x, float y)
{
	int nX = 0, nY = 0;
	if (n < 0 || n>9)
	{
		return;
	}
	if (n > 4)
	{
		nX = n - 5;
		nY = 1;
	}
	else
	{
		nX = n;
		nY = 0;
	}
	SetPolygonColor(D3DCOLOR_RGBA(255, 255, 255, 255));//ポリゴンカラーの設定
	DrawUI(TEX_10, x, y, NUMBER_WIDTH2, NUMBER_HEIGHT2, 0 + 200 * nX, 0 + 250 * nY, 200, 250);//テクスチャの描画
}

//29は画像の数字１つ分の横の長さ29×5は145
//32は画像の数字１つ分の縦の長さ32×2は64