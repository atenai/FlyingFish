#include <windows.h>
#include "Number2.h"
#include <d3dx9.h>



//void DrawScore(int 表したいスコアー(数字), int スコアーの右枠, float 表示したいX座標, float 表示したいY座標, bool スコアーの右枠を0で埋めるかどうか)
void DrawScore2(int nScore, int nDig, float x, float y, bool bZero)
{
	//左詰め
	if (nDig < 0)//nDigが0より小さいとき
	{
		nDig = 0;//nDigは0
		for (;;)//無限ループ
		{
			nScore /= 10;//nScore ÷= 10
			if (nScore == 0)//もしnScoreが0なら
			{
				break;
			}
			nDig++;
		}
	}

	//カンスト値
	int maxScore = 10;

	for (int i = 1; i < nDig; i++)
	{
		maxScore *= 10;
	}
	maxScore--;

	nScore = min(maxScore, nScore);

	for (int i = 1; i <= nDig; i++)
	{
		int n = nScore % 10;
		DrawNumber2(n, x + NUMBER_WIDTH2 * (nDig - i), y);
		nScore /= 10;
		if (nScore == 0 && !bZero)
		{
			break;
		}
	}


}