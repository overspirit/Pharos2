// MakeNoiseTexture.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define FLT_MAX 3.402823466e+38F 

double PerlinNoise(float x, float y);

int main()
{
	//  for(double d = -100.0; d < 100.0; d += 0.001)  
	//      printf("%lf\n", noise1(d) );  

	double* arr = new double[1000 * 1000];

	float min = FLT_MAX;
	float max = -FLT_MAX;

	for (int i = 0; i < 1000; i++)
	{
		float r = PerlinNoise(i / 100.0f, 0);
		printf("%lf\n", r);
		arr[i] = r;

		if (r < min) min = r;
		if (r > max) max = r;
	}

// 	for (double d1 = -100.0, d2 = -100.0, d3 = -100.0; d1 < 100 && d2 < 100 && d3 < 100; d1 += 0.001, d2 += 0.001, d3 += 0.001)
// 	{
// 		float vec[3] = { d1,d2,d3 };
// 		printf("%lf\n", noise3(vec));
// 	}

    return 0;
}

