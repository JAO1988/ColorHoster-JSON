# ColorHoster

An OpenRGB-compatible, high-performance SDK server for VIA per-key RGB firmware flashing and animation.

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

This repo stores **pre-built VIA JSON files** for various keyboard vendors.  
Each vendor folder contains:

1. A link to the manufacturer’s QMK firmware GitHub repo  
2. Basic QMK instructions for pulling down firmware

---

## Getting Started

### 1. Clone the ColorHoster-JSON Repo

`git clone https://github.com/JAO1988/ColorHoster-JSON.git`

Inside you’ll find two top-level folders:
**direct-mode/ — custom ColorHoster animation files**
**keyboard/ — vendor-specific JSONs**

1. Patch Your QMK Keymap
Locate your keyboard under `keyboard/`.
Create a new keymap folder (km) in your QMK tree, for example:
`qmk_firmware/keyboards/keychron/k2_he/ansi/keymaps/viach`

2. Copy both files from direct-mode/ into your keyboard's keymap folder:
`rgb_matrix_user.inc` & `animations/direct.h` (Folder structure must be included)

3. Enable Custom Animations
Open your `rules.mk` within the newly created keymap folder and add:
`RGB_MATRIX_CUSTOM_USER = yes`
To the bottom of your `rules.mk`. Save and close the file.

4. Update the keyboard `keymap.c` file
At the bottom of your `keymap.c` file, add the following lines for ColorHoster direct mode control:

uint8_t color_buffer[RGB_MATRIX_LED_COUNT * 2] = {0};
uint8_t brightness_buffer[RGB_MATRIX_LED_COUNT] = {[0 ... RGB_MATRIX_LED_COUNT - 1] = 255};

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

6. 
