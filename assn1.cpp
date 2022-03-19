#include "objects.h"

Tank tank;
Ground ground;
vector<CannonBall> cannonBalls;

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	ground.init();
	tank.init();
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
	float angle, x, y;
	glClear(GL_COLOR_BUFFER_BIT);

	ground.draw();

	glPushMatrix();
	glTranslatef(tank.get_dx(), tank.get_dy(), 0.0);
	tank.draw();

	vector<CannonBall>::iterator c;
	for (c = cannonBalls.begin(); c != cannonBalls.end(); c++) { //Æ÷Åº
		c->draw();
	}
	glPopMatrix();

	glutSwapBuffers();
}

void timer(int v) {
	vector<CannonBall>::iterator c;
	float angle = tank.gunBarrel_theta() * PI / 180;
	for (c = cannonBalls.begin(); c != cannonBalls.end(); c++) {
		if (c->getIsFlying()) {
			c->elapseTime();
			c->move_dx(scaleFactor * c->getInitSpeed() * tank.gunBarrel_length() * cos(angle));
			c->move_dy(scaleFactor * (-9.8 * c->getT() + c->getInitSpeed() * tank.gunBarrel_length() * sin(angle)));
			if (c->get_dy() - c->getR() < -0.3) {
				c->setIsFlying(false);
			}
			glutPostRedisplay();
		}
	}
	while (!cannonBalls.empty() && !cannonBalls.begin()->getIsFlying()) {
		cannonBalls.erase(cannonBalls.begin());
	}

	glutTimerFunc(1000 / 50, timer, v);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case ' ': 	// shooting cannon balls
			CannonBall c;
			c.init(tank.gunBarrel_X2(), tank.gunBarrel_Y2(), tank.gunBarrel_InitialSpeed());
			c.setIsFlying(true);
			cannonBalls.push_back(c);
			break;

		case 'w': //move gunbarrel counter clockwise
			tank.chage_theta(3.0);
			break;

		case 's': //move gunbarrel clockwise
			tank.chage_theta(-3.0);
			break;

		case 'e': //power up
			tank.gunBarrel_chageInitialSpeed(3.0);
			break;

		case 'q': //power down
			tank.gunBarrel_chageInitialSpeed(-3.0);
			break;
	}

	glutPostRedisplay();
}

void specialkeyboard(int key, int x, int y) { //moving tank
	vector<Wheel>::iterator w;
	switch (key) {
	case GLUT_KEY_RIGHT:
		if (tank.rightPos() >= ground.getX2())
			break;
		tank.move_dx(0.01);
		break;

	case GLUT_KEY_LEFT:
		if (tank.leftPos() <= ground.getX1())
			break;
		tank.move_dx(-0.01);
		break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 700);

	glutCreateWindow("fortress_assn1");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer, 0);	//https://cs.lmu.edu/~ray/notes/openglexamples/ spinning square examples for moving cannonBalls
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutMainLoop();
}