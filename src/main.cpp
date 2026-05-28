//.............................
// Module: main.cpp
// Original Owner: TYRN1172
// Team Name: 21503A
// Competition: V5RC 2026-2027
//.............................


#include "main.h"
#include "lemlib/api.hpp"
#include "pros/apix.h" 
#include "liblvgl/lvgl.h"

#define LAVANS_HEART_RN 0x7A0C38
#define BURNT_ROSE 0x984447
#define WARM_YELLOW 0xF9FAC3

int auton_selector = 0; // The Auton Selector variable, 0 for left, 1 for right, and so on for custom like skills

void touchUI()
{
	pros::screen::erase(); // Reset the screen

	// Draw the UI :3
	pros::screen::set_pen(LAVANS_HEART_RN);
	pros::screen::fill_rect(0, 0, 480, 240);
	pros::screen::set_pen(BURNT_ROSE);
	pros::screen::fill_rect(251, 49, 339, 91);
	pros::screen::fill_rect(364, 49, 452, 91);
	pros::screen::set_pen(WARM_YELLOW);
	pros::screen::print(TEXT_LARGE, 30, 30, "The Dawn of 21503A");
	pros::screen::print(TEXT_MEDIUM, 268, 64, "Left Auton");
	pros::screen::print(TEXT_MEDIUM, 378, 64, "Right Auton");


	// Auton Selector
	while (true)
	{
		pros::screen_touch_status_s_t touch= pros::screen::touch_status();
		if(touch.touch_status == pros::E_TOUCH_PRESSED) {
			if(touch.x > 251 && touch.x < 339 && touch.y > 49 && touch.y < 91) {
				auton_selector = 0; // Left Auton
				pros::delay(300);
			}
			// Put the Highlight Here to show if it was selected

			else if(touch.x > 364 && touch.x < 452 && touch.y > 49 && touch.y < 91) {
				auton_selector = 1; // Right Auton
				pros::delay(300);
			}
			// Put the Highlight Here to show if it was selected as well
			
		}
	}
}


void initialize() 
{
	touchUI();
}


// Needed to make sure the robot is disabled after driver or auton
void disabled() 
{

}


// Initializes the robot before a match
void competition_initialize() 
{

}


// What do u think this is?
void autonomous() 
{

}


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