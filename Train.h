#pragma once

#include <iostream>
#include <GL/gl.h>
#include "VECTOR.h"

class Train {
private:
  float width;
  float height;
  float breath;
  unsigned int car_num;
  VECTOR3D position, velocity, accel;
  float dt;
  bool door_open;
  float door_close_rate;
  GLuint textureObj;
public:
  Train();
  Train(float width, float height, float breath, unsigned int car_num);
  float getWidth() { return width; }
  float getHeight() { return height; }
  float getBreath() { return breath; }
  float getDt() { return dt; }
  VECTOR3D getPosition() { return position; }
  VECTOR3D getVelocity() { return velocity; }
  VECTOR3D getAcceleration() { return accel; }
  bool isDoorOpen() { return door_open; }
  unsigned int getCarNum() { return car_num; }
  void setSize(float width, float height, float breath) {
    this->width = width; this->height = height; this->breath = breath;
  }
  void setPosition(VECTOR3D position) { this->position = position; }
  void setVelocity(VECTOR3D velocity) { this->velocity = velocity; }
  void setAcceleration(VECTOR3D accel) { this->accel = accel; }
  void setCarNum(unsigned int car_num) { this->car_num = car_num; }
  void setDt(float dt) { this->dt = dt; }
  void doorOpen() { door_open = true; }
  void doorClose() { door_open = false; }
  void draw(GLenum tex, GLuint &texObj);
  void motionOneTime();
};
