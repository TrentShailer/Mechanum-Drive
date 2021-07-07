#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor Motor_FR = motor(PORT10, ratio18_1, true);
motor Motor_FL = motor(PORT1, ratio18_1, false);
motor Motor_BL = motor(PORT11, ratio18_1, false);
motor Motor_BR = motor(PORT20, ratio18_1, true);
controller Controller1 = controller(primary);
motor MotorSelector = motor(PORT15, ratio18_1, false);
motor MotorTightener = motor(PORT16, ratio18_1, false);
/*vex-vision-config:begin*/
vision Vision = vision (PORT2, 50);
/*vex-vision-config:end*/
limit LimitSwitch = limit(Brain.ThreeWirePort.A);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}