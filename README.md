# ColorHoster

An OpenRGB-compatible, high-performance SDK server for VIA per-key RGB firmware flashing and animation. This repository is used to store ColorHoster VIA-Compatible JSON files for various QMK Compatible Keyboards, as well as provide instructions for building and compiling your own ColorHoster-capable QMK firmware.

---

✨ **Features**

- 🎮 **OpenRGB Protocol v3 Support**  
  Full compatibility with OpenRGB clients  
- 🔋 **Optimized Updates**  
  Only sends changed LED data in minimal chunks  
- 🎨 **Flexible Brightness Control**  
  Choose between hue/saturation-only or full RGB + brightness  
- ⌨️ **Multi-Keyboard Support**  
  Manage multiple connected devices simultaneously  
- 🦀 **Rust-Powered**  
  Async I/O and thread-safe concurrency  
- 🌐 **Cross-Platform**  
  Windows / macOS / Linux support (x86 & ARM)  
- 💾 **Profile Management**  
  Save/load lighting configurations  

---

## Repository Structure

This repository is used for storing **pre-built VIA JSON files** with ColorHoster direct mode included. Each vendor folder contains:

1. A link to the manufacturer’s QMK firmware GitHub repository
2. Basic QMK instructions for pulling down firmware
---

## Getting Started

### Cloning the ColorHoster-JSON Repo
You can clone this project by either downloading the zip file:
![image](https://github.com/user-attachments/assets/87549ffb-6b6b-4274-8883-cba9a68b7e5e)

Or use `git` to clone the repository:\
`git clone https://github.com/JAO1988/ColorHoster-JSON.git`

Inside you’ll find two top-level folders:

**direct-mode/ — custom ColorHoster animation files**\
**keyboard/ — vendor-specific JSONs**


### Building your ColorHoster QMK Firmware
1. Patch your QMK Keymap:\
Locate your keyboard under the `/keyboards/` folder within the QMK project files.
Create a new keymap folder (-km) in your QMK tree, for example:\
`qmk_firmware/keyboards/keychron/k2_he/ansi/keymaps/viach`

2. Copy both files from direct-mode/ into your keyboard's keymap folder:\
`rgb_matrix_user.inc` & `animations/direct.h` (Folder structure must be included)\
![image](https://github.com/user-attachments/assets/d31ca1b7-bf4a-43f1-97a6-f2c7bf5393cc)

4. Enable Custom Animations:\
Open your `rules.mk` within the newly created keymap folder and add:\
`RGB_MATRIX_CUSTOM_USER = yes` to the bottom of your `rules.mk`.\
![image](https://github.com/user-attachments/assets/676301d1-82f3-42e9-aa4e-844befa1ec56)\
Save and close the file.

5. Update the keyboard `keymap.c` file:\
At the bottom of your `keymap.c` file, add the following lines for ColorHoster direct mode control:
```
#uint8_t color_buffer[RGB_MATRIX_LED_COUNT * 2] = {0};
#uint8_t brightness_buffer[RGB_MATRIX_LED_COUNT] = {[0 ... RGB_MATRIX_LED_COUNT - 1] = 255};

#ifdef VIA_ENABLE
void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    uint8_t channel_id = data[1];
    if (channel_id != id_custom_channel) return;

    uint8_t *command_id = &(data[0]);
    uint8_t value_id    = data[2];
    uint8_t led_index   = data[3];
    uint8_t led_count   = data[4];

    switch (*command_id) {
        case id_custom_set_value:
            if (value_id == 1) {
                memcpy(color_buffer + led_index * 2, data + 5, led_count * 2);
            } else if (value_id == 2) {
                memcpy(brightness_buffer + led_index, data + 5, led_count);
            }
            break;

        case id_custom_get_value:
            if (value_id == 1) {
                memcpy(data + 5, color_buffer + led_index * 2, led_count * 2);
            } else if (value_id == 2) {
                memcpy(data + 5, brightness_buffer + led_index, led_count);
            }
            break;

        case id_custom_save:
            // optional: implement persistent save here
            break;

        default:
            *command_id = id_unhandled;
            break;
    }
}
#endif // VIA_ENABLE
```
**Example:**
![image](https://github.com/user-attachments/assets/8f75d7cb-1e2d-484e-82eb-ac53dfdab688)

You're now ready to compile your added keyboard firmware with ColorHoster direct mode patched in.
## Flashing
With your favorite Command-Line Application of choice, begin compiling your new keymap for your keyboard.\
**Example:** ```qmk compile --clean -kb keychron/k2_he/ansi -km viach```

This will compile your QMK firmware with ColorHoster direct-mode support. You can use the `QMK Toolbox` GUI flashing tool or `qmk flash` command in substition of `qmk compile` to compile and flash your keyboard from the CLI.
## VIA JSON Configuration
To expose your Direct-mode animations in VIA, update the JSON for your keyboard:

1. Add the “Direct” Effect
Under the "Effects" array, append a new entry in the options:
```
{
  "label": "Effect",
  "type": "dropdown",
  "content": ["id_qmk_rgb_matrix_effect", 3, 2],
  "options": [
    ["All Off", 0],
    ["SOLID_COLOR", 1],
    ["BREATHING", 2],
    ["CYCLE_ALL", 3],
    ["CYCLE_LEFT_RIGHT", 4],
    ["CYCLE_UP_DOWN", 5],
    ["RAINBOW_MOVING_CHEVRON", 6],
    ["CYCLE_OUT_IN", 7],
    ["CYCLE_OUT_IN_DUAL", 8],
    ["CYCLE_PINWHEEL", 9],
    ["CYCKE_SPIRAL", 10],
    ["DUAL_BEACON", 11],
    ["RAINBOW_BEACON", 12],
    ["RAINDROPS", 13],
    ["TYPING_HEATMAP", 14],
    ["SOLID_REACTIVE_SIMPLE", 15],
    ["SOLID_REACTIVE", 16],
    ["SOLID_REACTIVE_NEXUS", 17],
    ["MATRIX_MULTISPLASH", 18],
    ["Direct", 19]    // ← your Direct Mode ID Number (may vary)
  ]
}
```
> Note: Replace 19 with your keyboard’s next available effect ID.

2. Add Direct-Mode Parameters:
In the same JSON file, add these under the parameters section:
```
// Effect Speed (for all except Off & Direct)
{
  "showIf": "{id_qmk_rgb_matrix_effect} > 1 && {id_qmk_rgb_matrix_effect} != 19",
  "label": "Effect Speed",
  "type": "range",
  "options": [0, 255],
  "content": ["id_qmk_rgb_matrix_effect_speed", 3, 3]
},
// Color Picker (for select effects)
{
  "showIf": "{id_qmk_rgb_matrix_effect} != 0 && {id_qmk_rgb_matrix_effect} != 19 && ( {id_qmk_rgb_matrix_effect} < 4 || {id_qmk_rgb_matrix_effect} == 18 || ({id_qmk_rgb_matrix_effect} > 17 && {id_qmk_rgb_matrix_effect} != 21) )",
  "label": "Color",
  "type": "color",
  "content": ["id_qmk_rgb_matrix_color", 3, 4]
},
// Direct-Mode Palette Selector
{
  "showIf": "{id_qmk_rgb_matrix_effect} == 19",
  "label": "Color Palette",
  "type": "color-palette",
  "content": ["id_qmk_rgb_matrix_color", 3, 4]
}
```
3. Declare LEDs in the Layout
Under the "layout" → "keymap" section, each LED is declared as:

```
"0,0\nl0",
{ "x": 0.5, "c": "#cccccc" },
"0,1\nl1",
"0,2\nl2",
...
"0,14\nl13"
],
[
  { "y": 0.25 },
  "1,0\nl14",
  "1,1\nl15",
  ...
  "1,13\nl27",
  ...
]
```
- The string "row,col\nl*" binds LED index * at grid position (row,col).
- Begin at "0,0\nl0" and increment the LED index for each matrix position.

## Troubleshooting
If your VIA JSON fails to load or LEDs appear in the wrong spots:
- **Validate JSON syntax**  
  • Inspect your modified VIA JSON file ensure missing commas, unquoted keys, or stray braces are corrected.\
  • You can use an online JSON inspect tool like *JSON Lint* https://jsonlint.com to highlight subtle formatting errors.\
- **Check `"id_qmk_rgb_matrix_effect"` indices**  
  • Ensure the `"Direct"` effect ID matches your firmware’s `RGB_MATRIX_EFFECT_COUNT` offset—off-by-one here prevents VIA from recognizing the dropdown.  
  • All content arrays (`["id_qmk_rgb_matrix_color", 3, 4]`, etc.) must reference valid VIA channel IDs and lengths.\
- **Still unsure?**\
  • Reach out to JAO1988 (Siphoned Anomaly) within the `OpenRGB` Discord Channel.\
  • Support questsions or concerns for QMK-related questions should be relayed to the `qmk-firmware-hacking` channel.
