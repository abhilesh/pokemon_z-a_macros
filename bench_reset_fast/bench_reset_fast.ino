#include "Adafruit_TinyUSB.h"

// =======================================================
// --- CUSTOM POKKEN CONTROLLER DESCRIPTOR ---
// =======================================================
uint8_t const desc_hid_report[] = {
    0x05, 0x01,        // Usage Page (Generic Desktop)
    0x09, 0x05,        // Usage (Gamepad)
    0xA1, 0x01,        // Collection (Application)

    // 16 Buttons (2 bytes)
    0x05, 0x09,        //   Usage Page (Button)
    0x19, 0x01,        //   Usage Minimum (Button 1)
    0x29, 0x10,        //   Usage Maximum (Button 16)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x75, 0x01,        //   Report Size (1 bit)
    0x95, 0x10,        //   Report Count (16 bits)
    0x81, 0x02,        //   Input (Data,Var,Abs)

    // D-Pad Hat Switch (1 byte)
    0x05, 0x01,        //   Usage Page (Generic Desktop)
    0x09, 0x39,        //   Usage (Hat switch)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x07,        //   Logical Maximum (7)
    0x35, 0x00,        //   Physical Minimum (0)
    0x46, 0x3B, 0x01,  //   Physical Maximum (315)
    0x65, 0x14,        //   Unit (Degrees)
    0x75, 0x04,        //   Report Size (4 bits)
    0x95, 0x01,        //   Report Count (1)
    0x81, 0x42,        //   Input (Data,Var,Abs,Null)

    // D-Pad Padding (4 bits)
    0x75, 0x04,        //   Report Size (4)
    0x95, 0x01,        //   Report Count (1)
    0x81, 0x03,        //   Input (Const,Var,Abs)

    // Left Stick X and Y (2 bytes)
    0x05, 0x01,        //   Usage Page (Generic Desktop)
    0x09, 0x30,        //   Usage (X)
    0x09, 0x31,        //   Usage (Y)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x02,        //   Report Count (2)
    0x81, 0x02,        //   Input (Data,Var,Abs)

    // Right Stick X and Y (2 bytes) - KEEP AT CENTER
    0x09, 0x33,        //   Usage (Rx) - Right stick X
    0x09, 0x34,        //   Usage (Ry) - Right stick Y
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x02,        //   Report Count (2)
    0x81, 0x02,        //   Input (Data,Var,Abs)

    0xC0               // End Collection
};

Adafruit_USBD_HID usb_hid;

// Button definitions
#define BTN_Y      0x01
#define BTN_B      0x02
#define BTN_A      0x04
#define BTN_X      0x08
#define BTN_L      0x10
#define BTN_R      0x20
#define BTN_ZL     0x40
#define BTN_ZR     0x80
#define BTN_MINUS  0x100
#define BTN_PLUS   0x200
#define BTN_LSTICK 0x0400
#define BTN_RSTICK 0x0800
#define BTN_HOME   0x1000
#define BTN_CAPTURE 0x2000

// D-Pad definitions
#define DPAD_UP        0x00
#define DPAD_UP_RIGHT  0x01
#define DPAD_RIGHT     0x02
#define DPAD_DOWN_RIGHT 0x03
#define DPAD_DOWN      0x04
#define DPAD_DOWN_LEFT 0x05
#define DPAD_LEFT      0x06
#define DPAD_UP_LEFT   0x07
#define DPAD_CENTER    0x08

// Stick definitions
#define STICK_UP    0
#define STICK_DOWN  255
#define STICK_LEFT  0
#define STICK_RIGHT 255
#define STICK_CTR   128

// =======================================================
// --- HELPER FUNCTIONS ---
// =======================================================

// Send 8-byte report: [buttons(2)][dpad(1)][L-X][L-Y][R-X][R-Y][padding]
void send_report(uint16_t buttons, uint8_t dpad, uint8_t lx, uint8_t ly) {
  uint8_t report[8] = {0};

  report[0] = buttons & 0xFF;
  report[1] = (buttons >> 8) & 0xFF;
  report[2] = dpad;
  report[3] = lx;
  report[4] = ly;
  report[5] = STICK_CTR; // Right X centered
  report[6] = STICK_CTR; // Right Y centered
  report[7] = 0x00;

  usb_hid.sendReport(0, report, sizeof(report));
}

void pressButton(uint16_t button, int duration) {
  send_report(button, DPAD_CENTER, STICK_CTR, STICK_CTR);
  delay(duration);
  send_report(0, DPAD_CENTER, STICK_CTR, STICK_CTR);
  delay(100); // 100ms cooldown for stability
}

void pressDPad(uint8_t direction, int duration) {
  send_report(0, direction, STICK_CTR, STICK_CTR);
  delay(duration);
  send_report(0, DPAD_CENTER, STICK_CTR, STICK_CTR);
  delay(100); // 100ms cooldown for stability
}

void moveStick(uint8_t x, uint8_t y, int duration) {
  send_report(0, DPAD_CENTER, x, y);
  delay(duration);
}

void resetStick() {
  send_report(0, DPAD_CENTER, STICK_CTR, STICK_CTR);
}

// =======================================================
// --- SETUP (WITH AUTO-LAUNCH) ---
// =======================================================

void setup() {
  // FORCE VID/PID to act as a POKKEN CONTROLLER
  USBDevice.setID(0x0f0d, 0x0092);
  USBDevice.setManufacturerDescriptor("HORI CO.,LTD.");
  USBDevice.setProductDescriptor("POKKEN CONTROLLER");

  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setStringDescriptor("POKKEN CONTROLLER");
  usb_hid.begin();

  // WAIT FOR SWITCH TO RECOGNIZE CONTROLLER
  while(!USBDevice.mounted()) delay(1);
  delay(500);

  // Auto-register as Player 1
  pressButton(BTN_A, 100);

  // CRITICAL DELAY: Wait 4 seconds for Switch to
  // finish registering before sending next command.
  delay(4000);

  // === AUTOMATE GAME LAUNCH ===

  // Press B (first time) - often doesn't register
  pressButton(BTN_B, 100);
  delay(1000);  // Wait for "Controllers" menu

  // Press B (second time) to exit "Change Grip/Order"
  pressButton(BTN_B, 100);
  delay(1000);  // Wait for "Controllers" menu

  // Press B (third time) to exit "Controllers" menu and reach home screen
  pressButton(BTN_B, 100);
  delay(1000);  // Wait for home screen to be stable

  // Navigate to game icon (ADJUST THESE BASED ON YOUR GAME POSITION!)
  // Example: Up once, Left twice
  pressDPad(DPAD_UP, 100);
  delay(500);
  pressDPad(DPAD_LEFT, 100);
  delay(500);
  pressDPad(DPAD_LEFT, 100);
  delay(500);

  // Launch game
  pressButton(BTN_A, 100);
  delay(5000);  // Wait for game to start loading

  // Macro starts NOW!
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500); 
  digitalWrite(LED_BUILTIN, LOW);
}

// =======================================================
// --- MAIN LOOP ---
// =======================================================

void loop() {

  // Send report with: A button pressed + Left stick held down
  send_report(BTN_A, DPAD_CENTER, STICK_CTR, STICK_DOWN);
  delay(50);  // Hold for 100ms
  
  // Release A button but keep stick down
  send_report(0, DPAD_CENTER, STICK_CTR, STICK_DOWN);
  delay(50);  // Wait 100ms before next press
}