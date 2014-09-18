#include "GLBox.h"
#include <glew.h>

void GLBox::Set(float dx, float dy, float dz)
{
	vertexs[0] = -dx/2;vertexs[1] = dy/2;vertexs[2] = -dz/2;
	vertexs[3] = -dx/2;vertexs[4] = dy/2;vertexs[5] = dz/2;
	vertexs[6] = dx/2;vertexs[7] = dy/2;vertexs[8] = dz/2;
	vertexs[9] = dx/2;vertexs[10] = dy/2;vertexs[11] = -dz/2;
	vertexs[12] = -dx/2;vertexs[13] = -dy/2;vertexs[14] = -dz/2;
	vertexs[15] = -dx/2;vertexs[16] = -dy/2;vertexs[17] = dz/2;
	vertexs[18] = dx/2;vertexs[19] = -dy/2;vertexs[20] = dz/2;
	vertexs[21] = dx/2;vertexs[22] = -dy/2;vertexs[23] = -dz/2;
}

void GLBox::SetTexture(int faceIndex, int texture)
{
	if(faceIndex < 2)
	{
		for(int i = 0; i < 4; i++)
			textures[faceIndex*4+i] = texture;
	}
	else 
	{
		for(int i = 0; i < 2; i++)
		{
			textures[(faceIndex-2+i)%4] = texture;
			textures[(faceIndex-2+i)%4+4] = texture;
		}
	}
}

void GLBox::SetUV(int faceIndex, int vertexIndex, float u, float v)
{
	
	if(faceIndex < 2)
	{
		uvs[faceIndex*4+vertexIndex][0] = u;
		uvs[faceIndex*4+vertexIndex][1] = v;
	}
	else 
	{
		if(vertexIndex < 2)
		{
			uvs[(faceIndex-2+vertexIndex)%4][0] = u;
			uvs[(faceIndex-2+vertexIndex)%4][1] = v;
		}
		else
		{
			uvs[(faceIndex+1-vertexIndex)%4+4][0] = u;
			uvs[(faceIndex+1-vertexIndex)%4+4][1] = v;
		}
	}
}

void GLBox::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	//up
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexCoord2d(uvs[0][0],uvs[0][1]);
	glVertex3f(vertexs[0], vertexs[1], vertexs[2]);
	glTexCoord2d(uvs[1][0],uvs[1][1]);
	glVertex3f(vertexs[3], vertexs[4], vertexs[5]);
	glTexCoord2d(uvs[2][0], uvs[2][1]);
	glVertex3f(vertexs[6], vertexs[7], vertexs[8]);

	glTexCoord2d(uvs[3][0], uvs[3][1]);
	glVertex3f(vertexs[9], vertexs[10], vertexs[11]);
	glTexCoord2d(uvs[0][0],uvs[0][1]);
	glVertex3f(vertexs[0], vertexs[1], vertexs[2]);
	glTexCoord2d(uvs[2][0], uvs[2][1]);
	glVertex3f(vertexs[6], vertexs[7], vertexs[8]);

	//down
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glTexCoord2d(uvs[4][0],uvs[4][1]);
	glVertex3f(vertexs[12], vertexs[13], vertexs[14]);
	glTexCoord2d(uvs[5][0],uvs[5][1]);
	glVertex3f(vertexs[15], vertexs[16], vertexs[17]);
	glTexCoord2d(uvs[6][0], uvs[6][1]);
	glVertex3f(vertexs[18], vertexs[19], vertexs[20]);

	glTexCoord2d(uvs[7][0], uvs[7][1]);
	glVertex3f(vertexs[21], vertexs[22], vertexs[23]);
	glTexCoord2d(uvs[4][0],uvs[4][1]);
	glVertex3f(vertexs[12], vertexs[13], vertexs[14]);
	glTexCoord2d(uvs[6][0], uvs[6][1]);
	glVertex3f(vertexs[18], vertexs[19], vertexs[20]);

	//left
	glBindTexture(GL_TEXTURE_2D, textures[8]);
	glTexCoord2d(uvs[8][0],uvs[8][1]);
	glVertex3f(vertexs[0], vertexs[1], vertexs[2]);
	glTexCoord2d(uvs[11][0],uvs[11][1]);
	glVertex3f(vertexs[12], vertexs[13], vertexs[14]);
	glTexCoord2d(uvs[10][0], uvs[10][1]);
	glVertex3f(vertexs[15], vertexs[16], vertexs[17]);

	glTexCoord2d(uvs[10][0], uvs[10][1]);
	glVertex3f(vertexs[15], vertexs[16], vertexs[17]);
	glTexCoord2d(uvs[9][0], uvs[9][1]);
	glVertex3f(vertexs[3], vertexs[4], vertexs[5]);
	glTexCoord2d(uvs[8][0],uvs[8][1]);
	glVertex3f(vertexs[0], vertexs[1], vertexs[2]);
	
	//near
	glBindTexture(GL_TEXTURE_2D, textures[12]);
	glTexCoord2d(uvs[12][0],uvs[12][1]);
	glVertex3f(vertexs[3], vertexs[4], vertexs[5]);
	glTexCoord2d(uvs[15][0],uvs[15][1]);
	glVertex3f(vertexs[15], vertexs[16], vertexs[17]);
	glTexCoord2d(uvs[14][0], uvs[14][1]);
	glVertex3f(vertexs[18], vertexs[19], vertexs[20]);

	glTexCoord2d(uvs[14][0], uvs[14][1]);
	glVertex3f(vertexs[18], vertexs[19], vertexs[20]);
	glTexCoord2d(uvs[13][0], uvs[13][1]);
	glVertex3f(vertexs[6], vertexs[7], vertexs[8]);
	glTexCoord2d(uvs[12][0],uvs[12][1]);
	glVertex3f(vertexs[3], vertexs[4], vertexs[5]);
	
	//right
	glBindTexture(GL_TEXTURE_2D, textures[16]);
	glTexCoord2d(uvs[16][0],uvs[16][1]);
	glVertex3f(vertexs[6], vertexs[7], vertexs[8]);
	glTexCoord2d(uvs[19][0],uvs[19][1]);
	glVertex3f(vertexs[18], vertexs[19], vertexs[20]);
	glTexCoord2d(uvs[18][0], uvs[18][1]);
	glVertex3f(vertexs[21], vertexs[22], vertexs[23]);

	glTexCoord2d(uvs[18][0], uvs[18][1]);
	glVertex3f(vertexs[21], vertexs[22], vertexs[23]);
	glTexCoord2d(uvs[17][0], uvs[17][1]);
	glVertex3f(vertexs[9], vertexs[10], vertexs[11]);
	glTexCoord2d(uvs[16][0],uvs[16][1]);
	glVertex3f(vertexs[6], vertexs[7], vertexs[8]);

	//far
	glBindTexture(GL_TEXTURE_2D, textures[20]);
	glTexCoord2d(uvs[20][0],uvs[20][1]);
	glVertex3f(vertexs[9], vertexs[10], vertexs[11]);
	glTexCoord2d(uvs[23][0],uvs[23][1]);
	glVertex3f(vertexs[21], vertexs[22], vertexs[23]);
	glTexCoord2d(uvs[22][0], uvs[22][1]);
	glVertex3f(vertexs[12], vertexs[13], vertexs[14]);

	glTexCoord2d(uvs[22][0], uvs[22][1]);
	glVertex3f(vertexs[12], vertexs[13], vertexs[14]);
	glTexCoord2d(uvs[21][0], uvs[21][1]);
	glVertex3f(vertexs[0], vertexs[1], vertexs[2]);
	glTexCoord2d(uvs[20][0],uvs[20][1]);
	glVertex3f(vertexs[9], vertexs[10], vertexs[11]);
	glEnd();
}