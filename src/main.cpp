//.............................//
//  Module: main.cpp           //
//  Original Owner: TYRN1172   //
//  Team Name: 21503A          //
//  Competition: V5RC 2026-2027//
//.............................//

#include "main.h"
#include "pros/apix.h"   // pulls in LVGL (liblvgl) for PROS 4.x
#include "liblvgl/lvgl.h"
#include "lemlib/api.hpp"  // re-enable only once LemLib is installed in the project

// Works on both LVGL 8 and LVGL 9, so the project builds whichever liblvgl is installed
#if LVGL_VERSION_MAJOR >= 9
  #define LV_BTN_CREATE(p) lv_button_create(p)
  #define LV_SCR_ACTIVE()  lv_screen_active()
#else
  #define LV_BTN_CREATE(p) lv_btn_create(p)
  #define LV_SCR_ACTIVE()  lv_scr_act()
#endif

// LVGL Color Definitions
#define COLOR_LAVAN  lv_color_hex(0x7A0C38)
#define COLOR_ROSE   lv_color_hex(0x984447)
#define COLOR_YELLOW lv_color_hex(0xF9FAC3)
#define COLOR_BLOOD  lv_color_hex(0xB50015)

// Global State
int auton_selector = 0; // 0: Left, 1: Right, 2: Skills
int auton_test = 0;     // 0: Select mode, 1: Test mode

// UI Object Pointers
static lv_obj_t* btn_left = nullptr;
static lv_obj_t* btn_right = nullptr;
static lv_obj_t* btn_skills = nullptr;
static lv_obj_t* btn_select = nullptr;
static lv_obj_t* btn_test = nullptr;
static lv_obj_t* status_label = nullptr;

// Test-run state (touched from the LVGL task and the auton task)
static lv_timer_t* test_timer = nullptr;
static int countdown = 0;
static volatile bool auton_running = false;
static volatile bool auton_done = false;

static void set_button_color(lv_obj_t* btn, lv_color_t color) {
  lv_obj_set_style_bg_color(btn, color, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(btn, LV_OPA_COVER, LV_PART_MAIN);
}

static void update_button_styles() {
  if (!btn_left || !btn_right || !btn_skills || !btn_select || !btn_test) return;
  set_button_color(btn_left,   auton_selector == 0 ? COLOR_BLOOD : COLOR_ROSE);
  set_button_color(btn_right,  auton_selector == 1 ? COLOR_BLOOD : COLOR_ROSE);
  set_button_color(btn_skills, auton_selector == 2 ? COLOR_BLOOD : COLOR_ROSE);
  set_button_color(btn_select, auton_test == 0 ? COLOR_BLOOD : COLOR_ROSE);
  set_button_color(btn_test,   auton_test == 1 ? COLOR_BLOOD : COLOR_ROSE);
}

// Runs the autonomous routine on its own PROS task so the LVGL task is never blocked
static void auton_runner(void* param) {
  autonomous();
  auton_done = true;
}

// LVGL timer (runs inside the LVGL task, so touching widgets here is safe).
// Fires once per second: counts down, launches auton, then waits for it to finish.
static void test_timer_cb(lv_timer_t* t) {
  if (countdown > 0) {
    lv_label_set_text_fmt(status_label, "Auton starting in %d...", countdown);
    countdown--;
    return;
  }

  if (!auton_running && !auton_done) {
    auton_running = true;
    lv_label_set_text(status_label, "Running Autonomous...");
    pros::Task runner(auton_runner, nullptr, "Auton Test");
    return;
  }

  if (auton_done) {
    auton_running = false;
    auton_done = false;
    auton_test = 0;
    update_button_styles();
    lv_label_set_text(status_label, "Test Complete. System Ready.");
    lv_timer_t`(t);
    test_timer = nullptr;
  }
}

static void btn_event_cb(lv_event_t* e) {
  lv_obj_t* target = static_cast<lv_obj_t*>(lv_event_get_target(e));

  // Ignore input while a test run is in progress
  if (test_timer != nullptr) return;

  if (target == btn_left) {
    auton_selector = 0;
    lv_label_set_text(status_label, "Selected: Left Auton");
  } else if (target == btn_right) {
    auton_selector = 1;
    lv_label_set_text(status_label, "Selected: Right Auton");
  } else if (target == btn_skills) {
    auton_selector = 2;
    lv_label_set_text(status_label, "Selected: Skills");
  } else if (target == btn_select) {
    auton_test = 0;
    lv_label_set_text(status_label, "Auton Locked in.");
  } else if (target == btn_test) {
    auton_test = 1;
    countdown = 5;
    auton_running = false;
    auton_done = false;
    test_timer = lv_timer_create(test_timer_cb, 1000, nullptr);
  }

  update_button_styles();
}

// Creates a labelled button using the standard LVGL v8 button widget
static lv_obj_t* make_button(lv_obj_t* parent, const char* text, int32_t x, int32_t y) {
  lv_obj_t* btn = LV_BTN_CREATE(parent);
  lv_obj_set_size(btn, 100, 45);
  lv_obj_align(btn, LV_ALIGN_TOP_LEFT, x, y);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, nullptr);

  lv_obj_t* lbl = lv_label_create(btn);
  lv_label_set_text(lbl, text);
  lv_obj_set_style_text_color(lbl, COLOR_YELLOW, LV_PART_MAIN);
  lv_obj_center(lbl);
  return btn;
}

void create_gui() {
  lv_obj_t* scr = LV_SCR_ACTIVE();
  lv_obj_set_style_bg_color(scr, COLOR_LAVAN, LV_PART_MAIN);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, LV_PART_MAIN);

  lv_obj_t* title = lv_label_create(scr);
  lv_label_set_text(title, "The Dawn of 21503A");
  lv_obj_set_style_text_color(title, COLOR_YELLOW, LV_PART_MAIN);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 20, 15);

  status_label = lv_label_create(scr);
  lv_label_set_text(status_label, "Select Autonomous Routine");
  lv_obj_set_style_text_color(status_label, COLOR_YELLOW, LV_PART_MAIN);
  lv_obj_align(status_label, LV_ALIGN_TOP_LEFT, 20, 45);

  btn_left   = make_button(scr, "Left",   20,  90);
  btn_right  = make_button(scr, "Right",  130, 90);
  btn_skills = make_button(scr, "Skills", 240, 90);
  btn_select = make_button(scr, "Select", 20,  150);
  btn_test   = make_button(scr, "Test",   130, 150);

  update_button_styles();
}

void initialize() {
  create_gui();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
  switch (auton_selector) {
    case 0:
      // Left Auton Code
      pros::delay(300);
      break;
    case 1:
      // Right Auton Code
      pros::delay(300);
      break;
    case 2:
      // Skills Routine
      pros::delay(300);
      break;
    default:
      break;
  }
}

void opcontrol() {
  pros::Controller master(pros::E_CONTROLLER_MASTER);
  pros::MotorGroup left_mg({1, -2, 3});
  pros::MotorGroup right_mg({-4, 5, -6});

  while (true) {
    int dir = master.get_analog(ANALOG_LEFT_Y);
    int turn = master.get_analog(ANALOG_RIGHT_X);

    left_mg.move(dir - turn);
    right_mg.move(dir + turn);

    pros::delay(20);
  }
}