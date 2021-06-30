// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Motor_FR             motor         10              
// Motor_FL             motor         1               
// Motor_BL             motor         11              
// Motor_BR             motor         20              
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "Mechanum.h"

using namespace vex;

competition Competition;

MechanumDrivetrain mechanumDrivetrain = MechanumDrivetrain(220);

void pre_auton(void) { vexcodeInit(); }

void autonomous(void) {
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::north, 25);
  wait(1000, msec);
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::northEast, 25);
  wait(1000, msec);
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::east, 25);
  wait(1000, msec);
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::southEast, 25);
  wait(1000, msec);
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::south, 25);
  wait(1000, msec);
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::southWest, 25);
  wait(1000, msec);
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::west, 25);
  wait(1000, msec);
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::northWest, 25);
  wait(1000, msec);

  mechanumDrivetrain.TurnFor(90, MechanumDrivetrain::turnDirection::left, 15);
  wait(1000, msec);
  mechanumDrivetrain.TurnFor(180, MechanumDrivetrain::turnDirection::right, 15);
  wait(1000, msec);
}

void usercontrol(void) {
  while (1) {
    mechanumDrivetrain.ManualControl();
    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
