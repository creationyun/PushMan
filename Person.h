#pragma once

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "VECTOR.h"
#include "Train.h"

class Person {
private:
  float height, body_size;
  VECTOR3D position, velocity, accel;
  GLUquadricObj *quadratic;
  float mass, dt;
public:
  Person();
  Person(float height, float body_size, VECTOR3D position, float mass);
  float getHeight() { return height; }
  float getBodySize() { return body_size; }
  float getMass() { return mass; }
  float getDt() { return dt; }
  VECTOR3D getPosition() { return position; }
  VECTOR3D getVelocity() { return velocity; }
  VECTOR3D getAcceleration() { return accel; }
  void setSize(float height, float body_size) {
    this->height = height; this->body_size = body_size;
  }
  void setMass(float mass) { this->mass = mass; }
  void setDt(float dt) { this->dt = dt; }
  void setPosition(VECTOR3D position) { this->position = position; }
  void setVelocity(VECTOR3D velocity) { this->velocity = velocity; }
  void setAcceleration(VECTOR3D accel) { this->accel = accel; }
  void draw();
  void motionOneTime();
  bool isInsideTrain(Train &train);
};
