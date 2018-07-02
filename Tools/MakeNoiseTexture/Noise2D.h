#pragma once

class Noise2D
{
public:
	Noise2D();
	~Noise2D();

private:
	float		m_persistence;
	int			m_octaves;

	int			m_width;
	int			m_height;

	vector<float>		m_metaNoise;
	vector<float>		m_noiseData;

private:
	float Noise(int x, int y);    // 根据(x,y)获取一个初步噪声值 
	float SmoothedNoise(int x, int y);   //光滑噪声 
	float Cosine_Interpolate(float a, float b, float x); // 余弦插值                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	float InterpolatedNoise(float x, float y);   // 获取插值噪声  

	float PerlinNoise(float x, float y);    // 最终调用：根据(x,y)获得其对应的PerlinNoise值  
	
public:
	void SetPersistence(float value) { m_persistence = value; }
	float GetPersistence() { return m_persistence; }

	void SetOctaves(int value) { m_octaves = value; }
	int GetOctaves() { return m_octaves; }

	void SetSize(int width, int height) { m_width = width; m_height = height; }

	float* GenerateNoiseData();
};