#include "objects.h"

Tank tank1, tank2;
Ground ground;
bool isEnd = false;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	ground.init();
	tank1.init(-2.0, 1.0, 1.0, 1.0);
	tank2.init(-2.0, 1.0, 0.0, 0.0);
}

void draw_life(int life, float x_start) {
	float angle, x, y, r = 0.05;

	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glTranslatef(x_start, -0.7, 0.0);


	for (int j = 0; j < life; j++) {
		glPushMatrix();

		glTranslatef(j * (2 * (r + 0.03)), 0.0, 0.0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			angle = i * PI / 180;
			x = r * cos(angle);
			y = r * sin(angle);
			glVertex2f(x, y);
		}
		glEnd();

		glPopMatrix();
	}

	glPopMatrix();
}

void reshape(int w, int h) {
	glViewport(0.0, 0.0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(ground.getX1(), ground.getX2(), -1.0, -1.0 + ground.getX2() - ground.getX1());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	ground.draw();
	tank1.draw();

	glPushMatrix();
	glScalef(-1.0, 1.0, 0.0);
	tank2.draw();
	glPopMatrix();

	draw_life(tank1.getLife(), -3.3);
	draw_life(tank2.getLife(), 3.0);

	isEnd = (tank1.getLife() <= 0) || (tank2.getLife() <= 0);
	if (isEnd) {
		//Text Rendering http://openglut.sourceforge.net/group__bitmapfont.html
		glRasterPos2f(0, 3.0);
		if (tank1.getLife() <= 0)
			glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"LOSE\nPress Space to Restart");
		else
			glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)"WIN\nPress Space to Restart");
	}
	glutSwapBuffers();
}

void timer(int v) {
	if (!isEnd) {
		tank1.update(&tank2);
		tank2.update(&tank1);
	}
	glutTimerFunc(1000 / 50, timer, v);
}

void tank2UpdateStateTimer(int v) {
	if (!isEnd) {
		if (!tank1.getFail()) {
			int a = rand() % 2;
			int randVal[2] = { -1, 1 };
			tank2.chage_theta(3.0 * v);
			tank2.gunBarrel_chageSpeed(3.0 * randVal[a]);
		}
		else {
			tank2.gunBarrel_setThetaToInit();
			tank2.gunBarrel_setSpeedToInit();
		}
		glutPostRedisplay();
	}
	if (tank2.gunBarrel_theta() <= 15 || tank2.gunBarrel_theta() >= 60)
		glutTimerFunc(403, tank2UpdateStateTimer, -v);
	else
		glutTimerFunc(403, tank2UpdateStateTimer, v);
}

void tank2FireTimer(int v) {
	if (!isEnd) {
		CannonBall e;

		tank2.addCannonBall();
	}
	glutTimerFunc(3000, tank2FireTimer, v);
}

void keyboard(unsigned char key, int x, int y) {
	if (!isEnd) {
		switch (key) {
		case ' ': 	// shooting cannon balls
			tank1.addCannonBall();
			break;

		case 'w': //move gunbarrel counter clockwise
			tank1.chage_theta(3.0);
			break;

		case 's': //move gunbarrel clockwise
			tank1.chage_theta(-3.0);
			break;

		case 'e': //power up
			tank1.gunBarrel_chageSpeed(3.0);
			break;

		case 'q': //power down
			tank1.gunBarrel_chageSpeed(-3.0);
			break;

		case 'c': //all pass
			tank2.changeFail();
			break;

		case 'f': // all fail
			tank1.changeFail();
			break;
		}
		glutPostRedisplay();
	}
	else {
		if (key == ' ') {	//restart
			isEnd = false;
			init();
		}
	}
}

void specialkeyboard(int key, int x, int y) { //moving tank
	if (!isEnd) {
		switch (key) {
		case GLUT_KEY_RIGHT:
			if (tank1.rightPos() >= ground.getX2() || tank1.rightPos() >= -tank2.rightPos())
				break;
			tank1.move_dx(0.01);
			break;

		case GLUT_KEY_LEFT:
			if (tank1.leftPos() <= ground.getX1())
				break;
			tank1.move_dx(-0.01);
			break;
		}
		glutPostRedisplay();
	}
}

void main(int argc, char** argv) {
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0.0, 0.0);
	glutInitWindowSize(1500, 900);

	glutCreateWindow("fortress_assn2");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer, 0);	//https://cs.lmu.edu/~ray/notes/openglexamples/ spinning square examples for moving cannonBalls
	glutTimerFunc(100, tank2UpdateStateTimer, 1);
	glutTimerFunc(3000, tank2FireTimer, 0);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutMainLoop();
}