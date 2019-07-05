// === PushMan.cpp - Main functions of 'Push Man' project ===
//
// Date: 2019. 4. 29. ~ 2019. 6. 8.
// Writer: Changjo Yun (윤창조)
// Computer Graphics Term Project on Korea University
//

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
#include <cmath>

#include "VECTOR.h"
#include "MATRIX.h"
#include "Person.h"
#include "Train.h"

#define PI 3.14159265358979323846
#define CROWD_NUM 264
#define PLATFORM_WIDTH 50.0f
#define PLATFORM_HEIGHT 5.0f
#define PLATFORM_BREATH 5.0f
using namespace std;

// =====----- Prototypes -----=====
void initialize(int, char **);
void initTexture(const char *, GLuint &);
void display();
void timerFunc(int value);
void reshape(int, int);
void drawXZPlane(GLfloat x, GLfloat y, GLfloat z);
void drawYZPlane(GLfloat x, GLfloat y, GLfloat z);
void drawXYPlane(GLfloat x, GLfloat y, GLfloat z);
void lighting();
void keyCallback(unsigned char key, int x, int y);
bool collisionTest(Person &a, Person &b);
bool collisionTest(Train &t, Person &p);

// =====----- Global Variables -----=====
Train myTrain;
Person I_am;
Person crowd[500];
VECTOR3D camera, cameraDir(0.0f, 0.0f, -1.0f);
int timeInterval=30;  // 30 ms
GLuint textureObj[4];

// =====----- Main Function -----=====
int main(int argc, char **argv)
{
  initialize(argc, argv);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyCallback);
  glutTimerFunc(timeInterval, timerFunc, 0);
  glutMainLoop();
  return 0;
}

// =====----- Initialize OpenGL Module -----=====
void initialize(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowPosition(400, 100);
	glutInitWindowSize(1024, 576);
	glutCreateWindow("Push Man");
  glEnable(GL_DEPTH_TEST);

  // GLEW initialize
  GLenum errorCode = glewInit();
	if (GLEW_OK != errorCode) {
		cout << "Error: GLEW Initialization is failed\n" << glewGetErrorString(errorCode) << endl;
		exit(EXIT_FAILURE);
	}

  // initialize and load textures
  glGenTextures(4, textureObj);
  initTexture("gangnam_stn_texture.jpg", textureObj[0]);
  initTexture("ground_texture.jpg", textureObj[1]);
  initTexture("tile_texture.jpg", textureObj[2]);
  initTexture("2nd_line_train.jpg", textureObj[3]);

  // initialize position of I and camera
  I_am.setPosition(VECTOR3D(0.0f, 0.0f, -PLATFORM_BREATH/2.f));
  camera = I_am.getPosition() + 0.25f*cameraDir;

  // initialize position and velocity of train (start point: left of platform)
  myTrain.setPosition(VECTOR3D(PLATFORM_WIDTH/2.f+2.75f, 0.0f, -PLATFORM_BREATH-5.0f));
  myTrain.setVelocity(VECTOR3D(0.15f, 0.0f, 0.0f));

  // initialize positions of people (crowd) in the train
  for (int i = 0; i < 33; i++) {
    for (int j = 0; j < 4; j++) {
      VECTOR3D relativePos(0.3+0.6*i, 0, 0.3+0.6*j);
      crowd[4*i+j].setPosition(myTrain.getPosition() + relativePos);
    }
  }
  for (int i = 33; i < 66; i++) {
    for (int j = 0; j < 4; j++) {
      VECTOR3D relativePos(1.0+0.6*i, 0, 0.3+0.6*j);
      crowd[4*i+j].setPosition(myTrain.getPosition() + relativePos);
    }
  }

  // OpenGL light setting
  lighting();
}

// =====----- Initialize OpenGL Texture -----=====
void initTexture(const char *filename, GLuint &binding) {
  glBindTexture(GL_TEXTURE_2D, binding);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
}

// =====----- Display Module -----=====
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
  //setting camera
  camera = I_am.getPosition() + 0.25f*cameraDir + VECTOR3D(0, I_am.getHeight()*0.8, 0);
  VECTOR3D view = camera + 0.1f*cameraDir;
  gluLookAt(camera.x, camera.y, camera.z, view.x, view.y, view.z, 0.0f, 1.0f, 0.0f);

  //draw train
  myTrain.draw(GL_TEXTURE3, textureObj[3]);

  //draw platform
  glTranslatef(-PLATFORM_WIDTH/2.f, 0, -PLATFORM_BREATH-2.15f);
  drawXZPlane(PLATFORM_WIDTH, 0.0f, PLATFORM_BREATH);
  drawYZPlane(0.0f, PLATFORM_HEIGHT, PLATFORM_BREATH);
  drawYZPlane(PLATFORM_WIDTH, PLATFORM_HEIGHT, PLATFORM_BREATH);
  drawXYPlane(PLATFORM_WIDTH, PLATFORM_HEIGHT, PLATFORM_BREATH);
  glTranslatef(PLATFORM_WIDTH/2.f, 0, PLATFORM_BREATH+2.15f);

  //draw people
  I_am.draw();
  for (int i = 0; i < CROWD_NUM; i++) {
    crowd[i].draw();
  }

  glutSwapBuffers();
}

// =====----- Timer Function for Animation -----=====
void timerFunc(int value)
{
  const int period = 1350;
  const float brake = -0.2;
  const float maxSpeed = 1.5;
  float speed = myTrain.getVelocity().x;

  // Train time schedule (periodic)
  if (value == 0) {
    // reset point
    if (I_am.isInsideTrain(myTrain)) {
      I_am.setPosition(I_am.getPosition() - VECTOR3D(2*PLATFORM_WIDTH+2.75f, 0, 0));
    }

    for (int i = 0; i < CROWD_NUM; i++) {
      if (crowd[i].isInsideTrain(myTrain))
        crowd[i].setPosition(crowd[i].getPosition() - VECTOR3D(2*PLATFORM_WIDTH+2.75f, 0, 0));
    }

    myTrain.setPosition(myTrain.getPosition() - VECTOR3D(2*PLATFORM_WIDTH+2.75f, 0, 0));
  } else if (0 < value && value < period/4) {
    // 1/4 period
    if (speed <= maxSpeed) {
      myTrain.setAcceleration(VECTOR3D(-brake, 0, 0));
    } else {
      myTrain.setAcceleration(VECTOR3D(0, 0, 0));
    }
  } else if (period/4 <= value && value < 2*period/4) {
    // 2/4 period
    if (speed > 0) {
      myTrain.setAcceleration(VECTOR3D(brake, 0, 0));
    } else {
      myTrain.setAcceleration(VECTOR3D(0, 0, 0));
      myTrain.setVelocity(VECTOR3D(0, 0, 0));
    }
    // door open
    if (value == period*5/16) myTrain.doorOpen();
  } else if (2*period/4 <= value && value < 3*period/4) {
    // 3/4 period
    if (speed > 0) {
      myTrain.setAcceleration(VECTOR3D(brake, 0, 0));
    } else {
      myTrain.setAcceleration(VECTOR3D(0, 0, 0));
      myTrain.setVelocity(VECTOR3D(0, 0, 0));
    }
    // door close
    if (value == period*11/16) myTrain.doorClose();
  } else if (3*period/4 <= value && value < period) {
    // 4/4 period
    if (speed <= maxSpeed) {
      myTrain.setAcceleration(VECTOR3D(-brake, 0, 0));
    } else {
      myTrain.setAcceleration(VECTOR3D(0, 0, 0));
    }
  } else {
    // back to the start point
    value = -1;
  }

  if (I_am.isInsideTrain(myTrain)) {
    // inside train: reduce the relative velocity of I
    I_am.setVelocity(myTrain.getVelocity() + (I_am.getVelocity()-myTrain.getVelocity()) * 0.7);
    I_am.setAcceleration(myTrain.getAcceleration());
  } else {
    // outside train: reduce the velocity of I
    I_am.setVelocity(I_am.getVelocity() * 0.7);
    I_am.setAcceleration(VECTOR3D(0, 0, 0));
  }

  for (int i = 0; i < CROWD_NUM; i++) {
    if (crowd[i].isInsideTrain(myTrain)) {
      // inside train: reduce the relative velocity of crowd
      crowd[i].setVelocity(myTrain.getVelocity() + (crowd[i].getVelocity()-myTrain.getVelocity()) * 0.7);
      crowd[i].setAcceleration(myTrain.getAcceleration());
    } else {
      // outside train: reduce the velocity of crowd
      crowd[i].setVelocity(crowd[i].getVelocity() * 0.7);
      crowd[i].setAcceleration(VECTOR3D(0, 0, 0));
    }
  }

  // Total collision detection and reaction
  collisionTest(myTrain, I_am);
  for (int i = 0; i < CROWD_NUM; i++) {
    collisionTest(I_am, crowd[i]);
    collisionTest(myTrain, crowd[i]);
    for (int j = i+1; j < CROWD_NUM; j++) {
      collisionTest(crowd[i], crowd[j]);
    }
  }

  // apply one time motion of everything
  myTrain.motionOneTime();
  I_am.motionOneTime();
  for (int i = 0; i < CROWD_NUM; i++) {
    crowd[i].motionOneTime();
  }

  glutPostRedisplay();
  glutTimerFunc(timeInterval, timerFunc, value+1);
}

// =====----- Reshape Module -----=====
void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.1, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// =====----- Plane Model (x, y, z) -----=====
void drawXZPlane(GLfloat x, GLfloat y, GLfloat z) {
  // using texture 1
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureObj[1]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);

    glMultiTexCoord2f(GL_TEXTURE1, (x/z)*10.0f, 10.0f);
    glVertex3f(0.0f, y, 0.0f);
    glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 10.0f);
    glVertex3f(x, y, 0.0f);
    glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
    glVertex3f(x, y, z);
    glMultiTexCoord2f(GL_TEXTURE1, (x/z)*10.0f, 0.0f);
    glVertex3f(0.0f, y, z);
	glEnd();

  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
}

// =====----- Plane Model (x, y, z) -----=====
void drawYZPlane(GLfloat x, GLfloat y, GLfloat z) {
  // using texture 2
  glActiveTexture(GL_TEXTURE2);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureObj[2]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);

    glMultiTexCoord2f(GL_TEXTURE2, 10.0f, 10.0f);
    glVertex3f(x, 0.0f, 0.0f);
    glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 10.0f);
    glVertex3f(x, y, 0.0f);
    glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
    glVertex3f(x, y, z);
    glMultiTexCoord2f(GL_TEXTURE2, 10.0f, 0.0f);
    glVertex3f(x, 0.0f, z);
	glEnd();

  glActiveTexture(GL_TEXTURE2);
  glDisable(GL_TEXTURE_2D);
}

// =====----- Plane Model (x, y, z) -----=====
void drawXYPlane(GLfloat x, GLfloat y, GLfloat z) {
  // using texture 0
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureObj[0]);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);

    glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, z);
    glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
    glVertex3f(x, 0.0f, z);
    glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
    glVertex3f(x, y, z);
    glMultiTexCoord2f(GL_TEXTURE0, 2.0f, 0.0f);
    glVertex3f(0.0f, y, z);
	glEnd();

  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
}

// =====----- Light Shading -----=====
void lighting(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	VECTOR3D color = VECTOR3D(0.8, 0.8, 0.8);
	float light_pos[] = { 25.0, 50.0, 50.0, 0.0 };
	float light_dir[] = { 0, -1, -1 };
	float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float frontColor[] = { color.x, color.y, color.z };
	float matShininess = 100;
	float noMat[] = { 0, 0, 0, 1 };
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

// =====----- Key Callback Function -----=====
void keyCallback(unsigned char key, int x, int y) {
  float angle = 0.174533f;  // radian value of 10 degree
  VECTOR3D LeftRight(-cameraDir.z, 0.0f, cameraDir.x); // right side of camera
  MATRIX rotate;  // rotation matrix
  rotate.ele[0][0] = cos(angle);
  rotate.ele[0][2] = -sin(angle);
  rotate.ele[1][1] = 1.0f;
  rotate.ele[2][0] = sin(angle);
  rotate.ele[2][2] = cos(angle);

	switch (key) {
	case 'w':  // move front
		I_am.setPosition(I_am.getPosition() + 0.1f*cameraDir);
		break;
	case 's':  // move back
		I_am.setPosition(I_am.getPosition() - 0.1f*cameraDir);
		break;
	case 'a':  // move left
		I_am.setPosition(I_am.getPosition() - 0.1f*LeftRight);
		break;
	case 'd':  // move right
		I_am.setPosition(I_am.getPosition() + 0.1f*LeftRight);
		break;
  case 'q':  // rotate counter clockwise
    cameraDir = rotate.Transpose() * cameraDir;
    break;
  case 'e':  // rotate clockwise
    cameraDir = rotate * cameraDir;
    break;
	}

	glutPostRedisplay();
}

// =====----- Collision Test Function between two people -----=====
bool collisionTest(Person &a, Person &b)
{
  // collision response: elastic force
  VECTOR3D between_vec = b.getPosition() - a.getPosition();
  float distance = between_vec.Magnitude();
  float x = a.getBodySize()/2.f + b.getBodySize()/2.f - distance;
  const float k = 1000.f;

  between_vec.Normalize();  // |between_vec| = 1

  if (x > 0) {
    a.setAcceleration(a.getAcceleration() - k * x * between_vec / a.getMass());
    b.setAcceleration(b.getAcceleration() + k * x * between_vec / b.getMass());
  }

  return x > 0;
}

// =====----- Collision Test Function between train and person -----=====
bool collisionTest(Train &t, Person &p)
{
  // collision response: move inside or outside of train definitely
  float limit = p.getBodySize()/2.f;
  float width = 1.02f*t.getCarNum()*t.getWidth();
  VECTOR3D relativePos = p.getPosition() - t.getPosition();
  VECTOR3D x(0, 0, 0);
  bool doorCollTest = false;

  if (0 < relativePos.x && relativePos.x < width) {
    if (0 < relativePos.z && relativePos.z < limit) {
      x.z = limit - relativePos.z;
      p.setPosition(p.getPosition() + x);
      return true;
    }
    if (-limit < relativePos.z && relativePos.z <= 0) {
      x.z = limit + relativePos.z;
      p.setPosition(p.getPosition() - x);
      return true;
    }
  }
  if (0 < relativePos.z && relativePos.z < t.getBreath()) {
    if (0 < relativePos.x && relativePos.x < limit) {
      x.x = limit - relativePos.x;
      p.setPosition(p.getPosition() + x);
      return true;
    }
    if (-limit < relativePos.x && relativePos.x <= 0) {
      x.x = limit + relativePos.x;
      p.setPosition(p.getPosition() - x);
      return true;
    }
  }
  if (0 < relativePos.z && relativePos.z < t.getBreath()) {
    if (0 < width - relativePos.x && width - relativePos.x < limit) {
      x.x = limit - (width - relativePos.x);
      p.setPosition(p.getPosition() - x);
      return true;
    }
    if (-limit < width - relativePos.x && width - relativePos.x <= 0) {
      x.x = limit + (width - relativePos.x);
      p.setPosition(p.getPosition() + x);
      return true;
    }
  }

  if (t.isDoorOpen()) {
    for (int i = 0; i < t.getCarNum(); i++) {
      float temp = 1.02f * i * t.getWidth();
      doorCollTest = doorCollTest ||
        ((temp < relativePos.x && relativePos.x < temp + (1.f/7.f)*t.getWidth()) ||
        (temp + (5.f/14.f)*t.getWidth() < relativePos.x && relativePos.x < temp + (9.f/14.f)*t.getWidth()) ||
        (temp + (6.f/7.f)*t.getWidth() < relativePos.x && relativePos.x < temp + t.getWidth()));
    }
  } else {
    doorCollTest = 0 < relativePos.x && relativePos.x < width;
  }

  if (doorCollTest) {
    if (0 < t.getBreath() - relativePos.z && t.getBreath() - relativePos.z < limit) {
      x.z = limit - (t.getBreath() - relativePos.z);
      p.setPosition(p.getPosition() - x);
      return true;
    }
    if (-limit < t.getBreath() - relativePos.z && t.getBreath() - relativePos.z <= 0) {
      x.z = limit + (t.getBreath() - relativePos.z);
      p.setPosition(p.getPosition() + x);
      return true;
    }
  }

  return false;
}
