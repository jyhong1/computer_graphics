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
		x1 = -0.5;
		x2 = 3.0;
		y1 = -0.31;
		y2 = -0.31;
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
		y = -0.21;
		width = 0.6;
		height = 0.2;
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

class Wheel : public UpperBody {
public:
	float dy;
	float line1_x;
	float line1_y;

	float line2_x;
	float line2_y;

	void init(int i) {
		r = 0.05;
		dx = -0.3 + (2 * i + 1) * r;
		dy = -0.3 + r;
		line1_x = -r;
		line1_y = -0.3 + r;
		line2_x = r;
		line2_y = -0.3 + r;
	}
};

Ground ground;
GunBarrel gunBarrel;
LowerBody lowerBody;
UpperBody upperBody;
vector<CannonBall> cannonBalls;
vector<Wheel> wheels;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0); //black
	glShadeModel(GL_FLAT);

	ground.init();
	gunBarrel.init();
	lowerBody.init();
	upperBody.init();
	for (int i = 0; i < 6; i++) {
		Wheel w;
		w.init(i);
		wheels.push_back(w);
	}
}

void reshape(int w, int h) {
	glViewport(0.0, 0.0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(ground.x1, ground.x2, -1.0, -1.0 + ground.x2 - ground.x1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display() {
	float angle, x, y;
	glClear(GL_COLOR_BUFFER_BIT);

	vector<CannonBall>::iterator c;
	for (c = cannonBalls.begin(); c != cannonBalls.end(); c++) {
		if (c->isFlying)
			glColor3f(1.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
		for (int i = 0; i < 360; i++) {
			angle = i * 3.14159265 / 180;
			x = c->r * cos(angle);
			y = c->r * sin(angle);
			glVertex2f(x + c->dx, y + c->dy);
		}
		glEnd();
	}

	glBegin(GL_QUADS); //�簢��
	glColor3f(1.0, 1.0, 1.0); //white
	glVertex2f(lowerBody.x, lowerBody.y);
	glVertex2f(lowerBody.x, lowerBody.y + lowerBody.height);
	glVertex2f(lowerBody.x + lowerBody.width, lowerBody.y + lowerBody.height);
	glVertex2f(lowerBody.x + lowerBody.width, lowerBody.y);
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		angle = i * 3.141592 / 180;
		x = upperBody.r * cos(angle);
		y = upperBody.r * sin(angle);
		glVertex2f(x + upperBody.dx, y);
	}
	glEnd();

	glLineWidth(gunBarrel.width);
	glBegin(GL_LINES); // gun barrel
	glColor3f(1.0, 1.0, 1.0); //white
	glVertex3f(gunBarrel.getX1(), gunBarrel.getY1(), 0.0);
	glVertex3f(gunBarrel.getX2(), gunBarrel.getY2(), 0.0);
	glEnd();

	vector<Wheel>::iterator w;
	for (w = wheels.begin(); w != wheels.end(); w++) {
		glBegin(GL_POLYGON);
		glColor3f(1.0, 1.0, 1.0);
		for (int i = 0; i < 360; i++) {
			angle = i * 3.141592 / 180;
			x = w->r * cos(angle);
			y = w->r * sin(angle);
			glVertex2f(x + w->dx, y + w->dy);
		}
		glEnd();

		glLineWidth(3.0);
		glBegin(GL_LINE_LOOP);
		glColor3f(0.7, 0.7, 0.7);
		for (int i = 0; i < 360; i++) {
			angle = i * 3.141592 / 180;
			x = w->r * cos(angle);
			y = w->r * sin(angle);
			glVertex2f(x + w->dx, y + w->dy);
		}
		glEnd();

		glBegin(GL_LINES); // ���� ��
		glColor3f(0.7, 0.7, 0.7);
		glVertex3f(w->line1_x + w->dx, w->line1_y, 0.0);
		glVertex3f(w->line2_x + w->dx, w->line2_y, 0.0);
		glEnd();

		glTranslatef(w->dx, w->dy, 0);
		glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-w->dx, -w->dy, 0);
		glBegin(GL_LINES); // ���� ��
		glColor3f(0.7, 0.7, 0.7);
		glVertex3f(w->line1_x + w->dx, w->line1_y, 0.0);
		glVertex3f(w->line2_x + w->dx, w->line2_y, 0.0);
		glEnd();
		glLoadIdentity();

		glTranslatef(w->dx, w->dy, 0);
		glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
		glTranslatef(-w->dx, -w->dy, 0);
		glBegin(GL_LINES); // ���� ��
		glColor3f(0.7, 0.7, 0.7);
		glVertex3f(w->line1_x + w->dx, w->line1_y, 0.0);
		glVertex3f(w->line2_x + w->dx, w->line2_y, 0.0);
		glEnd();
		glLoadIdentity();
	}

	glLineWidth(ground.width);
	glBegin(GL_LINES); // ��(ground)
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
			if (c->dy - c->r < -0.3) {
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
	vector<Wheel>::iterator w;
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (lowerBody.x + lowerBody.width >= ground.x2)
			break;
		lowerBody.x += 0.01;
		upperBody.dx += 0.01;
		gunBarrel.dx += 0.01;
		for (w = wheels.begin(); w != wheels.end(); w++) {
			w->dx += 0.01;
		}
		break;

	case GLUT_KEY_LEFT:
		if (lowerBody.x <= ground.x1)
			break;
		lowerBody.x -= 0.01;
		upperBody.dx -= 0.01;
		gunBarrel.dx -= 0.01;
		for (w = wheels.begin(); w != wheels.end(); w++) {
			w->dx -= 0.01;
		}
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