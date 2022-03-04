#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>

//git test
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINES);
		glLineWidth(30.0);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(1.0, -0.3, 0.0);
		glVertex3f(-1.0, -0.3, 0.0);
	glEnd();
	glFlush();

}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 320);

	glutCreateWindow("fortress_test");
	glutDisplayFunc(display);
	glutMainLoop();
}