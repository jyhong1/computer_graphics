#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include <math.h>

#define scaleFactor 0.01

using namespace std;
//rectangle is moving with the line.

class Ground {
public:
	float x1, x2, y1, y2;
	float width;

	void init() {
		x1 = -2.0;
		x2 = 2.0;
		y1 = -0.3;
		y2 = -0.3;
		width = 0.5;
	}
};

class GunBarrel : public Ground {
public:
	float dx, dy;

	void init() {
		x1 = 0.0;
		x2 = 0.3;
		y1 = 0.0;
		y2 = 0.2;
		width = 0.3;
		dx = 0;
		dy = 0;
	}

	float getX1() {
		return x1 + dx;
	}

	float getX2() {
		return x2 + dx;
	}

	float getY1() {
		return y1 + dy;
	}

	float getY2() {
		return y2 + dy;
	}
};

class LowerBody {
public:
	float x;
	float y;
	float width;
	float height;

	void init() {
		x = -0.3;
		y = -0.3;
		width = 0.3;
		height = 0.6;
	}
};

class Circle {
public:
	float r;
};

class UpperBody : public Circle {
public:
	float dx;

	void init() {
		dx = 0.0;
		r = 0.12;
	}
};

class CannonBall : public UpperBody {
public:
	float dy;
	float initSpeed;
	float t;
	bool isFlying;

	void init() {
		dx = 0.0;
		dy = 0.0;
		r = 0.05;
		isFlying = false;
		t = 0;
		initSpeed = 30;
	}
};

Ground ground;
GunBarrel gunBarrel;
LowerBody lowerBody;
UpperBody upperBody;
vector<CannonBall> cannonBalls;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0); //black
	glShadeModel(GL_FLAT);

	ground.init();
	gunBarrel.init();
	lowerBody.init();
	upperBody.init();
}

void reshape(int w, int h) {
	glViewport(0.0, 0.0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2, 2, -2, 2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	vector<CannonBall>::iterator c;
	for (c = cannonBalls.begin(); c != cannonBalls.end(); c++) {
		if (c->isFlying)
			glColor3f(1.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
		for (int i = 0; i < 360; i++) {
			float angle = i * 3.14159265 / 180;
			float x = c->r * cos(angle);
			float y = c->r * sin(angle);
			glVertex2f(x + c->dx, y + c->dy);
		}
		glEnd();
	}

	glBegin(GL_QUADS); //»ç°¢Çü
	glColor3f(1.0, 1.0, 1.0); //white
	glVertex2f(lowerBody.x, lowerBody.y);
	glVertex2f(lowerBody.x, lowerBody.y + lowerBody.width);
	glVertex2f(lowerBody.x + lowerBody.height, lowerBody.y + lowerBody.width);
	glVertex2f(lowerBody.x + lowerBody.height, lowerBody.y);
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float angle = i * 3.141592 / 180;
		float x = upperBody.r * cos(angle);
		float y = upperBody.r * sin(angle);
		glVertex2f(x + upperBody.dx, y);
	}
	glEnd();

	glLineWidth(gunBarrel.width);
	glBegin(GL_LINES); // gun barrel
	glColor3f(1.0, 1.0, 1.0); //white
	glVertex3f(gunBarrel.getX1(), gunBarrel.getY1(), 0.0);
	glVertex3f(gunBarrel.getX2(), gunBarrel.getY2(), 0.0);
	glEnd();

	glLineWidth(ground.width);
	glBegin(GL_LINES); // ¶¥(ground)
	glColor3f(1.0, 0.0, 0.0); //red
	glVertex3f(ground.x1, ground.y1, 0.0);
	glVertex3f(ground.x2, ground.y2, 0.0);
	glEnd();

	glutSwapBuffers();
}

void timer(int v) {
	vector<CannonBall>::iterator c;
	for (c = cannonBalls.begin(); c != cannonBalls.end(); c++) {
		if (c->isFlying) {
			c->t += 0.05;
			c->dx += scaleFactor * c->initSpeed * (gunBarrel.x2 - gunBarrel.x1);
			c->dy += scaleFactor * (-9.8 * c->t + c->initSpeed * (gunBarrel.y2 - gunBarrel.y1));
			if (c->dy - c->r <= -0.3) {
				c->isFlying = false;
				c->t = 0;
			}
			glutPostRedisplay();
		}
	}
	while (!cannonBalls.empty() && !cannonBalls.begin()->isFlying) {
		cannonBalls.erase(cannonBalls.begin());
	}

	glutTimerFunc(1000 / 60, timer, v);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == ' ') {
		CannonBall c;
		c.init();
		c.dx = gunBarrel.getX2();
		c.dy = gunBarrel.getY2();
		c.t = 0;
		c.isFlying = true;
		cannonBalls.push_back(c);
	}
	glutPostRedisplay();
}

void specialkeyboard(int key, int x, int y) { //moving tank
	switch (key) {
	case GLUT_KEY_RIGHT:
		lowerBody.x += 0.01;
		upperBody.dx += 0.01;
		gunBarrel.dx += 0.01;
		break;

	case GLUT_KEY_LEFT:
		lowerBody.x -= 0.01;
		upperBody.dx -= 0.01;
		gunBarrel.dx -= 0.01;
		break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);

	glutCreateWindow("fortress_test");
	init(); //essential

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer, 0);	//https://cs.lmu.edu/~ray/notes/openglexamples/ spinning square
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutMainLoop();
}