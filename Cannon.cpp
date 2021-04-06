//  ========================================================================
//  COSC363: Computer Graphics (2018);  University of Canterbury.
//
//  FILE NAME: Cannon.cpp
//  See Lab02.pdf for details
//
//  Program displays the model of a cannon (mesh data from Cannon.off)
//  Use left/right arrow keys to rotate the scene
//  Use up/down arrow keys to move camera up/down
//  ========================================================================

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h> 
#include <sstream>
#include <GL/freeglut.h>
#include "loadBMP.h"
using namespace std;

//--Globals ---------------------------------------------------------------
float* x, * y, * z;  //vertex coordinate arrays
int* t1, * t2, * t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles
float cam_hgt = 400;
float cannon_x = 0;
float cannon_y = 0;
bool cannon_done = true;
float angle = -5;
float eye_x = -120;
float eye_z = 150;
float look_x = 100 * sin(angle) + eye_x;
float look_z = -100 * cos(angle) + eye_z;
float theta = -70;

const int N = 10;  // Total number of vertices on the base curve

float vx_init[N] = { 0, 0, 20, 20, 20, 20, 20, 20, 20, 20 };
float vy_init[N] = { 0, 0, 10, 20, 30, 40, 50, 60, 70, 80 };
float vz_init[N] = { 0 };

GLuint txId[10];

void loadTexture()
{
	glGenTextures(10, txId); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, txId[0]);		//Use this texture
	loadBMP("WoodTexture.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	
	glBindTexture(GL_TEXTURE_2D, txId[1]);		//Use this texture
	loadBMP("back.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[2]);		//Use this texture
	loadBMP("bottom.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[3]);		//Use this texture
	loadBMP("front.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[4]);		//Use this texture
	loadBMP("right.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[5]);		//Use this texture
	loadBMP("left.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[6]);		//Use this texture
	loadBMP("top.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, txId[7]);		//Use this texture
	loadBMP("CannonBall.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void drawSkybox() {
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glBegin(GL_QUADS);
	// BACK
	glTexCoord2f(0., 1.);  glVertex3f(-800, 500, 840);
	glTexCoord2f(0., 0.);  glVertex3f(-800, -0.005, 840);
	glTexCoord2f(1., 0.);  glVertex3f(-800, -0.005, -800);
	glTexCoord2f(1., 1.);  glVertex3f(-800, 500, -800);
	glEnd();

	//glBindTexture(GL_TEXTURE_2D, txId[2]);
	//glBegin(GL_QUADS);
	/// BOTTOM
	//glTexCoord2f(0., 1.); glVertex3f(840, 0.1, -800);
	//glTexCoord2f(0., 0.); glVertex3f(-800, 0.1, -800);
	//glTexCoord2f(1., 0.);  glVertex3f(-800, 0.1, 840);
	//glTexCoord2f(1., 1.);  glVertex3f(840, 0.1, 840);
	//glEnd();

	glBindTexture(GL_TEXTURE_2D, txId[3]);
	glBegin(GL_QUADS);
	// FRONT
	glTexCoord2f(0., 1.);  glVertex3f(840, 500, -800);
	glTexCoord2f(0., 0.);  glVertex3f(840, -0.005, -800);
	glTexCoord2f(1., 0.);  glVertex3f(840, -0.005, 840);
	glTexCoord2f(1., 1.);  glVertex3f(840, 500, 840);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, txId[4]);
	glBegin(GL_QUADS);
	// LEFT
	glTexCoord2f(0., 1.);  glVertex3f(-800, 500, -800);
	glTexCoord2f(0., 0.);  glVertex3f(-800, -0.005, -800);
	glTexCoord2f(1., 0.); glVertex3f(840, -0.005, -800);
	glTexCoord2f(1., 1.); glVertex3f(840, 500, -800);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, txId[5]);
	glBegin(GL_QUADS);
	// RIGHT 
	glTexCoord2f(0., 1.);  glVertex3f(840, 500, 840);
	glTexCoord2f(0., 0.);  glVertex3f(840, -0.005, 840);
	glTexCoord2f(1., 0.); glVertex3f(-800, -0.005, 840);
	glTexCoord2f(1., 1.); glVertex3f(-800, 500, 840);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, txId[6]);
	glBegin(GL_QUADS);
	// TOP
	glTexCoord2f(0., 1.); glVertex3f(840, 500, -800);
	glTexCoord2f(0., 0.); glVertex3f(-800, 500, -800);
	glTexCoord2f(1., 0.);  glVertex3f(-800, 500, 840);
	glTexCoord2f(1., 1.);  glVertex3f(840, 500, 840);
	glEnd();


	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}



//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if (!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges

	x = new float[nvrt];                        //create arrays
	y = new float[nvrt];
	z = new float[nvrt];

	t1 = new int[ntri];
	t2 = new int[ntri];
	t3 = new int[ntri];

	for (int i = 0; i < nvrt; i++)                         //read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];

	for (int i = 0; i < ntri; i++)                         //read polygon list 
	{
		fp_in >> num >> t1[i] >> t2[i] >> t3[i];
		if (num != 3)
		{
			cout << "ERROR: Polygon with index " << i << " is not a triangle." << endl;  //not a triangle!!
			exit(1);
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal1(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	glNormal3f(nx, ny, nz);
}

void normal2(float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3)
{
	float nx, ny, nz;
	nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

	glNormal3f(nx, ny, nz);
}

//--------draws the mesh model of the cannon----------------------------
void drawCannon()
{
	glColor3f(0.4, 0.5, 0.4);

	//Construct the object model here using triangles read from OFF file
	glBegin(GL_TRIANGLES);
	for (int tindx = 0; tindx < ntri; tindx++)
	{
		normal1(tindx);
		glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
		glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
		glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
	}
	glEnd();
}

//----------draw a floor plane-------------------
void drawFloor()
{
	bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for (int x = -800; x <= 800; x += 40)
	{
		for (int z = -800; z <= 800; z += 40)
		{
			if (flag) glColor3f(0.6, 1.0, 0.8);
			else glColor3f(0.8, 1.0, 0.6);
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z + 40);
			glVertex3f(x + 40, 0, z + 40);
			glVertex3f(x + 40, 0, z);
			flag = !flag;
		}
	}
	glEnd();
}

void drawPendulum() 
{
	glColor3f(0, 0, 1);
	glPushMatrix();
	glTranslatef(260, 0, 110);
	glScalef(10, 500, 2);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(260, 0, -110);
	glScalef(10, 500, 2);
	glutSolidCube(1);
	glPopMatrix();

	//horizontal bar
	glPushMatrix();
	glTranslatef(260, 250, 0);
	glScalef(10, 2, 220);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(130, 250, 0);
	glRotatef(theta, 1, 0, 0);
	glTranslatef(-130, -250, 0);
	glPushMatrix();
	glTranslatef(260, 225, 0);
	glScalef(10, 50, 2);
	glutSolidCube(1);
	glPopMatrix();

	//torus
	glPushMatrix();
	glTranslatef(260, 175, 0);
	glRotatef(90, 0, 1, 0);
	glutSolidTorus(6, 20, 20, 20);
	glPopMatrix();
	glPopMatrix();

}


void drawSpheres() 
{
	GLUquadricObj *sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[7]);
	gluSphere(sphere, 5, 36, 18);
	glDisable(GL_TEXTURE_2D);

}

//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()
{
	float lpos[4] = { 100., 100., 100., 1.0 };  //light's position
	float shadowMat[16] = { lpos[1],0,0,0, -lpos[0],0,-lpos[2],-1, 0,0,lpos[1],0, 0,0,0,lpos[1] };


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position
	look_x = 100 * sin(angle) + eye_x;
	look_z = -100 * cos(angle) + eye_z;
	gluLookAt(eye_x, 450, eye_z, look_x, 350, look_z, 0, 1, 0);

	glPushMatrix();
	glTranslatef(0, -0.005, 0);
	drawFloor();
	glPopMatrix();

	//--cannon
	glPushMatrix();
	glTranslatef(-20, 30, 0);
	glRotatef(30, 0, 0, 1);
	glTranslatef(20, -30, 0);
	drawCannon();
	glPopMatrix();

	//--start mount
	glPushMatrix();
	glColor3f(1, 0.7, 0.1);
	glTranslatef(-10, 5, 17);
	glScalef(80, 10, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.7, 0.1);
	glTranslatef(-20, 25, 17);
	glScalef(40, 30, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.7, 0.1);
	glTranslatef(-10, 5, -17);
	glScalef(80, 10, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.7, 0.1);
	glTranslatef(-20, 25, -17);
	glScalef(40, 30, 6);
	glutSolidCube(1);
	glPopMatrix();
	//--end mount

	//--cannon ball
	glPushMatrix();
	glTranslatef(38.88 + cannon_x, 64 + cannon_y, 0);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 80, 50);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 80, 60);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 80, 55);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 80, 65);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 80, 45);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 80, 50);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 80, 60);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 80, 55);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 80, 65);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 80, 45);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 80, 45);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10, 80, 50);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 80, 45);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 80, 50);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 80, 55);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5, 80, 60);
	drawSpheres();
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glPushMatrix(); //Draw Shadow Object
	glMultMatrixf(shadowMat);
	glTranslatef(-80, 0, 40);
	glScalef(5, 5, 5);
	glColor4f(0.2, 0.2, 0.2, 1.0);
	glutSolidSphere(5, 36, 18);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(-50, 15, 50);
	glScalef(5, 5, 5);
	drawSpheres();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 65, 50);
	glutSolidSphere(20, 10, 20);
	glPopMatrix();



	drawPendulum();
	
	glPushMatrix();
	glColor3f(0.5, 0.35, 0.05);
	glTranslatef(0, 0, 50);
	float vx[N], vy[N], vz[N];
	float wx[N], wy[N], wz[N];
	float lgt_pos[] = { 5.0f, 50.0f, 100.0f, 1.0f };  //light0 position 

	for (int i = 0; i < N; i++)		//Initialize data everytime
	{
		vx[i] = vx_init[i];
		vy[i] = vy_init[i];
		vz[i] = vz_init[i];
	}

	for (int j = 0; j < 36; j++) //36 slices in 10 deg steps
	{
		for (int i = 0; i < N; i++) //N vertices along each slice
		{
			wx[i] = vx[i] * cos(0.174533) + vz[i] * sin(0.174533);
			wy[i] = vy[i];
			wz[i] = -vx[i] * sin(0.174533) + vz[i] * cos(0.174533);
		}
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < N; i++) {
			if (i > 0) normal2(vx[i - 1], vy[i - 1], vz[i - 1],
				wx[i - 1], wy[i - 1], wz[i - 1],
				vx[i], vy[i], vz[i]);
			glTexCoord2f((float)j / N, (float)i / (N - 1));
			glVertex3f(vx[i], vy[i], vz[i]);
			if (i > 0) normal2(wx[i - 1], wy[i - 1], wz[i - 1],
				wx[i], wy[i], wz[i],
				vx[i], vy[i], vz[i]);
			glTexCoord2f((float)(j + 1) / N, (float)i / (N - 1));
			glVertex3f(wx[i], wy[i], wz[i]);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		//Copy W array to V for next iteration
		for (int i = 0; i < N; i++)
		{
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}
	glPopMatrix();

	drawSkybox();

	//--cannon2
	glTranslatef(500, 0, 0);
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
	glTranslatef(-20, 30, 0);
	glRotatef(30, 0, 0, 1);
	glTranslatef(20, -30, 0);
	drawCannon();
	glPopMatrix();

	//--start mount
	glPushMatrix();
	glColor3f(1, 0.7, 0.1);
	glTranslatef(-10, 5, 17);
	glScalef(80, 10, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.7, 0.1);
	glTranslatef(-20, 25, 17);
	glScalef(40, 30, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.7, 0.1);
	glTranslatef(-10, 5, -17);
	glScalef(80, 10, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0.7, 0.1);
	glTranslatef(-20, 25, -17);
	glScalef(40, 30, 6);
	glutSolidCube(1);
	glPopMatrix();


	glutSwapBuffers();
}


//------ Cannon ball timer to move it ------------------------------------
void cannonShotTimer(int value)
{
	if (cannon_y + 1 > 0 && cannon_done) {
		if (cannon_x > 417) {
			cannon_done = false;
		}
		cannon_x++;
		theta += 0.33;
		cannon_y = (-pow(cannon_x, 2)) / 420 + cannon_x;  //cannon parabola function (range increase the divider of x^2)
		glutPostRedisplay();
		glutTimerFunc(5, cannonShotTimer, 0);
	}
	else {
		if (cannon_x < 3) {
			cannon_done = true;
		}
		cannon_x--;
		theta -= 0.33;
		cannon_y = (-pow(cannon_x, 2)) / 420 + cannon_x;  //cannon parabola function (range increase the divider of x^2)
		glutPostRedisplay();
		glutTimerFunc(5, cannonShotTimer, 0);
	}
}


//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	loadMeshFile("Cannon.off");				//Specify mesh file name here
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	loadTexture();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 100, 2000);  //The camera view volume 
	
}

bool detect_collision() {

	bool collision = false;
	if (eye_x > 720 || eye_x < -680 || eye_z > 720 || eye_z < -680) {
		collision = true;
	}
	return collision;
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
	
	float temp_eye_x = eye_x;
	float temp_eye_z = eye_z;
	
	if (key == GLUT_KEY_LEFT) angle -= 0.1;
	else if (key == GLUT_KEY_RIGHT) angle += 0.1;
	else if (key == GLUT_KEY_DOWN) {
		eye_x -= 10 * sin(angle); eye_z += 10 * cos(angle);
		}
	else if (key == GLUT_KEY_UP) {
		eye_x += 10 * sin(angle); eye_z -= 10 * cos(angle);
	}

	if (detect_collision()) {
		eye_x = temp_eye_x;
		eye_z = temp_eye_z;
	}

	glutPostRedisplay();
}

//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Cannon");
	initialize();

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutTimerFunc(5, cannonShotTimer, 0);
	glutMainLoop();
	return 0;
}