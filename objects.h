#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>
#include <math.h>

#define scaleFactor 0.01
#define PI 3.141592

using namespace std;

class Line {
protected:
	float length, width;
public:
	void init(float length, float width) {
		this->length = length;
		this->width = width;
	}

	float getLength() { return length; }
};

class Ground : public Line { //¶¥(red line)
protected:
	float x1, x2, y1, y2;
public:
	void init() {
		x1 = -2.0;
		x2 = 2.0;
		y1 = -0.31;
		y2 = -0.31;
		width = 0.5;
	}

	float getX1() { return x1; }
	float getX2() { return x2; }

	void draw() {
		glLineWidth(width);
		glBegin(GL_LINES); // ground
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(x1, y1, 0.0);
		glVertex3f(x2, y2, 0.0);
		glEnd();
	}
};

class GunBarrel : public Line { //Æ÷½Å
protected:
	float theta, dx, dy;
public:
	void init() {
		width = 0.3;
		length = 0.33;
		theta = 30;
		dx = 0;
		dy = 0;
	}

	float getTheta() { return theta; }
	float getX2() { return length * cos(theta * PI / 180) + dx; }
	float getY2() { return length * sin(theta * PI / 180) + dy; }

	void draw() {
		glPushMatrix();
		glTranslatef(dx, dy, 0.0);
		glRotatef(theta, 0.0f, 0.0f, 1.0f);
		glLineWidth(width);
		glBegin(GL_LINES); // Gun Barrel
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(length, 0.0, 0.0);
		glEnd();
		glPopMatrix();
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
		glPushMatrix();
		glTranslatef(dx, dy, 0.0);
		glBegin(GL_QUADS); //Lower Body
		glColor3f(1.0, 1.0, 1.0);
		glVertex2f(0.0, 0.0);
		glVertex2f(0.0, height);
		glVertex2f(width, height);
		glVertex2f(width, 0.0);
		glEnd();
		glPopMatrix();
	}

	float getWidth() { return width; };
	float get_dx() { return dx; };

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
		glPushMatrix();
		glTranslatef(dx, dy, 0);
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

	void move_dx(float dx) { this->dx += dx; }
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
	float get_dy() { return dy; }
	void setIsFlying(bool b) { isFlying = b; }
	void elapseTime() { t += 0.05; }
	void move_dy(float dy) { this->dy += dy; }

	void draw() {
		float angle, x, y;
		if (isFlying)
			glColor3f(1.0, 1.0, 0.0);
		else
			glColor3f(0.0, 0.0, 0.0);
		glPushMatrix();
		glTranslatef(dx, dy, 0.0);
		glBegin(GL_POLYGON); // Canon Balls // https://blog.amaorche.com/25 drawing circle
		for (int i = 0; i < 360; i++) {
			angle = i * PI / 180;
			x = r * cos(angle);
			y = r * sin(angle);
			glVertex2f(x, y);
		}
		glEnd();
		glPopMatrix();
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
		spoke.init(2 * r, 3.0);
	}

	void draw() {
		float angle, x, y;
		glPushMatrix();
		glTranslatef(dx, dy, 0.0);
		glBegin(GL_POLYGON);	// wheels
		glColor3f(1.0, 1.0, 1.0);
		for (int i = 0; i < 360; i++) {
			angle = i * PI / 180;
			x = r * cos(angle);
			y = r * sin(angle);
			glVertex2f(x, y);
		}
		glEnd();

		/*¹ÙÄû»ì begin*/
		glLineWidth(3.0);
		glBegin(GL_LINE_LOOP);
		glColor3f(0.7, 0.7, 0.7);
		for (int i = 0; i < 360; i++) {
			angle = i * PI / 180;
			x = r * cos(angle);
			y = r * sin(angle);
			glVertex2f(x, y);
		}
		glEnd();

		for (int i = 0; i < 3; i++) {
			glPushMatrix();
			glRotatef(60.0f * i, 0.0f, 0.0f, 1.0f);	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml
			glBegin(GL_LINES); // spokes to show the rotation well
			glColor3f(0.7, 0.7, 0.7);
			glVertex3f(-spoke.getLength() / 2, 0.0, 0.0);
			glVertex3f(spoke.getLength() / 2, 0.0, 0.0);
			glEnd();
			glPopMatrix();
		}
		glPopMatrix();
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
	float gunBarrel_length() { return gunBarrel.getLength(); }
	float gunBarrel_X2() { return gunBarrel.getX2(); }
	float gunBarrel_Y2() { return gunBarrel.getY2(); }
	float gunBarrel_theta() { return gunBarrel.getTheta(); }
	float rightPos() { return lowerBody.get_dx() + lowerBody.getWidth() + dx; }
	float leftPos() { return lowerBody.get_dx() + dx; }

	void draw() {
		lowerBody.draw();
		upperBody.draw();
		gunBarrel.draw();
		vector<Wheel>::iterator w;
		for (w = wheels.begin(); w != wheels.end(); w++) {
			w->draw();
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