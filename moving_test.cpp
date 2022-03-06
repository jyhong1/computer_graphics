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

class Wheel : public Circle {
public:
	float dx;
	float line1_x;
	float line1_y;

	float line2_x;
	float line2_y;
};

Rect lowerBody;
UpperBody upperBody;
CannonBall cannonBall;
Wheel wheel1;
Wheel wheel2;
Wheel wheel3;
Wheel wheel4;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0); //black
	glShadeModel(GL_FLAT);

	wheel1.dx = -0.2;
	wheel1.r = 0.06;
	wheel1.line1_x = -wheel1.r + wheel1.dx;
	wheel1.line1_y = -0.3 + wheel1.r;
	wheel1.line2_x = wheel1.r + wheel1.dx;
	wheel1.line2_y = -0.3 + wheel1.r;

	wheel2.dx = -0.2 + 0.12;
	wheel2.r = 0.06;
	wheel2.line1_x = -wheel2.r + wheel2.dx;
	wheel2.line1_y = -0.3 + wheel2.r;
	wheel2.line2_x = wheel2.r + wheel2.dx;
	wheel2.line2_y = -0.3 + wheel2.r;

	wheel3.dx = -0.2 + 0.24;
	wheel3.r = 0.06;
	wheel3.line1_x = -wheel3.r + wheel3.dx;
	wheel3.line1_y = -0.3 + wheel3.r;
	wheel3.line2_x = wheel3.r + wheel3.dx;
	wheel3.line2_y = -0.3 + wheel3.r;

	wheel4.dx = -0.2 + 0.36;
	wheel4.r = 0.06;
	wheel4.line1_x = -wheel4.r + wheel4.dx;
	wheel4.line1_y = -0.3 + wheel4.r;
	wheel4.line2_x = wheel4.r + wheel4.dx;
	wheel4.line2_y = -0.3 + wheel4.r;

	lowerBody.x = -0.3;
	lowerBody.y = -0.3 + (wheel1.r * 2);
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
	gluOrtho2D(-1, 1, -1, 1);
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
		glVertex2f(x + upperBody.dx, y + (wheel1.r * 2));
	}
	glEnd();

	/*making wheel begin*/
	glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
	glColor3f(1.0, 1.0, 0.7); //ivory

	for (int i = 0; i < 360; i++) {
		float angle = i * 3.141592 / 180;
		float x = wheel1.r * cos(angle);
		float y = wheel1.r * sin(angle);
		glVertex2f(x + wheel1.dx, y - 0.3 + wheel1.r);
	}
	glEnd();

	glLineWidth(3.0);
	glBegin(GL_LINES); // ¹ÙÄû »ì
	glColor3f(0.0, 0.0, 0.0); //white
	glVertex3f(wheel1.line1_x, wheel1.line1_y, 0.0);
	glVertex3f(wheel1.line2_x, wheel1.line2_y, 0.0);
	glEnd();

	glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
	glColor3f(1.0, 1.0, 0.7); //ivory

	for (int i = 0; i < 360; i++) {
		float angle = i * 3.141592 / 180;
		float x = wheel2.r * cos(angle);
		float y = wheel2.r * sin(angle);
		glVertex2f(x + wheel2.dx, y - 0.3 + wheel2.r);
	}
	glEnd();

	glLineWidth(3.0);
	glBegin(GL_LINES); // ¹ÙÄû »ì
	glColor3f(0.0, 0.0, 0.0); //white
	glVertex3f(wheel2.line1_x, wheel2.line1_y, 0.0);
	glVertex3f(wheel2.line2_x, wheel2.line2_y, 0.0);
	glEnd();

	glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
	glColor3f(1.0, 1.0, 0.7); //ivory

	for (int i = 0; i < 360; i++) {
		float angle = i * 3.141592 / 180;
		float x = wheel3.r * cos(angle);
		float y = wheel3.r * sin(angle);
		glVertex2f(x + wheel3.dx, y - 0.3 + wheel3.r);
	}
	glEnd();

	glLineWidth(3.0);
	glBegin(GL_LINES); // ¹ÙÄû »ì
	glColor3f(0.0, 0.0, 0.0); //white
	glVertex3f(wheel3.line1_x, wheel3.line1_y, 0.0);
	glVertex3f(wheel3.line2_x, wheel3.line2_y, 0.0);
	glEnd();

	glBegin(GL_POLYGON); //https://blog.amaorche.com/25 circle
	glColor3f(1.0, 1.0, 0.7); //ivory

	for (int i = 0; i < 360; i++) {
		float angle = i * 3.141592 / 180;
		float x = wheel4.r * cos(angle);
		float y = wheel4.r * sin(angle);
		glVertex2f(x + wheel4.dx, y - 0.3 + wheel4.r);
	}
	glEnd();

	glLineWidth(3.0);
	glBegin(GL_LINES); // ¹ÙÄû »ì
	glColor3f(0.0, 0.0, 0.0); //white
	glVertex3f(wheel4.line1_x, wheel4.line1_y, 0.0);
	glVertex3f(wheel4.line2_x, wheel4.line2_y, 0.0);
	glEnd();

	/*making wheel end*/


	glLineWidth(3.0);
	glBegin(GL_LINES); // gun barrel
	glColor3f(1.0, 1.0, 1.0); //white
	glVertex3f(0.0, 0.0 + (wheel1.r*2), 0.0);
	glVertex3f(0.3, 0.2 + (wheel1.r * 2), 0.0);
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
		cannonBall.t += 0.1;
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
		wheel1.dx += 0.01;
		wheel1.line1_x += 0.01;
		wheel1.line2_x += 0.01;
		wheel2.dx += 0.01;
		wheel2.line1_x += 0.01;
		wheel2.line2_x += 0.01;
		wheel3.dx += 0.01;
		wheel3.line1_x += 0.01;
		wheel3.line2_x += 0.01;
		wheel4.dx += 0.01;
		wheel4.line1_x += 0.01;
		wheel4.line2_x += 0.01;
		break;

	case GLUT_KEY_LEFT:
		lowerBody.x -= 0.01;
		upperBody.dx -= 0.01;
		wheel1.dx -= 0.01;
		wheel1.line1_x -= 0.01;
		wheel1.line2_x -= 0.01;
		wheel2.dx -= 0.01;
		wheel2.line1_x -= 0.01;
		wheel2.line2_x -= 0.01;
		wheel3.dx -= 0.01;
		wheel3.line1_x -= 0.01;
		wheel3.line2_x -= 0.01;
		wheel4.dx -= 0.01;
		wheel4.line1_x -= 0.01;
		wheel4.line2_x -= 0.01;
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