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
	float x;
	float y;
	float a;
}cir;

rect rectangle;
cir circle;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0); //black
	glShadeModel(GL_FLAT);
	
	rectangle.x = -0.3;
	rectangle.y = -0.3;
	rectangle.width = 0.3;
	rectangle.height = 0.6;

	circle.a = 0.0;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_QUADS); //사각형
		glColor3f(1.0, 1.0, 1.0); //white
		glVertex2f(rectangle.x, rectangle.y);
		glVertex2f(rectangle.x, rectangle.y + rectangle.width);
		glVertex2f(rectangle.x + rectangle.height, rectangle.y + rectangle.width);
		glVertex2f(rectangle.x + rectangle.height, rectangle.y);
	glEnd();

	glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
		for (int i = 0; i < 360; i++) {
			float angle = i * 3.141592 / 180;
			circle.x = 0.12 * cos(angle);
			circle.y = 0.12 * sin(angle);
			glVertex2f(circle.x + circle.a, circle.y);
		}
	glEnd();

	glLineWidth(5.0); //begin 전에 선언해줘야함.
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
		rectangle.x += 0.01;
		circle.a += 0.01;
		break;

	case GLUT_KEY_LEFT:
		rectangle.x -= 0.01;
		circle.a -= 0.01;
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