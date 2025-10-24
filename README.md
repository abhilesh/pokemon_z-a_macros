# Pokémon Z-A Macros for Raspberry Pi Pico

Automate repetitive tasks in Pokémon Legends: Z-A using a Raspberry Pi Pico that emulates a HORI POKKEN CONTROLLER. Automatically registers as a controller, launches the game, and performs in-game actions like fast traveling for re-rolling Pokémon spawns

## 🛠️ Requirements

**Hardware:**

- [Raspberry Pi Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)
- [USB-A to USB Micro-B Cable](https://thepihut.com/products/usb-to-micro-usb-cable-0-5m?variant=37979679228099&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gad_source=1&gad_campaignid=11673057096&gbraid=0AAAAADfQ4GHcz-DwliEvvdjPE9UTbI18I&gclid=CjwKCAjwx-zHBhBhEiwA7Kjq63K0QrQuu4RC5P4cmuPcmLkHypATLf34tSD7fhUpMgi2cGBGcOEBvxoCSj8QAvD_BwE)
- [Nintendo Switch](https://store.nintendo.co.uk/en/nintendo-switch-with-neon-blue-neon-red-joy-con-controllers-000000000010010739)

> Tested on Nintendo Switch 1 and the load times may vary on different models. Check and adjust delays accordingly.

**Software:**

1. **Arduino IDE** - [arduino.cc](https://www.arduino.cc/en/software)
2. **Board Package** - [Earle Philhower's arduino-pico](https://github.com/earlephilhower/arduino-pico)
3. **Library** - Adafruit TinyUSB Library

> "Adafruit TinyUSB" is bundled with `arduino-pico`, but you may need to install it via Library Manager if not available.

## 📦 IDE & Project

1. Install Arduino IDE from [here](https://www.arduino.cc/en/software).
2. Open Arduino IDE and add the `arduino-pico` board package:
   - Go to `File` → `Preferences` (or `Settings` on macOS) and add the following URL to the "Additional Board URLs" field:
   `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json`
   - Select `Tools` → `Board Manager` → Search for `rp2040`
   - Install "Raspberry Pi Pico/RP2040/RP2350" by Earle F. Philhower, III
3. Restart Arduino IDE to apply changes.
4. Clone this repository or download the ZIP and extract it.
5. Open one of the `.ino` macro files in Arduino IDE.

## 📥 Uploading the Macro

1. Connect Raspberry Pi Pico to your computer while holding the `BOOTSEL` button to enter USB mass storage mode.
2. Open the macro `.ino` file in Arduino IDE.
3. In Arduino IDE, configure the following:
   - Select `Board`: `Tools` → `Board` → `Raspberry Pi Pico`
   - Select `Port`: `Tools` → `Port` → (Select the Pico's port)
   - Select `USB Stack`: `Tools` → `USB Stack` → `Adafruit TinyUSB`
4. Upload the sketch to the Pico: Click the upload button (right arrow icon).


## 🚀 Usage

1. Launch Pokémon Legends: Z-A and fast travel to the location where you want to reroll spawns. Do not move the controller.
2. Navigate to the Switch home screen → Controllers → Change Grip/Order
3. Plug in the Pico to the switch
4. Wait for auto-registration and check for a USB Pro Controller connection for Player 1.
5. Once registered, the macros will exit out of the controller menu and launch the game automatically.
6. The macro will then start performing fast travel actions at the selected location.

> Ensure that the correct `.ino` file is uploaded for the desired fast travel point

**To stop:** Disconnect Pico or press HOME on a real controller.

## ⚙️ Macro Customization

This script is "blind" and relies entirely on timing.

- **Adjust Timing**: If the macro runs too fast (e.g., presses 'A' before the map is open), increase the `delay()` values in the `loop()` function.
- **Change Actions**: You can edit the `loop()` to do anything you want. Use the helper functions:
   - `pressButton(BTN_A, 100)`;
      - (Buttons: `BTN_A`, `BTN_B`, `BTN_X`, `BTN_Y`, `BTN_L`, `BTN_R`, `BTN_ZL`, `BTN_ZR`, `BTN_PLUS`, `BTN_MINUS`, `BTN_HOME`)
   - `pressDPad(DPAD_UP, 100)`;
      - (Directions: `DPAD_UP`,  `DPAD_DOWN`, `DPAD_LEFT`, `DPAD_RIGHT`, etc.)
   - `moveStick(STICK_RIGHT, STICK_CTR, 40)`;
      - (Moves the Left Stick. (X, Y, duration))


## 🔧 Troubleshooting

- **Q: My computer won't detect my Pico after uploading!**

  - A: This is normal! The script tells your PC it's a controller, not a Pico. To re-upload, you must force it into `BOOTSEL` mode:

   1. Unplug the Pico.
   2. Press and HOLD the BOOTSEL button on the Pico.
   3. While holding it, plug the Pico back into your computer.
   4. Release the button. It will appear as a drive. You can now upload again.

- **Q: My controller isn't recognized by the Switch.**

   - A: Go to `Tools → USB Stack` and make sure it is set to `Adafruit TinyUSB`.

- **Q: The macro gets stuck or misses buttons.**

  - A: Your Switch is loading slower than the script expects. Increase the `delay()` values in the `loop()` or `setup()` function.

## ⚠️ Disclaimer

**Educational purposes only.** Automation may violate Nintendo's Terms of Service. Use at your own risk. Not responsible for account bans, save data loss, or hardware damage.

---

**Author:** Abhilesh ([@abhilesh](https://github.com/abhilesh))
