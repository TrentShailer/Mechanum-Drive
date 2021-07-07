using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern motor Motor_FR;
extern motor Motor_FL;
extern motor Motor_BL;
extern motor Motor_BR;
extern controller Controller1;
extern motor MotorSelector;
extern motor MotorTightener;
extern signature Vision__SIG_1;
extern signature Vision__SIG_2;
extern signature Vision__SIG_3;
extern signature Vision__SIG_4;
extern signature Vision__SIG_5;
extern signature Vision__SIG_6;
extern signature Vision__SIG_7;
extern vision Vision;
extern limit LimitSwitch;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );