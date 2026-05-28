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
#define BLOOD 0xb50015

int auton_selector = 0; // The Auton Selector variable, 0 for left, 1 for right, and so on for custom like skills
int auton_test = 0; // Engage to test auton and run it after 5 seconds, disengage to just select it

void touchUI()
{
	pros::screen::erase(); // Reset the screen

	// Draw the UI :3
	pros::screen::set_pen(LAVANS_HEART_RN);
	pros::screen::fill_rect(0, 0, 480, 240);
	pros::screen::set_pen(BURNT_ROSE);
	pros::screen::fill_rect(251, 49, 339, 91); // Left Auton Button
	pros::screen::fill_rect(364, 49, 452, 91); // Right Auton Button
	pros::screen::fill_rect(21, 136, 109, 178); // Select Button
	pros::screen::fill_rect(118, 136, 206, 178); // Test Button
	pros::screen::set_pen(WARM_YELLOW);
	pros::screen::print(TEXT_LARGE, 30, 30, "The Dawn of 21503A");
	pros::screen::print(TEXT_MEDIUM, 268, 64, "Left Auton");
	pros::screen::print(TEXT_MEDIUM, 378, 64, "Right Auton");


	// Auton Selector / Tester
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

			else if (touch.x > 21 && touch.x < 109 && touch.y > 136 && touch.y < 178) {
				auton_test = 0; // Toggle Auton Selection
				pros::delay(300);
			}

			else if (touch.x > 118 && touch.x < 206 && touch.y > 136 && touch.y < 178) {
				auton_test = 1; // Toggle Auton Testing
				pros::delay(300);
				
			}
			// Put the Highlight Here to show if it was selected as well
			
		}
	}

	// // Auton Tester
	// while (true)
	// {
	// 	pros::screen_touch_status_s_t touch= pros::screen::touch_status();
	// 	if(touch.touch_status == pros::E_TOUCH_PRESSED) {
	// 		if(touch.x > 21 && touch.x < 109 && touch.y > 136 && touch.y < 178) {
	// 			auton_test = 0; // Toggle Auton Selection
	// 			pros::delay(300);
	// 		}
	// 		// Put the Highlight Here to show if it was toggled

	// 		else if(touch.x > 118 && touch.x < 206 && touch.y > 136 && touch.y < 178) {
	// 			auton_test = 1; // Toggle Auton Testing
	// 			// Maybe a button to reset the selection or something, up to u
	// 			pros::delay(300);
	// 		}
	// 		// Put the Highlight Here to show if it was toggled as well
			
	// 	}
	// }
}




void initialize() 
{
	touchUI();
	while (pros::competition::is_disabled()) {
			if (auton_test ==1)
			{	
			// Do the test waiting thing
			for (int i = 5; i > 0; i--) {
				pros::screen::print(TEXT_LARGE, 30, 30, "Auton Starting in %d seconds", i);
				pros::delay(1000);
			}
		}
		autonomous(); // Run the selected auton

		auton_test = 0; // Reset the auton test variable

		pros::delay(20);
	}
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
	// Use the Auton Selector Variable :)
	switch (auton_selector) {
			
		default:
			// Safety fallback when nothing is selected
			pros::screen::print(TEXT_LARGE, 30, 30, "NOTHING IS SELECTED ;3 (fix or I'll eat u)");
			break;


		case 0:
			// Left Auton Code Here

			pros::screen::set_pen(BLOOD);
			pros::screen::fill_rect(251, 49, 339, 91); // Left Auton Button Highlighted
			pros::screen::set_pen(WARM_YELLOW);
			pros::screen::print(TEXT_LARGE, 30, 30, "Left Auton Selected");
			pros::screen::print(TEXT_MEDIUM, 268, 64, "Left Auton");

			// Remove this if u want
			pros::delay(300);
			pros::screen::print(TEXT_LARGE, 30, 30, "there's no auton bro");
			break;


		case 1:
			// Right Auton Code Here
			pros::screen::set_pen(BLOOD);
			pros::screen::fill_rect(364, 49, 452, 91); // Right Auton Button
			pros::screen::set_pen(WARM_YELLOW);
			pros::screen::print(TEXT_LARGE, 30, 30, "Right Auton Selected");
			pros::screen::print(TEXT_MEDIUM, 378, 64, "Right Auton");

			// Remove this if u want
			pros::delay(300);
			pros::screen::print(TEXT_LARGE, 30, 30, "there's no auton bro");
			break;


		case 2:
			// Skills or something I guess
			pros::screen::print(TEXT_LARGE, 30, 30, "SKILLS AUTON SELECTED");
			pros::delay(300);
			pros::screen::print(TEXT_LARGE, 30, 30, "there's no auton bro, u have a skill issue");
			break;
	}
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