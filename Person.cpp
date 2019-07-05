#include "Person.h"

Person::Person() {
  height = 1.7f;
  body_size = 0.6f;
  quadratic = NULL;
  mass = 60.0f;
  dt = 0.1f;
}

Person::Person(float height, float body_size, VECTOR3D position, float mass) {
  this->height = height;
  this->body_size = body_size;
  this->position = position;
  this->mass = mass;
  dt = 0.1f;
  quadratic = NULL;
}

void Person::draw() {
  if (quadratic == NULL)
    quadratic = gluNewQuadric();

  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glPushMatrix();
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  gluCylinder(quadratic, body_size/2.f, body_size/2.f*0.67f, height*0.67f, 30, 30);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.0f, height*0.67f+0.25f, 0.0f);
  gluSphere(quadratic, 0.25f, 30, 30);
  glPopMatrix();
  glPopMatrix();
}

void Person::motionOneTime() {
  velocity += accel * dt;
  position += velocity * dt;
}

bool Person::isInsideTrain(Train &train) {
  float limit = body_size/2.f;
  VECTOR3D relativePos = position - train.getPosition();
  return relativePos.z >= 0 && relativePos.x >= 0 &&
    1.02f*train.getCarNum()*train.getWidth() - relativePos.x >= 0 &&
    train.getBreath() - relativePos.z >= 0;
}
