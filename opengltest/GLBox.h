#include "GLObject.h"

class GLBox
{
public:
	GLBox(float dx = 1, float dy = 1, float dz = 1)
	{
		Set(dx, dy, dz);
		for(int i = 0; i < 4; i++)
		{
			SetUV(i, 0, 0, 0);
		}
		for(int i = 0; i < 4; i++)
		{
			SetUV(i, 1, 1, 0);
		}
		for(int i = 0; i < 4; i++)
		{
			SetUV(i, 2, 1, 1);
		}
		for(int i = 0; i < 4; i++)
		{
			SetUV(i, 3, 0, 1);
		}
	}
	void Set(float dx, float dy, float dz);
	void SetTexture(int faceIndex, int texture);
	void SetUV(int faceIndex, int vertexIndex, float u, float v);
	void Draw();
protected:
	float vertexs[24];
	float uvs[24][2];
	int textures[24];
};