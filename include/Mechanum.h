#ifndef Mechanum
#define Mechanum

#include "util.h"
#include "vex.h"
#include <iostream>
#include <vex_task.h>

using namespace vex;

class MechanumDrivetrain {
private:
  float fl_speed = 0;
  float fr_speed = 0;
  float bl_speed = 0;
  float br_speed = 0;

  float robotRadius = 0;

public:
  MechanumDrivetrain(int _LRWheelDist) { robotRadius = _LRWheelDist / 2; }
  float acceleration = 20;
  float stoppingTreshold = 5;
  float maxSpeed = 100;

  enum travelDirection {
    north = 0,
    northEast = 45,
    east = 90,
    southEast = 135,
    south = 180,
    southWest = -135,
    west = -90,
    northWest = -45
  };

  enum turnDirection { left, right };

  void DriveFor(int distance_MM, MechanumDrivetrain::travelDirection dir, int speed) {
    float deg = distance_MM / 0.88663;
    float fr_deg = 0, fl_deg = 0, br_deg = 0, bl_deg = 0;
    switch (dir) {
    case travelDirection::north:
      fr_deg = fl_deg = br_deg = bl_deg = deg;
      break;
    case travelDirection::northEast:
      fr_deg = bl_deg = deg * 2;
      fl_deg = br_deg = -deg / 5;
      break;
    case travelDirection::east:
      fl_deg = br_deg = -deg;
      fr_deg = bl_deg = deg;
      break;
    case travelDirection::southEast:
      fr_deg = bl_deg = -deg * 2;
      fl_deg = br_deg = deg / 5;
      break;
    case travelDirection::south:
      fr_deg = fl_deg = br_deg = bl_deg = -deg;
      break;
    case travelDirection::southWest:
      fl_deg = br_deg = -deg * 2;
      fr_deg = bl_deg = deg / 5;
      break;
    case travelDirection::west:
      fl_deg = br_deg = deg;
      fr_deg = bl_deg = -deg;
      break;
    case travelDirection::northWest:
      fl_deg = br_deg = deg * 2;
      fr_deg = bl_deg = -deg / 5;
      break;
    }
    Motor_FR.spinFor(fr_deg, vex::degrees, speed, vex::velocityUnits::pct,
                     false);
    Motor_FL.spinFor(fl_deg, vex::degrees, speed, vex::velocityUnits::pct,
                     false);
    Motor_BR.spinFor(br_deg, vex::degrees, speed, vex::velocityUnits::pct,
                     false);
    Motor_BL.spinFor(bl_deg, vex::degrees, speed, vex::velocityUnits::pct,
                     true);
  }

  void TurnFor(int angle_deg, MechanumDrivetrain::turnDirection dir, int speed) {
    float wheel_deg = (robotRadius * angle_deg) / 101.6;
    float left = dir == turnDirection::left ? -wheel_deg : wheel_deg;
    float right = dir == turnDirection::left ? wheel_deg : -wheel_deg;

    Motor_FR.spinFor(vex::forward, right, rotationUnits::deg, speed,
                     velocityUnits::pct, false);
    Motor_BR.spinFor(vex::forward, right, rotationUnits::deg, speed,
                     velocityUnits::pct, false);
    Motor_FL.spinFor(vex::forward, left, rotationUnits::deg, speed,
                     velocityUnits::pct, false);
    Motor_BL.spinFor(vex::forward, left, rotationUnits::deg, speed,
                     velocityUnits::pct, true);
  }

  void ManualControl() {
    int forwardVal = Controller1.Axis3.position(vex::percent);
    int sidewaysVal = Controller1.Axis4.position(vex::percent);
    int turnVal = Controller1.Axis1.position(vex::percent) / 4;

    if(floatWithin(forwardVal, -15, 15)){
      forwardVal = 0;
    }
    if(floatWithin(sidewaysVal, -15, 15)){
      sidewaysVal = 0;
    }
    if(floatWithin(turnVal, -15, 15)){
      turnVal = 0;
    }
    

    float fr_target = forwardVal + sidewaysVal - turnVal;
    float fl_target = forwardVal - sidewaysVal + turnVal;
    float br_target = forwardVal - sidewaysVal - turnVal;
    float bl_target = forwardVal + sidewaysVal + turnVal;

    float fr_increaseBy = (fr_target - fr_speed) / acceleration;
    fr_speed += fr_increaseBy;
    if (floatWithin(fr_speed, -stoppingTreshold, stoppingTreshold) &&
        floatWithin(fr_target, -stoppingTreshold, stoppingTreshold)) {
      Motor_FR.stop();
    } else {
      Motor_FR.spin(vex::forward, fr_speed * (maxSpeed / 100), vex::percent);
    }

    float fl_increaseBy = (fl_target - fl_speed) / acceleration;
    fl_speed += fl_increaseBy;
    if (floatWithin(fl_speed, -stoppingTreshold, stoppingTreshold) &&
        floatWithin(fl_target, -stoppingTreshold, stoppingTreshold)) {
      Motor_FL.stop();
    } else {
      Motor_FL.spin(vex::forward, fl_speed * (maxSpeed / 100), vex::percent);
    }

    float br_increaseBy = (br_target - br_speed) / acceleration;
    br_speed += br_increaseBy;
    if (floatWithin(br_speed, -stoppingTreshold, stoppingTreshold) &&
        floatWithin(br_target, -stoppingTreshold, stoppingTreshold)) {
      Motor_BR.stop();
    } else {
      Motor_BR.spin(vex::forward, br_speed * (maxSpeed / 100), vex::percent);
    }

    float bl_increaseBy = (bl_target - bl_speed) / acceleration;
    bl_speed += bl_increaseBy;
    if (floatWithin(bl_speed, -stoppingTreshold, stoppingTreshold) &&
        floatWithin(bl_target, -stoppingTreshold, stoppingTreshold)) {
      Motor_BL.stop();
    } else {
      Motor_BL.spin(vex::forward, bl_speed * (maxSpeed / 100), vex::percent);
    }
  }
};

#endif