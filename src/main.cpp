#include "Auton-Select.h"
#include "Mechanum.h"
#include "util.h"
#include "vex.h"
#include <vex_task.h>

using namespace vex;

competition Competition;

MechanumDrivetrain mechanumDrivetrain = MechanumDrivetrain(220);

side selectedSide = side::none;
TowerColour selectedTeam = TowerColour::red;

enum ArmDirections {
  north = 0,
  east = 90,
  south = 180,
  west = -90,
};

ArmDirections currentArm = north;
bool canSelect = true;

int letSelect() {
  canSelect = false;
  vex::this_thread::sleep_for(2000);
  canSelect = true;
  return 1;
}

void SelectArm(ArmDirections target, bool blocking = false) {
  if (!blocking)
    thread select(letSelect);

  // Use MotorSelector to find how far to rotate to get to given arm
  int deg = target - currentArm;
  currentArm = target;
  // Might need to solve clicks
  MotorSelector.spinFor(directionType::fwd, deg, rotationUnits::deg, blocking);
}

//	640 x 400 Pixels
//  50fps = 0.02hz

int centreTolorance = 20;
int sizeTolorance = 10;
int targetWidth = 50;
int targetHeight = 30;

MechanumDrivetrain::travelDirection
GetDirection(MechanumDrivetrain::travelDirection relativeDirection,
             ArmDirections globalDirection) {

  int newDir = globalDirection + relativeDirection;
  if (newDir == -180)
    newDir = 180;

  return (MechanumDrivetrain::travelDirection)(newDir);
}

void PickupTower(TowerColour towerColour) {
  // Select signature based off tower colour
  Vision.takeSnapshot(towerColour); // R0, B1, Y2
  bool inPlace = false;
  // If tower found
  if (Vision.objectCount > 0) {
    // While not in correct position
    while (!inPlace) {
      bool moved = false;
      // Take photo
      Vision.takeSnapshot(towerColour);
      VexVisionObject obj = Vision.largestObject;

      // Center bot on tower
      if (obj.centerX > 320 + centreTolorance) {
        moved = true;
        // move left
        mechanumDrivetrain.DriveFor(
            75, MechanumDrivetrain::travelDirection::west, 25);
      } else if (obj.centerY < 320 - centreTolorance) {
        moved = true;
        // move right
        mechanumDrivetrain.DriveFor(
            75,
            GetDirection(MechanumDrivetrain::travelDirection::east, currentArm),
            25);
      }

      // Get a specific distance from tower
      if (obj.width > targetWidth + sizeTolorance &&
          obj.height > targetHeight + sizeTolorance) {
        moved = true;
        // Too close
        mechanumDrivetrain.DriveFor(
            75, MechanumDrivetrain::travelDirection::south, 25);
      } else if (obj.width < targetWidth - sizeTolorance &&
                 obj.height < targetHeight - sizeTolorance) {
        moved = true;
        // Too far
        mechanumDrivetrain.DriveFor(
            75, MechanumDrivetrain::travelDirection::north, 25);
      }

      if (!moved)
        break;
      wait(20, msec);
    }

    // Lower arms
    MotorTightener.spinFor(directionType::fwd, 60, rotationUnits::deg);
    // Move forward specified distance so that arms are under tower
    mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                                25);
    // lift arms
  } else
    return;
}

void LeftAuton() {
  // Roughly position infront of goal

  // Push goal up onto balance platfrom whilst deccellerating
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                              50);
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                              25);
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                              12);
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                              6);
}

void RightAuton() {
  // Position so forward is yellow goal

  mechanumDrivetrain.DriveFor(1500, MechanumDrivetrain::travelDirection::north,
                              75);
  PickupTower(TowerColour::yellow);
  mechanumDrivetrain.DriveFor(1200, MechanumDrivetrain::travelDirection::west,
                              75);
  mechanumDrivetrain.TurnFor(90, MechanumDrivetrain::turnDirection::right, 50);
  SelectArm(west, true);
  PickupTower(TowerColour::yellow);
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::north,
                              75);
  mechanumDrivetrain.DriveFor(1200, MechanumDrivetrain::travelDirection::east,
                              75);
  MotorTightener.spinFor(directionType::fwd, 60, rotationUnits::deg);
  SelectArm(north, true);
  MotorTightener.spinFor(directionType::fwd, 60, rotationUnits::deg);
}

void LeftAutonWP() {
  // Roughly position infront of goal

  // Push goal up onto balance platfrom whilst deccellerating
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                              50);
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                              25);
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                              12);
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::north,
                              6);
}

void RightAutonWP() {
  // Roughly position bot infront of goal

  // Drive towards goal
  mechanumDrivetrain.DriveFor(600, MechanumDrivetrain::travelDirection::north,
                              50);
  // centre on and pickup team tower
  PickupTower(selectedTeam);
  // Reverse from line
  mechanumDrivetrain.DriveFor(300, MechanumDrivetrain::travelDirection::south,
                              50);
  // Drop tower
  MotorTightener.spinFor(directionType::fwd, 60, rotationUnits::deg);
}

bool wp = false;

void pre_auton(void) {
  vexcodeInit();
  mechanumDrivetrain.acceleration = 10;
  mechanumDrivetrain.maxSpeed = 50;
  selectedSide = InitAutonSelect();
  selectedTeam = InitTeamSelect();
  wp = InitAutonWinPoint();
}

void autonomous(void) {
  if (selectedSide == side::left) {
    if (wp)
      LeftAutonWP();
    else
      LeftAuton();
  } else if (selectedSide == side::right) {
    if (wp)
      RightAutonWP();
    else
      RightAuton();
  }
}

bool canDrive = true;

void usercontrol(void) {
  while (1) {
    mechanumDrivetrain.ManualControl();
    if (canSelect) {
      if (Controller1.ButtonUp.pressing())
        SelectArm(ArmDirections::north);
      else if (Controller1.ButtonRight.pressing())
        SelectArm(ArmDirections::east);
      else if (Controller1.ButtonDown.pressing())
        SelectArm(ArmDirections::south);
      else if (Controller1.ButtonLeft.pressing())
        SelectArm(ArmDirections::west);
    }

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
