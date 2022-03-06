#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include <math.h>

//rectangle is moving with the line.

class Line {
public:
	float x1, x2, y1, y2;
	float width;
};

class GunBarrel : public Line {
	float dx, dy;
};

class Rect {
public:
	float x;
	float y;
	float width;
	float height;
};

class Circle {
public:
	float r;
};

class UpperBody : public Circle {
public:
	float dx;
};

class CannonBall : public UpperBody {
public:
	float dy;
	float initSpeed;
	float t;
	bool isFlying;
};

Rect lowerBody;
UpperBody upperBody;
CannonBall cannonBall;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0); //black
	glShadeModel(GL_FLAT);

	lowerBody.x = -0.3;
	lowerBody.y = -0.3;
	lowerBody.width = 0.3;
	lowerBody.height = 0.6;

	upperBody.dx = 0.0;
	upperBody.r = 0.12;

	cannonBall.dx = 0.0;
	cannonBall.dy = 0.0;
	cannonBall.r = 0.05;
	cannonBall.isFlying = false;
	cannonBall.t = 0;
	cannonBall.initSpeed = 30;
}

void reshape(int w, int h) {
	glViewport(0.0, 0.0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (cannonBall.isFlying)
		glColor3f(1.0, 1.0, 1.0);
	else
		glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
	for (int i = 0; i < 360; i++) {
		float angle = i * 3.141592653589793238462643383279502884197169399375105820974944 / 180;
		float x = cannonBall.r * cos(angle);
		float y = cannonBall.r * sin(angle);
		glVertex2f(x + cannonBall.dx, y + cannonBall.dy);
	}
	glEnd();

	glBegin(GL_QUADS); //»ç°¢Çü
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

	glLineWidth(3.0);
	glBegin(GL_LINES); // gun barrel
	glColor3f(1.0, 1.0, 1.0); //white
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.3, 0.2, 0.0);
	glEnd();

	glLineWidth(5.0);
	glBegin(GL_LINES); // ¶¥(ground)
	glColor3f(1.0, 0.0, 0.0); //red
	glVertex3f(1.0, -0.3, 0.0);
	glVertex3f(-1.0, -0.3, 0.0);
	glEnd();

	glutSwapBuffers();
}

void timer(int v) {
	if (cannonBall.isFlying) {
		cannonBall.t += 1;
		cannonBall.dx += 0.05;
		cannonBall.dy += 0.001 * (-4.9 * cannonBall.t * cannonBall.t + 30 * cannonBall.t);
		if (cannonBall.dy <= -0.3) {
			cannonBall.isFlying = false;
			cannonBall.t = 0;
		}
		glutPostRedisplay();
	}
	glutTimerFunc(1000 / 60, timer, v);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == ' ') {
		cannonBall.dx = 0;
		cannonBall.dy = 0;
		cannonBall.t = 0;
		cannonBall.isFlying = true;
	}
	glutPostRedisplay();
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
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer, 0);	//https://cs.lmu.edu/~ray/notes/openglexamples/ spinning square
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutMainLoop();

}