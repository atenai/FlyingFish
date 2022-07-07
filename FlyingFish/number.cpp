#include <d3dx9.h>
#include "Number.h"
#include "Polygon.h"
#include "UI.h"
#include "scene.h"
//void DrawNumber(int 表したい数, float 表示するX座標, float 表示するY座標)
void DrawNumber(int n, float x, float y)
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
	DrawUI(TEX_10, x, y, NUMBER_WIDTH, NUMBER_HEIGHT, 0 + 200 * nX, 0 + 250 * nY, 200, 250);//テクスチャの描画
}