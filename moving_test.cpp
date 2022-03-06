#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include <math.h>

//rectangle is moving with the line.

typedef struct rect {
	float x;
	float y;
	float width;
	float height;
}rect;

typedef struct circle {
	float dx;
	float r;
}cir;

rect lowerBody;
cir upperBody;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0); //black
	glShadeModel(GL_FLAT);

	lowerBody.x = -0.3;
	lowerBody.y = -0.3;
	lowerBody.width = 0.3;
	lowerBody.height = 0.6;

	upperBody.dx = 0.0;
	upperBody.r = 0.12;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS); //사각형
	glColor3f(1.0, 1.0, 1.0); //white
	glVertex2f(lowerBody.x, lowerBody.y);
	glVertex2f(lowerBody.x, lowerBody.y + lowerBody.width);
	glVertex2f(lowerBody.x + lowerBody.height, lowerBody.y + lowerBody.width);
	glVertex2f(lowerBody.x + lowerBody.height, lowerBody.y);
	glEnd();

	glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
	for (int i = 0; i < 360; i++) {
		float angle = i * 3.141592 / 180;
		float x = upperBody.r * cos(angle);
		float y = upperBody.r * sin(angle);
		glVertex2f(x + upperBody.dx, y);
	}
	glEnd();

	glLineWidth(5.0); //begin 전에 선언해줘야함. 왜?
	glBegin(GL_LINES); // 땅(ground)
	glColor3f(1.0, 0.0, 0.0); //red
	glVertex3f(1.0, -0.3, 0.0);
	glVertex3f(-1.0, -0.3, 0.0);
	glEnd();



	glutSwapBuffers();
}

void specialkeyboard(int key, int x, int y) { //moving tank
	switch (key) {
	case GLUT_KEY_RIGHT:
		lowerBody.x += 0.01;
		upperBody.dx += 0.01;
		break;

	case GLUT_KEY_LEFT:
		lowerBody.x -= 0.01;
		upperBody.dx -= 0.01;
		break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 400);

	glutCreateWindow("fortress_test");
	init(); //essential

	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutSpecialFunc(specialkeyboard);
	glutMainLoop();

}