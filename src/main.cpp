//.............................
// Module: main.cpp
// Original Owner: TYRN1172
// Team Name: 21503A
// Competition: V5RC 2026-2027
//.............................


#include "main.h"
#include "lemlib/api.hpp"
#include "pros/apix.h" 



void initialize() 
{
	// Write LVGL Code Here

}


// Needed to make sure the robot is disabled after driver or auton
void disabled() {}


// Initializes the robot before a match
void competition_initialize() {}


// What do u think this is?
void autonomous() {}


// Kinda like Usercontrol but lowk worse
void opcontrol() {
	// Defining Motors, Variables relevant to Drivers, etc

	// Maknig Motor Groups :d
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({1, -2, 3});    // Motor group with forwards ports 1 & 3 and reversed port 2
	pros::MotorGroup right_mg({-4, 5, -6});  // Motor group with forwards port 5 and reversed ports 4 & 6


	while (true) {
		
		// Split Arcade that Alex Liked for some stupid reason
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir - turn);                      // Sets left motor voltage
		right_mg.move(dir + turn);                     // Sets right motor voltage
		pros::delay(20);                               // Run for 20 ms then update


	}
}