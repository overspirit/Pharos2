#include "PreCompile.h"
#include "CoreGlobal.h"
#include "RenderGlobal.h"
#include "SceneGlobal.h"
#include "DesktopGlobal.h"
#include "Global.h"

Noise2D::Noise2D()
{
	m_persistence = 0.5f;
	m_octaves = 4;

	m_width = 0;
	m_height = 0;
}

Noise2D::~Noise2D()
{

}

float Noise2D::Noise(int x, int y)    // 根据(x,y)获取一个初步噪声值  
{	
	return m_metaNoise[(y + 1) * m_width + (x + 1)];
}

float Noise2D::SmoothedNoise(int x, int y)   //光滑噪声  
{
	float corners = (Noise(x - 1, y - 1) + Noise(x + 1, y - 1) + Noise(x - 1, y + 1) + Noise(x + 1, y + 1)) / 16;
	float sides = (Noise(x - 1, y) + Noise(x + 1, y) + Noise(x, y - 1) + Noise(x, y + 1)) / 8;
	float center = Noise(x, y) / 4;
	return corners + sides + center;
}

float Noise2D::Cosine_Interpolate(float a, float b, float x)  // 余弦插值  
{
	float ft = x * 3.1415927f;
	float f = (1 - cos(ft)) * 0.5f;
	return a * (1 - f) + b * f;
}

float Noise2D::InterpolatedNoise(float x, float y)   // 获取插值噪声  
{
	int integer_X = int(x);
	float  fractional_X = x - integer_X;
	int integer_Y = int(y);
	float fractional_Y = y - integer_Y;
	float v1 = SmoothedNoise(integer_X, integer_Y);
	float v2 = SmoothedNoise(integer_X + 1, integer_Y);
	float v3 = SmoothedNoise(integer_X, integer_Y + 1);
	float v4 = SmoothedNoise(integer_X + 1, integer_Y + 1);
	float i1 = Cosine_Interpolate(v1, v2, fractional_X);
	float i2 = Cosine_Interpolate(v3, v4, fractional_X);
	return Cosine_Interpolate(i1, i2, fractional_Y);
}

float Noise2D::PerlinNoise(float x, float y)    // 最终调用：根据(x,y)获得其对应的Noise2D值  
{
	float total = 0;
	float p = m_persistence;
	int n = m_octaves;

	for (int i = 0; i < n; i++)
	{
		float frequency = pow(2.0f, i);
		float amplitude = pow(p, i);
		total = total + InterpolatedNoise(x * frequency, y * frequency) * amplitude;
	}

	return total;
}

float* Noise2D::GenerateNoiseData()
{
	m_metaNoise.resize(m_width * m_height);

	for (uint32 i = 0; i < m_metaNoise.size(); i++)
	{
		m_metaNoise[i] = Math::random(-1.0f, 1.0f);
	}

	m_noiseData.resize(m_width * m_height);

	for (int32 i = 0; i < m_width * m_height; i++)
	{
		m_noiseData[i] = PerlinNoise(i / m_width / pow(2.0f, m_octaves), i % m_width / pow(2.0f, m_octaves));
	}
	
	m_metaNoise.clear();

	return &*m_noiseData.begin();
}