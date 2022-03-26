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

void draw_life(int life, float x_start) {
	float angle, x, y, r = 0.05;

	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();
	glTranslatef(x_start, -0.7, 0.0);


	for (int j = 0; j < life; j++) {
		glPushMatrix();

		glTranslatef(j *(2 * (r + 0.03)), 0.0, 0.0);
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

	glPushMatrix();
	glTranslatef(tank.get_dx(), tank.get_dy(), 0.0);
	tank.draw();

	vector<CannonBall>::iterator c;
	for (c = cannonBalls.begin(); c != cannonBalls.end(); c++) { //Æ÷Åº
		c->draw();
	}
	glPopMatrix();

	draw_life(tank.getLife(), -3.3);

	glutSwapBuffers();
}

void timer(int v) {
	vector<CannonBall>::iterator c;
	float angle = tank.gunBarrel_theta() * PI / 180;
	for (c = cannonBalls.begin(); c != cannonBalls.end(); c++) {
		if (c->getIsFlying()) {
			c->elapseTime();
			c->move_dx(scaleFactor * c->getSpeed() * tank.gunBarrel_length() * cos(angle));
			c->move_dy(scaleFactor * (-9.8 * c->getT() + c->getSpeed() * tank.gunBarrel_length() * sin(angle)));
			if (c->get_dy() - c->getR() < -0.3 || tank.is_in(c->get_dx(), c->get_dy())) {
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
		c.init(tank.gunBarrel_X2(), tank.gunBarrel_Y2(), tank.gunBarrel_Speed());
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
	glutInitWindowPosition(0.0, 0.0);
	glutInitWindowSize(1500, 900);

	glutCreateWindow("fortress_assn1");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer, 0);	//https://cs.lmu.edu/~ray/notes/openglexamples/ spinning square examples for moving cannonBalls
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialkeyboard);
	glutMainLoop();
}