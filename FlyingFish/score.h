#ifndef _SCORE_H_
#define _SCORE_H_
//void DrawScore(int 表したいスコアー(数字), int スコアーの右枠, float 表示したいX座標, float 表示したいY座標, bool スコアーの右枠を0で埋めるかどうか)
void DrawScore(int nScore, int nDig, float x, float y, bool bZero = false);	//nDigは桁数

//#define DIG_SCORE			(2)

#endif
