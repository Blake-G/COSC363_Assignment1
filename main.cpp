#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include <string>

using namespace std;

void flashTimer(int value);
void drawRivets(char loc, GLfloat width, GLfloat height, GLfloat depth);

struct Color
{
	float r;
	float g;
	float b;
	float baser;
	float baseg;
	float baseb;
};

Color robotFeatures{ 0.3, 0.0, 0.0, 0.3 };
//Color robotBase{ 0.0, 0.2, 0.05, 0.0, 0.2, 0.05 }; //Green robot
Color robotBase{ 0.4, 0.4, 0.4 }; //Grey robot

// Globals
GLfloat pi = 3.1415926535897;
GLfloat cam_hgt = 30;
GLfloat cam_pos = 0;

GLfloat theta = (pi/2);
GLfloat zoom = 50;
GLfloat zoomSpeed = 1;
GLfloat cameraX = 0;
GLfloat cameraY = 0;
GLfloat cameraZ = 0;
bool getFlash = false;
bool getPause = false;
int flashPauseCount = 0;
bool keys[256];
bool specials[200];

// Draw Floor
void drawFloor()
{
	float white[4] = { 1., 1., 1., 1. };
	float black[4] = { 0 };
	bool flag = false;
	//glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glBegin(GL_QUADS);
	
	glNormal3f(0, 1, 0);
	for (int x = -50; x <= 50; x += 1)
	{
		for (int z = -50; z <= 50; z += 1)
		{
			if (flag) glColor3f(0.2, 0.2, 0.2);
			else glColor3f(0.4, 0.4, 0.4);
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z + 1);
			glVertex3f(x + 1, 0, z + 1);
			glVertex3f(x + 1, 0, z);
			flag = !flag;
		}
	}
	glEnd();
	//glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

void drawTorso()
{
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(robotBase.r, robotBase.g, robotBase.b);
	glTranslatef(0, 1, 0);

	GLfloat torsoWidth = 8;
	GLfloat torsoHeight = 11;
	GLfloat torsoDepth = 5;

	//Robot Torso
	//Front
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0,           0,				0);
	glVertex3f(0,			torsoHeight,	0);
	glVertex3f(torsoWidth,	torsoHeight,	0);
	glVertex3f(torsoWidth,	0,				0);
	//Back
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0,           0,				torsoDepth);
	glVertex3f(0,			torsoHeight,	torsoDepth);
	glVertex3f(torsoWidth,	torsoHeight,	torsoDepth);
	glVertex3f(torsoWidth,	0,				torsoDepth);
	//Right
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0,			0,				0);
	glVertex3f(0,			torsoHeight,	0);
	glVertex3f(0,			torsoHeight,	torsoDepth);
	glVertex3f(0,			0,				torsoDepth);
	//Left
	glNormal3f(1.0, 0.0, -1.0);
	glVertex3f(torsoWidth,	0, 0);
	glVertex3f(torsoWidth,	torsoHeight,	0);
	glVertex3f(torsoWidth,	torsoHeight,	torsoDepth);
	glVertex3f(torsoWidth,	0,				torsoDepth);
	//Top
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0,			torsoHeight,	0);
	glVertex3f(0,			torsoHeight,	torsoDepth);
	glVertex3f(torsoWidth,	torsoHeight,	torsoDepth);
	glVertex3f(torsoWidth,	torsoHeight,	0);
	glEnd();

	//Torso rivets
	drawRivets('f', torsoWidth, torsoHeight, 0.0);
	drawRivets('f', torsoWidth, torsoHeight, torsoDepth);
	drawRivets('s', torsoDepth, torsoHeight, 0.0);
	drawRivets('s', torsoDepth, torsoHeight, torsoWidth);
	drawRivets('t', torsoWidth, torsoDepth, torsoHeight);

	glPopMatrix();
}

void getRotate(char loc, GLfloat rotate)
{
	if (loc == 'f' || loc == 's') {
		glRotatef(rotate, 0.0, 1.0, 0.0);
	}
	else if (loc == 't' || loc == 'b') {
		glRotatef(rotate, 1.0, 0.0, 0.0);
	}
}

void drawRivet(char loc, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat outerSize = 1;
	GLfloat innerSize = 0.75;
	GLfloat xAdjust = 0;
	GLfloat yAdjust = 0;
	GLfloat zAdjust = 0;
	GLfloat rotate = 0;

	if (z == 0 && loc == 'f') {
		zAdjust -= 0.01;
	}
	else if (z > 0 && loc == 'f') {
		zAdjust += 0.01;
	}
	else if (x == 0 && loc == 's') {
		xAdjust -= 0.01;
		rotate = 90;
	}
	else if (x > 0 && loc == 's') {
		xAdjust += 0.01;
		rotate = -90;
	}
	else if (y == 0 && loc == 't' || loc == 'b') {
		yAdjust -= 0.01;
		rotate = 90;
	}
	else if (y > 0 && loc == 't' || loc == 'b') {
		yAdjust += 0.01;
		rotate = -90;
	}

	for (int i = 0; i < 2; i++) {
		glPushMatrix();
		if (i == 0) {
			glColor3f(robotBase.r/2, robotBase.g/2, robotBase.b/2);
			glTranslatef(x, y, z);
			getRotate(loc, rotate);
			glScalef(1.0, 1.0, 0.2);
			glutSolidSphere(0.15 * outerSize, 30, 15);
		}
		else {
			glColor3f(robotBase.r*1.2, robotBase.g*1.2, robotBase.b*1.2);
			glTranslatef(x + xAdjust, y + yAdjust, z + zAdjust);
			getRotate(loc, rotate);
			glScalef(1.0, 1.0, 0.2);
			glutSolidSphere(0.15 * innerSize, 30, 15);
		}
		glPopMatrix();
	}
}

void drawRivets(char loc, GLfloat width, GLfloat height, GLfloat depth)
{
	if (loc == 'f') {
		drawRivet('f', 0.25, 0.25, depth);
		drawRivet('f', width - 0.25, 0.25, depth);
		drawRivet('f', width - 0.25, height - 0.25, depth);
		drawRivet('f', 0.25, height - 0.25, depth);
	}
	else if (loc == 's') {
		drawRivet('s', depth, 0.25, 0.25);
		drawRivet('s', depth, 0.25, width - 0.25);
		drawRivet('s', depth, height - 0.25, width - 0.25);
		drawRivet('s', depth, height - 0.25, 0.25);
	}
	else if (loc == 't') {
		drawRivet('t', 0.25, depth, 0.25);
		drawRivet('t', width - 0.25, depth, 0.25);
		drawRivet('t', width - 0.25, depth, height - 0.25);
		drawRivet('t', 0.25, depth, height - 0.25);
	}
	else if (loc == 'b') {
		drawRivet('t', 0.25, depth, 0.25);
		drawRivet('t', width - 0.25, depth, 0.25);
		drawRivet('t', width - 0.25, depth, height - 0.25);
		drawRivet('t', 0.25, depth, height - 0.25);
	}
}

void drawHeadBase()
{
	glBegin(GL_QUADS);
	//glColor3f(robotBase.r, robotBase.g, robotBase.b);

	GLfloat headWidth = 6;
	GLfloat headHeight = 5;
	GLfloat headDepth = 3;


	//Head base
	//Front
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, headHeight, 0.0);
	glVertex3f(headWidth, headHeight, 0.0);
	glVertex3f(headWidth, 0.0, 0.0);
	//Back
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, headDepth);
	glVertex3f(0.0, headHeight, headDepth);
	glVertex3f(headWidth, headHeight, headDepth);
	glVertex3f(headWidth, 0.0, headDepth);
	//Right
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, headHeight, 0.0);
	glVertex3f(0.0, headHeight, headDepth);
	glVertex3f(0.0, 0.0, headDepth);
	//Left
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(headWidth, 0.0, 0.0);
	glVertex3f(headWidth, headHeight, 0.0);
	glVertex3f(headWidth, headHeight, headDepth);
	glVertex3f(headWidth, 0.0, headDepth);
	//Top
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, headHeight, 0.0);
	glVertex3f(0.0, headHeight, headDepth);
	glVertex3f(headWidth, headHeight, headDepth);
	glVertex3f(headWidth, headHeight, 0.0);
	//Bottom
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, headDepth);
	glVertex3f(headWidth, 0.0, headDepth);
	glVertex3f(headWidth, 0.0, 0.0);

	glEnd();

	//Head rivets //A couple of discs in each corner of the head
	drawRivets('f', headWidth, headHeight, 0.0);
	drawRivets('f', headWidth, headHeight, headDepth);
	drawRivets('s', headDepth, headHeight, 0.0);
	drawRivets('s', headDepth, headHeight, headWidth);
	drawRivets('t', headWidth, headDepth, headHeight);
	drawRivets('b', headWidth, headDepth, 0.0);
}

void drawEyes()
{
	glPushMatrix();
	//glColor3f(robotFeatures.r, robotFeatures.g, robotFeatures.b);
	glColor3f(0.7, 0.7, 0.0);
	glTranslatef(4.0, 3.5, 0.25);
	glutSolidSphere(0.65, 30, 15);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.0, 3.5, 0.25);
	glutSolidSphere(0.65, 30, 15);
	glPopMatrix();
}

void drawMouth()
{
	glColor3f(robotFeatures.r, robotFeatures.g, robotFeatures.b);
	glPushMatrix();
	glTranslatef(1.80, 1.7, 0.5);
	
	glRotatef(90, 0, 1, 0);
	glScalef(0.8, 0.8, 0.8);

	glPushMatrix();
	//glTranslatef(1.5, 1.5, 0.5);
	
	glutSolidCylinder(1.0, 3.0, 30, 15);
	glPopMatrix();

	//Mouth side bits
	glPushMatrix();
	//glTranslatef(1.6, 1.5, 0.5);
	glutSolidSphere(1.0, 30, 15);
	glPopMatrix();
	glPushMatrix();
	//glColor3f(robotFeatures.r, 1.0, robotFeatures.b);
	glTranslatef(0.0, 0.0, 3.0);
	glutSolidSphere(1.0, 30, 15);
	glPopMatrix();

	//Mouth bars //A series of black cylinders over the current mouth cylinder
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.07);
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glutSolidCylinder(1.01, 0.15, 30, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.6);
	glutSolidCylinder(1.01, 0.15, 30, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.2);
	glutSolidCylinder(1.01, 0.15, 30, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.8);
	glutSolidCylinder(1.01, 0.15, 30, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 2.4);
	glutSolidCylinder(1.01, 0.15, 30, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 3.0);
	glutSolidCylinder(1.01, 0.15, 30, 15);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void drawAerial()
{
	glPushMatrix();
	glTranslatef(2.5, 5.0, 1.0);
	glBegin(GL_TRIANGLE_STRIP);
	//glColor3f(robotBase.r, robotBase.g, robotBase.b);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.25, 0.5, 0.0);
	glVertex3f(1.25, 0.5, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(0.25, 1.5, 0.0);
	glVertex3f(1.25, 1.5, 0.0);
	glVertex3f(0.0, 2.0, 0.0);
	glVertex3f(1.0, 2.0, 0.0);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(1.0, 0.0, 1.0);
	glVertex3f(0.25, 0.5, 1.0);
	glVertex3f(1.25, 0.5, 1.0);
	glVertex3f(0.0, 1.0, 1.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(0.25, 1.5, 1.0);
	glVertex3f(1.25, 1.5, 1.0);
	glVertex3f(0.0, 2.0, 1.0);
	glVertex3f(1.0, 2.0, 1.0);
	glEnd();

	//glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUAD_STRIP);
	glNormal3f(-1.0, 0.0, 0.0);
	GLfloat xOffset = 0.25;
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(xOffset, 0.5, 0.0);
	glVertex3f(xOffset, 0.5, 1.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 1.0);
	glVertex3f(xOffset, 1.5, 0.0);
	glVertex3f(xOffset, 1.5, 1.0);
	glVertex3f(0.0, 2.0, 0.0);
	glVertex3f(0.0, 2.0, 1.0);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	glNormal3f(1.0, 0.0, 0.0);
	xOffset = 1.25;
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 1.0);
	glVertex3f(xOffset, 0.5, 0.0);
	glVertex3f(xOffset, 0.5, 1.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 1.0);
	glVertex3f(xOffset, 1.5, 0.0);
	glVertex3f(xOffset, 1.5, 1.0);
	glVertex3f(1.0, 2.0, 0.0);
	glVertex3f(1.0, 2.0, 1.0);
	glEnd();

	glColor3f(robotFeatures.r, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(0.5, 2.5, 0.5);
	//glutWireSphere(0.9, 30, 15);
	glutSolidSphere(0.9, 30, 15);
	glPopMatrix();
	

	glPopMatrix();
}

void drawHead()
{
	glColor3f(robotBase.r, robotBase.g, robotBase.b);
	drawHeadBase();
	glColor3f(robotBase.r, robotBase.g, robotBase.b);
	drawAerial();
	drawEyes();
	drawMouth(); //Mouth 3.0
}

void drawAppendage(GLfloat width, GLfloat length, GLfloat depth)
{
	glColor3f(robotBase.r, robotBase.g, robotBase.b);
	glBegin(GL_QUADS);
	//Front
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(width, 0.0, 0.0);
	glVertex3f(width, length, 0.0);
	glVertex3f(0.0, length, 0.0);
	//Back
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, depth);
	glVertex3f(width, 0.0, depth);
	glVertex3f(width, length, depth);
	glVertex3f(0.0, length, depth);
	//Right
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, depth);
	glVertex3f(0.0, length, depth);
	glVertex3f(0.0, length, 0.0);
	//Left
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f(width, 0.0, 0.0);
	glVertex3f(width, 0.0, depth);
	glVertex3f(width, length, depth);
	glVertex3f(width, length, 0.0);
	//Top
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, length, 0.0);
	glVertex3f(width, length, 0.0);
	glVertex3f(width, length, depth);
	glVertex3f(0.0, length, depth);
	//Bottom
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(width, 0.0, 0.0);
	glVertex3f(width, 0.0, depth);
	glVertex3f(0.0, 0.0, depth);
	glEnd();
}

void drawRobot()
{
	GLfloat torsoWidth = 8.0;
	GLfloat torsoHeight = 11.0;
	GLfloat torsoDepth = 5.0;
	GLfloat headWidth = 6.0;
	GLfloat headHeight = 5.0;
	GLfloat headDepth = 3.0;
	GLfloat armWidth = 3.0;
	GLfloat armHeight = 9.0;
	GLfloat armDepth = 3.0;
	GLfloat legWidth = 3.0;
	GLfloat legHeight = 11.0;
	GLfloat legDepth = 3.0;


	glPushMatrix();
	glTranslatef(0.0, legHeight, 0.0);
	drawTorso(); //torso is (8, 11, 5)

	glPushMatrix();
	glTranslatef(1.0, torsoHeight, 1.0);
	drawHead(); //head is (6, 5, 3)
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.0, 2.0, 1.0);
	drawAppendage(armWidth, armHeight, 3.0); //Right arm (3, 7, 3)
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-3.0, 2.0, 1.0);
	drawAppendage(armWidth, armHeight, armDepth); //Left arm (3, 7, 3)
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.0); //Left leg (3, 7, 3)
	drawAppendage(legWidth, legHeight, legDepth);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, 0.0, 1.0); //Right leg (3, 7, 3)
	drawAppendage(legWidth, legHeight, legDepth);
	glPopMatrix();
}

// Display
void display()
{
	GLfloat lpos[4] = { cameraX + 100.0, 100.0, -100.0, 1.0 };

	glPolygonMode(GL_FRONT, GL_FILL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(zoom*cos(theta) + 3, cam_hgt, zoom*sin(theta), 3, 20, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	

	glPushMatrix();
	//Move World
	glTranslatef(cam_pos, 0, cameraZ);
	glRotatef(cameraX, 0, 1, 0);

	drawFloor();

	//Center marker
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 0.0);
	glutSolidCylinder(0.1, 30.0, 15, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0.0, 5.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidCylinder(0.1, 30.0, 15, 30);
	glPopMatrix();

	//glTranslatef(4, 0, -3);
	//glRotatef(-90, 0, 1, 0);
	drawRobot();
	glPopMatrix();

	glFlush();
}

// Initialize
void initialize()
{
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	//glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, 1, 1000);
}

//Camera Movement
void special(int key, int x, int y)
{
	specials[key] = true;

	//cout << "Pressed: " << key << endl;

	glutPostRedisplay();
}

void myKeyboard(unsigned char c, int x, int y)
{
	if (c == 'p') {
		robotFeatures.r = 0.7;
	}
	else if (c == 'l') {
		robotFeatures.r = 0.3;
	}
	else if (c == ',') {
		getFlash = true;
		getPause = true;
		flashPauseCount = 0;
	}

	keys[c] = true;

	//cout << "Pressed: " << c << endl;

	glutPostRedisplay();
}

void myUpKeyboard(unsigned char c, int x, int y)
{
	keys[c] = false;
	//cout << "Released: " << c << endl;
	//glutPostRedisplay();
}

void myUpSpecial(int key, int x, int y)
{
	specials[key] = false;
	//cout << "Released: " << key << endl;
	//glutPostRedisplay();
}

void myTimer(int value)
{
	theta += 0.01;
	glutPostRedisplay();
	glutTimerFunc(25, myTimer, 0);
}


void flashTimer(int value)
{
	int pause = 7; 
	if (getFlash == true && robotFeatures.r <= 2.0) {
		robotFeatures.r += 0.15;
		if (robotFeatures.r >= 0.4) {
			getFlash = false;
			getPause = true;
		}
	}
	else {
		if (flashPauseCount <= pause && getPause == true) {
			flashPauseCount++;
		}
		else  if (robotFeatures.r > robotFeatures.baser) {
			robotFeatures.r -= 0.1;
		}
		else if (flashPauseCount >= pause) {
			flashPauseCount = 0;
			getPause = false;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(25, flashTimer, 0);
}


void movementTimer(int value)
{

	if (specials[GLUT_KEY_UP]) {
		cam_hgt++;
	}
	else if (specials[GLUT_KEY_DOWN]) {
		cam_hgt--;
	}
	else if (specials[GLUT_KEY_LEFT]) {
		cam_pos += 0.5;//theta -= 0.1;
	}
	else if (specials[GLUT_KEY_RIGHT]) {
		cam_pos -= 0.5;//theta += 0.1;
	}



	if (keys['w']) {
		cameraZ += 0.5;;
	}
	else if (keys['w'] && keys['a'])
	{
		cameraZ += 0.5;
		cameraX += 1;
	}
	else if (keys['w'] && keys['d'])
	{
		cameraZ += 0.5;
		cameraX -= 1;
	}

	if (keys['s'])
	{
		cameraZ -= 0.5;
	}
	else if (keys['s'] && keys['a'])
	{
		cameraZ -= 0.5;
		cameraX += 1;
	}
	else if (keys['s'] && keys['d'])
	{
		cameraZ -= 0.5;
		cameraX -= 1;
	}
	if (keys['d'])
	{
		cameraX -= 1;
	}
	if (keys['a'])
	{
		cameraX += 1;
	}
	glutPostRedisplay();
	glutTimerFunc(25, movementTimer, 0);
}

void mouse(int button, int state, int x, int y)
{
	if (button == 3 && zoom > 5) {
		zoom -= zoomSpeed;
	}
	else if (button == 4 && zoom < 200) {
		zoom += zoomSpeed;
	}
}

// Main
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(1000, 10);
	glutCreateWindow("Robot");
	initialize();

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutSpecialUpFunc(myUpSpecial);
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myUpKeyboard);
	glutMouseFunc(mouse);
	//glutTimerFunc(50, myTimer, 0);
	glutTimerFunc(25, flashTimer, 0);
	glutTimerFunc(25, movementTimer, 0);
	glutMainLoop();
	return 0;
}