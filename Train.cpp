#include "Train.h"

Train::Train() {
  // default settings (light transit size)
  width = 20.0f;
  height = 3.4f;
  breath = 2.65f;
  car_num = 2;
  dt = 0.1f;
  door_open = false;
  door_close_rate = 0.0f;
}

Train::Train(float width, float height, float breath, unsigned int car_num) {
  this->width = width;
  this->height = height;
  this->breath = breath;
  dt = 0.1f;
  this->car_num = car_num;
  door_open = false;
  door_close_rate = 0.0f;
}

void Train::draw(GLenum tex, GLuint &texObj) {
  //setting cube vertices position
	GLfloat vertexList[3 * 26] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, breath,
	0.0f, height, 0.0f,
	0.0f, height, breath,
	width, height, 0.0f,
	width, height, breath,
	width, 0.0f, 0.0f,
	width, 0.0f, breath,
  (1.f/7.f)*width, 0.0f, breath,
  (1.f/7.f)*width, (2.f/3.f)*height, breath,
  (5.f/14.f)*width, (2.f/3.f)*height, breath,
  (5.f/14.f)*width, 0.0f, breath,
  (9.f/14.f)*width, 0.0f, breath,
  (9.f/14.f)*width, (2.f/3.f)*height, breath,
  (6.f/7.f)*width, (2.f/3.f)*height, breath,
  (6.f/7.f)*width, 0.0f, breath,
  0.0f, (2.f/3.f)*height, breath,
  width, (2.f/3.f)*height, breath,
  (1.f/7.f)*width+(door_close_rate/2.f)*(3.f/14.f)*width, 0.0f, breath,
  (1.f/7.f)*width+(door_close_rate/2.f)*(3.f/14.f)*width, (2.f/3.f)*height, breath,
  (5.f/14.f)*width-(door_close_rate/2.f)*(3.f/14.f)*width, (2.f/3.f)*height, breath,
  (5.f/14.f)*width-(door_close_rate/2.f)*(3.f/14.f)*width, 0.0f, breath,
  (9.f/14.f)*width+(door_close_rate/2.f)*(3.f/14.f)*width, 0.0f, breath,
  (9.f/14.f)*width+(door_close_rate/2.f)*(3.f/14.f)*width, (2.f/3.f)*height, breath,
  (6.f/7.f)*width-(door_close_rate/2.f)*(3.f/14.f)*width, (2.f/3.f)*height, breath,
  (6.f/7.f)*width-(door_close_rate/2.f)*(3.f/14.f)*width, 0.0f, breath,
	};

	//setting cube vertices indices
	unsigned int indexList[4 * 13] = {
	0, 1, 3, 2,
	2, 3, 5, 4,
	4, 5, 7, 6,
	6, 7, 1, 0,
	0, 2, 4, 6,
	16, 3, 5, 17,
  1, 16, 9, 8,
  11, 10, 13, 12,
  15, 14, 17, 7,
  8, 9, 19, 18,
  21, 20, 10, 11,
  12, 13, 23, 22,
  25, 24, 14, 15
	};

  // normal vectors in vertices index
  int normalIndexList[3 * 13] = {
  -1,  0,  0,
   0,  1,  0,
   1,  0,  0,
   0, -1,  0,
   0,  0, -1,
   0,  0,  1,
   0,  0,  1,
   0,  0,  1,
   0,  0,  1,
   0,  0,  1,
   0,  0,  1,
   0,  0,  1,
   0,  0,  1
  };

  GLfloat texCoordList[8 * 13] = {
    0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.2f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.2f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 1.0f, 0.2f, 1.0f, 0.2f, 0.0f,
    0.2f, 2.f/3.f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f, 2.f/3.f,
    0.2f, 0.0f, 0.2f, 2.f/3.f, 0.2f+0.8f*(1.f/7.f), 2.f/3.f, 0.2f+0.8f*(1.f/7.f), 0.0f,
    0.2f+0.8f*(5.f/14.f), 0.0f, 0.2f+0.8f*(5.f/14.f), 2.f/3.f, 0.2f+0.8f*(9.f/14.f), 2.f/3.f, 0.2f+0.8f*(9.f/14.f), 0.0f,
    0.2f+0.8f*(6.f/7.f), 0.0f, 0.2f+0.8f*(6.f/7.f), 2.f/3.f, 1.0f, 2.f/3.f, 1.0f, 0.0f,
    0.2f+0.8f*(1.f/7.f), 0.0f, 0.2f+0.8f*(1.f/7.f), 2.f/3.f, 0.2f+0.8f*((1.f/7.f)+(door_close_rate/2.f)*(3.f/14.f)), 2.f/3.f, 0.2f+0.8f*((1.f/7.f)+(door_close_rate/2.f)*(3.f/14.f)), 0.0f,
    0.2f+0.8f*((5.f/14.f)-(door_close_rate/2.f)*(3.f/14.f)), 0.0f, 0.2f+0.8f*((5.f/14.f)-(door_close_rate/2.f)*(3.f/14.f)), 2.f/3.f, 0.2f+0.8f*(5.f/14.f), 2.f/3.f, 0.2f+0.8f*(5.f/14.f), 0.0f,
    0.2f+0.8f*(9.f/14.f), 0.0f, 0.2f+0.8f*(9.f/14.f), 2.f/3.f, 0.2f+0.8f*((9.f/14.f)+(door_close_rate/2.f)*(3.f/14.f)), 2.f/3.f, 0.2f+0.8f*((9.f/14.f)+(door_close_rate/2.f)*(3.f/14.f)), 0.0f,
    0.2f+0.8f*((6.f/7.f)-(door_close_rate/2.f)*(3.f/14.f)), 0.0f, 0.2f+0.8f*((6.f/7.f)-(door_close_rate/2.f)*(3.f/14.f)), 2.f/3.f, 0.2f+0.8f*(6.f/7.f), 2.f/3.f, 0.2f+0.8f*(6.f/7.f), 0.0f,
  };

  glActiveTexture(tex);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texObj);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);

  for (int car = 0; car < car_num; car++) {
    //draw cube
  	glBegin(GL_QUADS);
  	for (int i = 0; i < 13; i++) {
      glNormal3f(normalIndexList[3*i], normalIndexList[3*i+1], normalIndexList[3*i+2]);
  		for (int j = 0; j < 4; j++) {
        glMultiTexCoord2f(tex, texCoordList[8*i + 2*j], 1.0f-texCoordList[8*i + 2*j + 1]);
  			glVertex3f(
  				vertexList[3 * indexList[4 * i + j]],
  				vertexList[3 * indexList[4 * i + j] + 1],
  				vertexList[3 * indexList[4 * i + j] + 2]
  			);
  		}
  	}
    glEnd();

    glTranslatef(width+width/50.0f, 0.0f, 0.0f);
  }
  // return to original position
  glPopMatrix();

  glActiveTexture(tex);
  glDisable(GL_TEXTURE_2D);
}

void Train::motionOneTime() {
  velocity += accel * dt;
  position += velocity * dt;
  if (door_open && door_close_rate > 0.0f) {
    door_close_rate -= 0.05f;
  }
  if (!door_open && door_close_rate < 1.0f) {
    door_close_rate += 0.05f;
  }
}
