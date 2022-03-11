#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include <math.h>

using namespace std;

class Line {
protected:
	float x1, x2, y1, y2;
	float width;
public:
	void init(float x1, float x2, float y1, float y2, float width) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
		this->width = width;
	}

	float getX1() { return x1; }
	float getY1() { return y1; }
	float getX2() { return x2; }
	float getY2() { return y2; }
	void setX1(float x) { x1 = x; }
	void setY1(float y) { y1 = y; }
	void setX2(float x) { x2 = x; }
	void setY2(float y) { y2 = y; }
	void setWidth(float w) { width = w; }
};

class Ground : public Line { //¶¥(red line)
public:
	void init() {
		x1 = -2.0;
		x2 = 2.0;
		y1 = -0.31;
		y2 = -0.31;
		width = 0.5;
	}

	void draw() {
		glLineWidth(width);
		glBegin(GL_LINES); // ground
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(x1, y1, 0.0);
		glVertex3f(x2, y2, 0.0);
		glEnd();
	}
};

class GunBarrel : public Ground { //Æ÷½Å
protected:
	float dx, dy;

public:
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

	void draw() {
		glLineWidth(width);
		glBegin(GL_LINES); // Gun Barrel
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(getX1(), getY1(), 0.0);
		glVertex3f(getX2(), getY2(), 0.0);
		glEnd();
	}

	void move_dx(float dx) {
		this->dx += dx;
	}
};

class LowerBody { //ÅÊÅ© ÇÏºÎ
protected:
	float width;
	float height;
	float dx;
	float dy;

public:
	void init() {
		dx = -0.3;
		dy = -0.21;
		width = 0.6;
		height = 0.2;
	}

	void draw() {
		glBegin(GL_QUADS); //Lower Body
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(dx, dy);
		glVertex2f(dx, dy + height);
		glVertex2f(dx + width, dy + height);
		glVertex2f(dx + width, dy);
		glEnd();
	}

	float getWidth() { return width; };
	float getHeight() { return height; };
	float get_dx() { return dx; };
	float get_dy() { return dy; };

	void move_dx(float dx) {
		this->dx += dx;
	}
};

class Circle { //¿ø
protected:
	float r;
public:
	float getR() { return r; }
};

class UpperBody : public Circle { //ÅÊÅ© »óºÎ(¿ø)
protected:
	float dx;
	float dy;

public:
	void init() {
		dx = 0.0;
		dy = 0.0;
		r = 0.12;
	}

	void draw() {
		float angle, x, y;
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			angle = i * 3.141592 / 180;
			x = r * cos(angle);
			y = r * sin(angle);
			glVertex2f(x + dx, y);
		}
		glEnd();
	}

	float get_dx() { return dx; }
	float get_dy() { return dy; }

	void move_dx(float dx) { this->dx += dx; }
	void move_dy(float dy) { this->dy += dy; }
};

class CannonBall : public UpperBody { //Æ÷Åº
protected:
	float initSpeed;
	float t;
	bool isFlying;

public:
	void init(float dx, float dy) {
		this->dx = dx;
		this->dy = dy;
		r = 0.05;
		isFlying = false;
		t = 0;
		initSpeed = 30;
	}

	bool getIsFlying() { return isFlying; }
	float getInitSpeed() { return initSpeed; }
	float getT() { return t; }
	void setIsFlying(bool b) { isFlying = b; }
	void elapseTime() { t += 0.05; }

	void draw(float tank_dx, float tank_dy) {
		float angle, x, y;
		if (isFlying)
			glColor3f(1.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_POLYGON); // Canon Balls // https://blog.amaorche.com/25 drawing circle
		for (int i = 0; i < 360; i++) {
			angle = i * 3.14159265 / 180;
			x = r * cos(angle);
			y = r * sin(angle);
			glVertex2f(x + tank_dx + dx, y + tank_dy + dy);
		}
		glEnd();
	}
};

class Wheel : public UpperBody { //ÅÊÅ©ÀÇ ¹ÙÄû
protected:
	Line spoke;

public:
	void init(int i) {
		r = 0.05;
		dx = -0.3 + (2 * i + 1) * r;
		dy = -0.3 + r;
		spoke.init(-r, r, -0.3 + r, -0.3 + r, 3.0);
	}

	void draw(float tank_dx, float tank_dy) {
		float angle, x, y;
		glBegin(GL_POLYGON);	// wheels
		glColor3f(1.0, 1.0, 1.0);
		for (int i = 0; i < 360; i++) {
			angle = i * 3.141592 / 180;
			x = r * cos(angle);
			y = r * sin(angle);
			glVertex2f(x + dx, y + dy);
		}
		glEnd();

		/*¹ÙÄû»ì begin*/
		glLineWidth(3.0);
		glBegin(GL_LINE_LOOP);
		glColor3f(0.7, 0.7, 0.7);
		for (int i = 0; i < 360; i++) {
			angle = i * 3.141592 / 180;
			x = r * cos(angle);
			y = r * sin(angle);
			glVertex2f(x + dx, y + dy);
		}
		glEnd();

		for (int i = 0; i < 3; i++) {
			glTranslatef(dx, dy, 0);	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glTranslate.xml
			glRotatef(60.0f * i, 0.0f, 0.0f, 1.0f);	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml
			glTranslatef(-dx, -dy, 0);
			glBegin(GL_LINES); // spokes to show the rotation well
			glColor3f(0.7, 0.7, 0.7);
			glVertex3f(spoke.getX1() + dx, spoke.getY1(), 0.0);
			glVertex3f(spoke.getX2() + dx, spoke.getY2(), 0.0);
			glEnd();
			glLoadIdentity();
			glTranslatef(tank_dx, tank_dy, 0);
		}
		/*¹ÙÄû»ì end*/
	}

	void move_dx(float dx) {
		this->dx += dx;
	}
};

class Tank {
protected:
	GunBarrel gunBarrel;
	LowerBody lowerBody;
	UpperBody upperBody;
	vector<Wheel> wheels;
	float dx, dy;

public:
	void init() {
		gunBarrel.init();
		lowerBody.init();
		upperBody.init();
		for (int i = 0; i < 6; i++) {
			Wheel w;
			w.init(i);
			wheels.push_back(w);
		}
		dx = -1.0;
		dy = 0;
	}

	float get_dx() { return dx; }
	float get_dy() { return dy; }
	float gunBarrel_X2() { return gunBarrel.getX2(); }
	float gunBarrel_Y2() { return gunBarrel.getY2(); }
	float gunBarrel_cos() { return gunBarrel.getX2() - gunBarrel.getX1(); }
	float gunBarrel_sin() { return gunBarrel.getY2() - gunBarrel.getY1(); }
	float rightPos() { return lowerBody.get_dx() + lowerBody.getWidth(); }
	float leftPos() { return lowerBody.get_dx(); }

	void draw() {
		lowerBody.draw();
		upperBody.draw();
		gunBarrel.draw();
		vector<Wheel>::iterator w;
		for (w = wheels.begin(); w != wheels.end(); w++) {
			w->draw(dx, dy);
		}
	}

	void move_dx(float dx) {
		gunBarrel.move_dx(dx);
		lowerBody.move_dx(dx);
		upperBody.move_dx(dx);
		vector<Wheel>::iterator w;
		for (w = wheels.begin(); w != wheels.end(); w++) {
			w->move_dx(dx);
		}
	}
};