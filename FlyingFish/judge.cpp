#include <math.h>
#include "judge.h"

//‹…‘Ì‚Æ‹…‘Ì
bool JudgementCheck(float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2, float fR1, float fR2)
{
	float fdX, fdY, fdZ, fL;

	fdX = fX2 - fX1;
	fdY = fY2 - fY1;
	fdZ = fZ2 - fZ1;
	fL = fR1 + fR2;

	if (fdX*fdX + fdY * fdY + fdZ * fdZ <= fL*fL)
	{
		return true;
	}
	else
	{
		return false;
	}
}