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
	pros::screen::set_eraser(LAVANS_HEART_RN);
	pros::screen::erase(); // Reset the screen

	// Color Management for UI Buttons
	if (auton_selector == 0)
	{
		pros::screen::set_pen(BLOOD);
	} else {
		pros::screen::set_pen(BURNT_ROSE);}
	pros::screen::fill_rect(251, 49, 339, 91); // Left Auton Button
	if (auton_selector == 1)
	{
		pros::screen::set_pen(BLOOD);
	} else {
		pros::screen::set_pen(BURNT_ROSE);}
	pros::screen::fill_rect(364, 49, 452, 91); // Right Auton Button
	if (auton_test == 0)
	{
		pros::screen::set_pen(BLOOD);
	} else {
		pros::screen::set_pen(BURNT_ROSE);}
	pros::screen::fill_rect(21, 136, 109, 178); // Select Button
	if (auton_test == 1)
	{
		pros::screen::set_pen(BLOOD);
	} else {
		pros::screen::set_pen(BURNT_ROSE);}
	pros::screen::fill_rect(118, 136, 206, 178); // Test Button
	

	// Draw the UI :3
	pros::screen::set_pen(WARM_YELLOW);
	pros::screen::print(TEXT_LARGE, 30, 30, "The Dawn of 21503A");
	pros::screen::print(TEXT_MEDIUM, 268, 64, "Left Auton");
	pros::screen::print(TEXT_MEDIUM, 378, 64, "Right Auton");
	pros::screen::print(TEXT_MEDIUM, 32, 151, "Select Auton");
	pros::screen::print(TEXT_MEDIUM, 135, 151, "Test Auton");

	while (true)
	{

		if (pros::competition::is_disabled() == false) {
			break; // Exit the loop if the robot is enabled
		}

		pros::screen_touch_status_s_t touch = pros::screen::touch_status(); // Get the touch status

		if (touch.touch_status == pros::E_TOUCH_PRESSED) { // Check if the screen is being touched

			if(touch.x > 251 && touch.x < 339 && touch.y > 49 && touch.y < 91) {
				auton_selector = 0;
				break;
			} else if (touch.x > 364 && touch.x < 452 && touch.y > 49 && touch.y < 91) {
				auton_selector = 1;
				break;
			} else if (touch.x > 21 && touch.x < 109 && touch.y > 136 && touch.y < 178) {
				auton_test = 0; // Toggle the auton test variable
				break;
			} else if (touch.x > 118 && touch.x < 206 && touch.y > 136 && touch.y < 178) {
				auton_test = 1; // Toggle the auton test variable
				break;
			}
		} pros::delay(20); // Delay to prevent excessive CPU usage
	}
}




void initialize() 
{
	while (pros::competition::is_disabled() == true)
	{
		touchUI();

		// Run when Test is Selected
		if (auton_test == 1)
		{	
			pros::screen::set_eraser(LAVANS_HEART_RN);
			pros::screen::erase(); // Clear the screen for the test
			pros::screen::set_pen(WARM_YELLOW);
			// Do the test waiting thing
			for (int i = 5; i > 0; i--) {
				if (i < 5){
				pros::screen::erase(); // Clear the screen for the next message
				}

				pros::screen::print(TEXT_LARGE, 30, 30, "Auton Starting in %d seconds", i);
				pros::delay(1000);
			}

			pros::screen::erase(); // Clear the screen for the auton
			pros::screen::print(TEXT_LARGE, 30, 30, "Running Auton");
			
			autonomous(); // Run the selected auton

			auton_test = 0; // Reset the auton test variable

			pros::delay(20);
		}
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

			// Remove this if u want
			pros::delay(300);
			pros::screen::print(TEXT_LARGE, 30, 30, "there's no auton bro");
			break;


		case 1:
			// Right Auton Code Here

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