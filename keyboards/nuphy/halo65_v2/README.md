If you want to get side LEDs to work you will need to complete some extra steps:

1. Change your rgb_matrix_user.inc to the one provided in this folder
2. Open up your side.c file located at the root of the keyboard folder and add an if statement to the case break lines:

<pre> 
if(rgblight_get_mode() != 43) {
    switch (side_mode_a) {
        case SIDE_WAVE:     side_wave_mode_show();      break;
        case SIDE_NEW:      side_new_mode_show();       break;
        case SIDE_MIX:      side_spectrum_mode_show();  break;
        case SIDE_BREATH:   side_breathe_mode_show();   break;
        case SIDE_STATIC:   side_static_mode_show();    break;
    }
}
</pre>

3. Enjoy! that should be it
