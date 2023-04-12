#include QMK_KEYBOARD_H

    
enum layers {
    _BASE,
    _BASE_TARKOV,
    _SPEC, /* Special layer with F1-F24 keys, media keys, etc */
    _RGB,
};


enum tap_dances {
    TD_SPECIAL_KEY,
};


enum custom_keycodes {
    C_AND_F13 = SAFE_RANGE,
    V_AND_F14,
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case C_AND_F13:
            if (record->event.pressed) {
                register_code(KC_C);
                _delay_ms(10);
                register_code(KC_F13);
            } else {
                unregister_code(KC_F13);
                _delay_ms(15);
                unregister_code(KC_C);
            }
            break;
        case V_AND_F14:
            if (record->event.pressed) {
                register_code(KC_V);
                _delay_ms(11);
                register_code(KC_F14);
            } else {
                unregister_code(KC_F14);
                _delay_ms(14);
                unregister_code(KC_V);
            }
            break;
    }
    return true;
}


void SPECIAL_KEY(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 2:  // Capitalizes the next word only
            caps_word_on();
            break;
        case 3:
            if (layer_state_is(_BASE_TARKOV)) {
                layer_off(_BASE_TARKOV);
            } else {
                layer_on(_BASE_TARKOV);
            }
            break;
        case 5:  // Toggle RGB layer
            if (layer_state_is(_RGB)) {
                layer_off(_RGB);
            } else {
                layer_on(_RGB);
            }
            break;
        case 10:  // Reset the keyboard so it can be flashed
            reset_keyboard();
    }
};


// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_SPECIAL_KEY] = ACTION_TAP_DANCE_FN(SPECIAL_KEY),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Qwerty
    * ,------------------------------------------------------------------------------------------.
    * | ` ~  |  1! |  2@ |  3# |  4$ |  5% |  6^ |  7& |  8* |  9( |  0) |  -  |  =  |   Bkspc   |
    * |------------------------------------------------------------------------------------------+
    * | Tab    |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |     \   |
    * |------------------------------------------------------------------------------------------+
    * | MO(_SPEC)|  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter    |
    * |------------------------------------------------------------------------------------------+
    * | Shift      |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |TD(CWORD|RGB|RES)|
    * |------------------------------------------------------------------------------------------+
    * | LCTL  |  Cmd  |  Alt  |              Space                | Left | Down |  Up  |  Right  |
    * `------------------------------------------------------------------------------------------'
    */
    [_BASE] = LAYOUT_60_ansi(
       KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,     KC_5,     KC_6,    KC_7,    KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,     KC_T,     KC_Y,    KC_U,    KC_I,    KC_O,     KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
       MO(_SPEC), KC_A,    KC_S,    KC_D,    KC_F,     KC_G,     KC_H,    KC_J,    KC_K,    KC_L,     KC_SCLN, KC_QUOT,          KC_ENT,
       KC_LSFT,            KC_Z,    KC_X,    KC_C,     KC_V,     KC_B,    KC_N,    KC_M,    KC_COMM,  KC_DOT,  KC_SLSH,          TD(TD_SPECIAL_KEY),
       KC_LCTL,   KC_LGUI, KC_LALT,                              KC_SPC,                              KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),
    /* Qwerty but C also presses Q and V also presses E */
    [_BASE_TARKOV] = LAYOUT_60_ansi(
       KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,     KC_5,     KC_6,    KC_7,    KC_8,    KC_9,     KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,     KC_T,     KC_Y,    KC_U,    KC_I,    KC_O,     KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
       MO(_SPEC), KC_A,    KC_S,    KC_D,    KC_F,     KC_G,     KC_H,    KC_J,    KC_K,    KC_L,     KC_SCLN, KC_QUOT,          KC_ENT,
       KC_LSFT,            KC_Z,    KC_X,    C_AND_F13,V_AND_F14,KC_B,    KC_N,    KC_M,    KC_COMM,  KC_DOT,  KC_SLSH,          TD(TD_SPECIAL_KEY),
       KC_LCTL,   KC_LGUI, KC_LALT,                              KC_SPC,                              KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),
    /* Special
    * ,------------------------------------------------------------------------------------------.
    * | ESC  |  F1 |  F2 |  F3 |  F4 |  F5 |  F6 |  F7 |  F8 |  F9 | F10 | F11 | F12 |  DELETE   |
    * |------------------------------------------------------------------------------------------+
    * | NumLock| F13 |     | F14 | F15 |     |     |     | Ins |     |PrtSc|     |     |         |
    * |------------------------------------------------------------------------------------------+
    * |          |     |     |     | F16 |W_OFF|     |     |KLock| ScL |     |     |             |
    * |------------------------------------------------------------------------------------------+
    * |            | F17 | F18 | Calc|     |     | N_K | Mute|     |     |     |                 |
    * |------------------------------------------------------------------------------------------+
    * |       |  W_ON |       |            Play/Pause             | HOME | PG_DN | PG_UP |  END  |
    * `------------------------------------------------------------------------------------------'
    */
    [_SPEC] = LAYOUT_60_ansi(
        KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,    KC_F5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  KC_F11,  KC_F12,  KC_DEL,
        KC_NUM,   KC_F13,  _______, KC_F14,  KC_F15,   _______,  _______, _______, KC_INS,  _______,  KC_PSCR, _______, _______, _______,
        _______,  _______, _______, _______, KC_F16,   GUI_OFF,  _______, _______, QK_LOCK, KC_SCRL,  _______, _______,          _______,
        _______,           KC_F17,  KC_F18,  KC_CALC,  _______,  _______, NK_TOGG, KC_MUTE, _______,  _______, _______,          _______,
        _______,  GUI_ON,  _______,                              KC_MPLY,                             KC_HOME, KC_PGDN, KC_PGUP, KC_END
    ),
    /* RGB
    * ,------------------------------------------------------------------------------------------.
    * |      |     |     |     |     |     |     |     |     |     |     | BR- | BR+ |           |
    * |------------------------------------------------------------------------------------------+
    * |        |     | SP+ |     |     | TOG |     |     |     |     |     |     |     |         |
    * |------------------------------------------------------------------------------------------+
    * |          |     | SP- |     |     |     |     |     |     |     |     |     |             |
    * |------------------------------------------------------------------------------------------+
    * |            |     |     |     |     |     |     |     |     |     |     |                 |
    * |------------------------------------------------------------------------------------------+
    * |       |       |       |                                   | HUE- |  SAT- |  SAT+ |  HUE+ |
    * `------------------------------------------------------------------------------------------'
    */
    [_RGB] = LAYOUT_60_ansi(
        XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, RGB_VAD, RGB_VAI, XXXXXXX,
        XXXXXXX,  XXXXXXX, RGB_SPI, XXXXXXX, XXXXXXX,  RGB_TOG,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        _______,  XXXXXXX, RGB_SPD, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,          XXXXXXX, 
        XXXXXXX,           XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, RGB_RMOD, RGB_MOD, XXXXXXX,          _______,
        XXXXXXX,  XXXXXXX, XXXXXXX,                              XXXXXXX,                             RGB_HUD, RGB_SAD, RGB_SAI, RGB_HUI
    )

    // TEMPLATE
    // LAYOUT_60_ansi(
    //    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    //    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    //    _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
    //    _______, _______, _______,                            _______,                            _______, _______, _______, _______,
    // ),
};


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (get_highest_layer(layer_state) == _SPEC) {
        uint8_t layer = get_highest_layer(layer_state);
        RGB rgb = hsv_to_rgb(rgb_matrix_get_hsv());

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED) {
                    if (keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
                        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                    }

                    if (keymap_key_to_keycode(layer, (keypos_t){col,row}) == KC_TRNS) {
                        rgb_matrix_set_color(index, 0, 0, 0);
                    }
                }
            }
        }
    }
    return false;
}
