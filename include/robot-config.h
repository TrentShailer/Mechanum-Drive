using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor Motor_FR;
extern motor Motor_FL;
extern motor Motor_BL;
extern motor Motor_BR;
extern controller Controller1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );