#ifndef Mechanum
#define Mechanum

#include "util.h"
#include "vex.h"
#include <iostream>
#include <vex_task.h>

using namespace vex;

class MechanumDrivetrain {
private:
  motor fl;
  motor fr;
  motor bl;
  motor br;

  float fl_speed = 0;
  float fr_speed = 0;
  float bl_speed = 0;
  float br_speed = 0;
  // 1 deg = 0.56mm

  float robotRadius = 0;

public:
  MechanumDrivetrain(motor FL, motor FR, motor BL, motor BR,
                     int _LRWheelDist)
      : fl(FL), fr(FR), bl(BL), br(BR) {
    fl = FL;
    fr = FR;
    bl = BL;
    br = BR;

    robotRadius = _LRWheelDist / 2;
  }
  float acceleration = 20;
  float stoppingTreshold = 20;
  float maxSpeed = 100;

  enum travelDirection {
    north,
    northEast,
    east,
    southEast,
    south,
    southWest,
    west,
    northWest
  };

  enum turnDirection { left, right };

  void DriveFor(int distance_CM, travelDirection dir, int speed) {
    float deg = distance_CM / 0.564;
    float fr_deg = 0, fl_deg = 0, br_deg = 0, bl_deg = 0;
    switch (dir) {
    case travelDirection::north:
      fr_deg = fl_deg = br_deg = bl_deg = deg;
      break;
    case travelDirection::northEast:
      fl_deg = br_deg = deg;
      break;
    case travelDirection::east:
      fr_deg = bl_deg = -deg;
      fl_deg = br_deg = deg;
      break;
    case travelDirection::southEast:
      fl_deg = br_deg = -deg;
      break;
    case travelDirection::south:
      fr_deg = fl_deg = br_deg = bl_deg = -deg;
      break;
    case travelDirection::southWest:
      fr_deg = bl_deg = -deg;
      break;
    case travelDirection::west:
      fr_deg = bl_deg = deg;
      fl_deg = br_deg = -deg;
      break;
    case travelDirection::northWest:
      fr_deg = bl_deg = deg;
      break;
    }
    fr.spinFor(fr_deg, vex::degrees, speed, vex::velocityUnits::pct, false);
    fl.spinFor(fl_deg, vex::degrees, speed, vex::velocityUnits::pct, false);
    br.spinFor(br_deg, vex::degrees, speed, vex::velocityUnits::pct, false);
    bl.spinFor(bl_deg, vex::degrees, speed, vex::velocityUnits::pct, true);
  }

  void TurnFor(int angle_deg, turnDirection dir, int speed) {
    float wheel_deg = (robotRadius * angle_deg) / 101.6;
    float left = dir == turnDirection::left ? -wheel_deg : wheel_deg;
    float right = dir == turnDirection::left ? wheel_deg : -wheel_deg;

    fr.spinFor(vex::forward, right, rotationUnits::deg, speed, velocityUnits::pct, false);
    br.spinFor(vex::forward, right, rotationUnits::deg, speed, velocityUnits::pct, false);
    fl.spinFor(vex::forward, left, rotationUnits::deg, speed, velocityUnits::pct, false);
    bl.spinFor(vex::forward, left, rotationUnits::deg, speed, velocityUnits::pct, true);
  }

  void ManualControl() {
    int forwardVal = Controller1.Axis2.position(vex::percent);
    int sidewaysVal = Controller1.Axis1.position(vex::percent);
    int turnVal = Controller1.Axis4.position(vex::percent);

    float fr_target = forwardVal - sidewaysVal + turnVal;
    float fl_target = forwardVal + sidewaysVal - turnVal;
    float br_target = forwardVal + sidewaysVal + turnVal;
    float bl_target = forwardVal - sidewaysVal - turnVal;

    float fr_increaseBy = (fr_target - fr_speed) / acceleration;
    fr_speed += fr_increaseBy;
    if (floatWithin(fr_speed, -stoppingTreshold, stoppingTreshold) &&
        floatWithin(fr_target, -stoppingTreshold, stoppingTreshold)) {
      fr.stop();
    } else {
      fr.spin(vex::forward, fr_speed, vex::percent);
    }

    float fl_increaseBy = (fl_target - fl_speed) / acceleration;
    fl_speed += fl_increaseBy;
    if (floatWithin(fl_speed, -stoppingTreshold, stoppingTreshold) &&
        floatWithin(fl_target, -stoppingTreshold, stoppingTreshold)) {
      fl.stop();
    } else {
      fl.spin(vex::forward, fl_speed, vex::percent);
    }

    float br_increaseBy = (br_target - br_speed) / acceleration;
    br_speed += br_increaseBy;
    if (floatWithin(br_speed, -stoppingTreshold, stoppingTreshold) &&
        floatWithin(br_target, -stoppingTreshold, stoppingTreshold)) {
      br.stop();
    } else {
      br.spin(vex::forward, br_speed, vex::percent);
    }

    float bl_increaseBy = (bl_target - bl_speed) / acceleration;
    bl_speed += bl_increaseBy;
    if (floatWithin(bl_speed, -stoppingTreshold, stoppingTreshold) &&
        floatWithin(bl_target, -stoppingTreshold, stoppingTreshold)) {
      bl.stop();
    } else {
      bl.spin(vex::forward, bl_speed, vex::percent);
    }
  }
};

#endif