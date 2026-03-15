
RGB_MATRIX_EFFECT(game_mode)
RGB_MATRIX_EFFECT(position_mode)
RGB_MATRIX_EFFECT(direct_mode)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

extern rgb_config_t rgb_matrix_config;

static bool game_mode(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        if(i<67)
        rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
    }

    RGB rgb = hsv_to_rgb(rgb_matrix_config.hsv);
    rgb_matrix_set_color(0, rgb.r, rgb.g, rgb.b);       // ESC

    rgb_matrix_set_color(17, rgb.r, rgb.g, rgb.b);      // W
    rgb_matrix_set_color(31, rgb.r, rgb.g, rgb.b);      // D
    rgb_matrix_set_color(32, rgb.r, rgb.g, rgb.b);      // S
    rgb_matrix_set_color(33, rgb.r, rgb.g, rgb.b);      // A

    rgb_matrix_set_color(56, rgb.r, rgb.g, rgb.b);      // up
    rgb_matrix_set_color(64, rgb.r, rgb.g, rgb.b);      // right
    rgb_matrix_set_color(65, rgb.r, rgb.g, rgb.b);      // down
    rgb_matrix_set_color(66, rgb.r, rgb.g, rgb.b);      // left

    return rgb_matrix_check_finished_leds(led_max);
}

static bool position_mode(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        if(i<67)
        rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
    }

    RGB rgb = hsv_to_rgb(rgb_matrix_config.hsv);

    rgb_matrix_set_color(34, rgb.r, rgb.g, rgb.b);      // F
    rgb_matrix_set_color(37, rgb.r, rgb.g, rgb.b);      // J
    rgb_matrix_set_color(56, rgb.r, rgb.g, rgb.b);      // up

    return rgb_matrix_check_finished_leds(led_max);
}

extern uint8_t color_buffer[RGB_MATRIX_LED_COUNT * 2];
extern uint8_t brightness_buffer[RGB_MATRIX_LED_COUNT];

static bool direct_mode(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB rgb = hsv_to_rgb((HSV){color_buffer[i * 2], color_buffer[i * 2 + 1], scale8(rgb_matrix_config.hsv.v, brightness_buffer[i])});
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS

