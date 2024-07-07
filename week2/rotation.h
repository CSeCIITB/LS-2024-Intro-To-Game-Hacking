#ifndef ROTATION_H
#define ROTATION_H

struct Rotation {
    float pitch;
    float yaw;
    float roll;
  public:
    Rotation();
    Rotation(float, float, float);
};

#endif