#include "3DShapes.h"
#include "glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/libx86/glew32.lib")

void Shapes::CreateCube_Direct()
{
	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);
		glVertex3d(0, 0, 1);//A
		glVertex3d(1, 0, 1);//B
		glVertex3d(0, 1, 1);//C

		glVertex3d(0, 1, 1);//C
		glVertex3d(1, 0, 1);//B
		glVertex3d(1, 1, 1);//D

		glVertex3d(1, 0, 1);//B
		glVertex3d(1, 0, 0);//F
		glVertex3d(1, 1, 1);//D

		glVertex3d(1, 0, 0);//F
		glVertex3d(1, 1, 0);//H
		glVertex3d(1, 1, 1);//D

		glVertex3d(0, 1, 0);//G
		glVertex3d(0, 1, 1);//C
		glVertex3d(1, 1, 1);//D

		glVertex3d(1, 1, 1);//D
		glVertex3d(1, 1, 0);//H
		glVertex3d(0, 1, 0);//G

		glVertex3d(0, 0, 0);//E
		glVertex3d(1, 0, 0);//F
		glVertex3d(0, 1, 0);//G

		glVertex3d(0, 0, 0);//E
		glVertex3d(0, 0, 1);//A
		glVertex3d(0, 1, 0);//G

		glVertex3d(0, 0, 1);//A
		glVertex3d(0, 1, 1);//C
		glVertex3d(0, 1, 0);//G

		glVertex3d(1, 0, 0);//F
		glVertex3d(0, 0, 0);//E
		glVertex3d(1, 1, 0);//H

		glVertex3d(0, 0, 0);//E
		glVertex3d(0, 1, 0);//G
		glVertex3d(1, 1, 0);//H

		glVertex3d(0, 0, 1);//A
		glVertex3d(0, 0, 0);//E
		glVertex3d(1, 0, 0);//F

		glVertex3d(1, 0, 1);//B
		glVertex3d(0, 0, 1);//A
		glVertex3d(1, 0, 0);//F
	glEnd();
}
