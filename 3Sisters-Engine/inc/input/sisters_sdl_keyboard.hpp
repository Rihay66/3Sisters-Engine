#pragma once

#ifndef SISTERS_SDL_KEYBOARD_HPP
#define SISTERS_SDL_KEYBOARD_HPP

#include <stdint.h>

/* Host several macros and functions to allow for keyboard inputs
*/
namespace SDL {

//? Engine renamed keyboard scancodes from SDL refer to https://wiki.libsdl.org/SDL2/SDL_Scancode

#define SISTER_KEY_UNKNOWN 0
#define SISTER_KEY_A 4
#define SISTER_KEY_B 5
#define SISTER_KEY_C 6
#define SISTER_KEY_D 7
#define SISTER_KEY_E 8
#define SISTER_KEY_F 9
#define SISTER_KEY_G 10
#define SISTER_KEY_H 11
#define SISTER_KEY_I 12
#define SISTER_KEY_J 13
#define SISTER_KEY_K 14
#define SISTER_KEY_L 15
#define SISTER_KEY_M 16
#define SISTER_KEY_N 17
#define SISTER_KEY_O 18
#define SISTER_KEY_P 19
#define SISTER_KEY_Q 20
#define SISTER_KEY_R 21
#define SISTER_KEY_S 22
#define SISTER_KEY_T 23
#define SISTER_KEY_U 24
#define SISTER_KEY_V 25
#define SISTER_KEY_W 26
#define SISTER_KEY_X 27
#define SISTER_KEY_Y 28
#define SISTER_KEY_Z 29
#define SISTER_KEY_1 30
#define SISTER_KEY_2 31
#define SISTER_KEY_3 32
#define SISTER_KEY_4 33
#define SISTER_KEY_5 34
#define SISTER_KEY_6 35
#define SISTER_KEY_7 36
#define SISTER_KEY_8 37
#define SISTER_KEY_9 38
#define SISTER_KEY_0 39
#define SISTER_KEY_RETURN 40
#define SISTER_KEY_ESCAPE 41
#define SISTER_KEY_BACKSPACE 42
#define SISTER_KEY_TAB 43
#define SISTER_KEY_SPACE 44
#define SISTER_KEY_MINUS 45
#define SISTER_KEY_EQUALS 46
#define SISTER_KEY_LEFTBRACKET 47
#define SISTER_KEY_RIGHTBRACKET 48
/**< Located at the lower left of the return
 *   key on ISO keyboards and at the right end
 *   of the QWERTY row on ANSI keyboards.
 *   Produces REVERSE SOLIDUS (backslash) and
 *   VERTICAL LINE in a US layout REVERSE
 *   SOLIDUS and VERTICAL LINE in a UK Mac
 *   layout NUMBER SIGN and TILDE in a UK
 *   Windows layout DOLLAR SIGN and POUND SIGN
 *   in a Swiss German layout NUMBER SIGN and
 *   APOSTROPHE in a German layout GRAVE
 *   ACCENT and POUND SIGN in a French Mac
 *   layout and ASTERISK and MICRO SIGN in a
 *   French Windows layout.
 */
#define SISTER_KEY_BACKSLASH 49
#define SISTER_KEY_NONUSHASH 50
/**< ISO USB keyboards actually use this code                           \
 *   instead of 49 for the same key but all                             \
 *   OSes I've seen treat the two codes                                 \
 *   identically. So as an implementor unless                           \
 *   your keyboard generates both of those                              \
 *   codes and your OS treats them differently                          \
 *   you should generate #define SISTER_KEY_BACKSLASH                   \
 *   instead of this code. As a user you                                \
 *   should not rely on this code because #define SISTER                \
 *   will never generate it with most (all?)                            \
 *   keyboards.                                                         \
 */
#define SISTER_KEY_SEMICOLON 51
#define SISTER_KEY_APOSTROPHE 52
#define SISTER_KEY_GRAVE 53
/**< Located in the top left corner (on both ANSI                       \
 *   and ISO keyboards). Produces GRAVE ACCENT and                      \
 *   TILDE in a US Windows layout and in US and UK                      \
 *   Mac layouts on ANSI keyboards GRAVE ACCENT                         \
 *   and NOT SIGN in a UK Windows layout SECTION                        \
 *   SIGN and PLUS-MINUS SIGN in US and UK Mac                          \
 *   layouts on ISO keyboards SECTION SIGN and                          \
 *   DEGREE SIGN in a Swiss German layout (Mac:                         \
 *   only on ISO keyboards) CIRCUMFLEX ACCENT and                       \
 *   DEGREE SIGN in a German layout (Mac: only on                       \
 *   ISO keyboards) SUPERSCRIPT TWO and TILDE in a                      \
 *   French Windows layout COMMERCIAL AT and                            \
 *   NUMBER SIGN in a French Mac layout on ISO                          \
 *   keyboards and LESS-THAN SIGN and GREATER-THAN                      \
 *   SIGN in a Swiss German German or French Mac                        \
 *   layout on ANSI keyboards.                                          \
 */
#define SISTER_KEY_COMMA 54
#define SISTER_KEY_PERIOD 55
#define SISTER_KEY_SLASH 56
#define SISTER_KEY_CAPSLOCK 57
#define SISTER_KEY_F1 58
#define SISTER_KEY_F2 59
#define SISTER_KEY_F3 60
#define SISTER_KEY_F4 61
#define SISTER_KEY_F5 62
#define SISTER_KEY_F6 63
#define SISTER_KEY_F7 64
#define SISTER_KEY_F8 65
#define SISTER_KEY_F9 66
#define SISTER_KEY_F10 67
#define SISTER_KEY_F11 68
#define SISTER_KEY_F12 69
#define SISTER_KEY_PRINTSCREEN 70
#define SISTER_KEY_SCROLLLOCK 71
#define SISTER_KEY_PAUSE 72
/**< insert on PC help on some Mac keyboards (but                       \
  does send code 73 not 117) */
#define SISTER_KEY_INSERT 73 
#define SISTER_KEY_HOME 74
#define SISTER_KEY_PAGEUP 75
#define SISTER_KEY_DELETE 76
#define SISTER_KEY_END 77
#define SISTER_KEY_PAGEDOWN 78
#define SISTER_KEY_RIGHT 79
#define SISTER_KEY_LEFT 80
#define SISTER_KEY_DOWN 81
#define SISTER_KEY_UP 82
/**< num lock on PC clear on Mac keyboards                              \
 */
#define SISTER_KEY_NUMLOCKCLEAR 83
#define SISTER_KEY_KP_DIVIDE 84
#define SISTER_KEY_KP_MULTIPLY 85
#define SISTER_KEY_KP_MINUS 86
#define SISTER_KEY_KP_PLUS 87
#define SISTER_KEY_KP_ENTER 88
#define SISTER_KEY_KP_1 89
#define SISTER_KEY_KP_2 90
#define SISTER_KEY_KP_3 91
#define SISTER_KEY_KP_4 92
#define SISTER_KEY_KP_5 93
#define SISTER_KEY_KP_6 94
#define SISTER_KEY_KP_7 95
#define SISTER_KEY_KP_8 96
#define SISTER_KEY_KP_9 97
#define SISTER_KEY_KP_0 98
#define SISTER_KEY_KP_PERIOD 99
/**< This is the additional key that ISO                               \
 *   keyboards have over ANSI ones                                     \
 *   located between left shift and Y.                                 \
 *   Produces GRAVE ACCENT and TILDE in a                              \
 *   US or UK Mac layout REVERSE SOLIDUS                               \
 *   (backslash) and VERTICAL LINE in a                                \
 *   US or UK Windows layout and                                       \
 *   LESS-THAN SIGN and GREATER-THAN SIGN                              \
 *   in a Swiss German German or French                                \
 *   layout. */
#define SISTER_KEY_APPLICATION 101 
/**< windows contextual menu compose */
#define SISTER_KEY_NONUSBACKSLASH 100
/**< The USB document says this is a status flag                       \
 *   not a physical key - but some Mac keyboards                       \
 *   do have a power key. */
#define SISTER_KEY_POWER 102
#define SISTER_KEY_KP_EQUALS 103
#define SISTER_KEY_F13 104
#define SISTER_KEY_F14 105
#define SISTER_KEY_F15 106
#define SISTER_KEY_F16 107
#define SISTER_KEY_F17 108
#define SISTER_KEY_F18 109
#define SISTER_KEY_F19 110
#define SISTER_KEY_F20 111
#define SISTER_KEY_F21 112
#define SISTER_KEY_F22 113
#define SISTER_KEY_F23 114
#define SISTER_KEY_F24 115
#define SISTER_KEY_EXECUTE 116
#define SISTER_KEY_HELP 117 /**< AL Integrated Help Center */
#define SISTER_KEY_MENU 118 /**< Menu (show menu) */
#define SISTER_KEY_SELECT 119
#define SISTER_KEY_STOP 120  /**< AC Stop */
#define SISTER_KEY_AGAIN 121 /**< AC Redo/Repeat */
#define SISTER_KEY_UNDO 122  /**< AC Undo */
#define SISTER_KEY_CUT 123   /**< AC Cut */
#define SISTER_KEY_COPY 124  /**< AC Copy */
#define SISTER_KEY_PASTE 125 /**< AC Paste */
#define SISTER_KEY_FIND 126  /**< AC Find */
#define SISTER_KEY_MUTE 127
#define SISTER_KEY_VOLUMEUP 128
#define SISTER_KEY_VOLUMEDOWN 129
#define SISTER_KEY_KP_COMMA 133
#define SISTER_KEY_KP_EQUALSAS400 134
#define SISTER_KEY_INTERNATIONAL1  135 /**< used on Asian keyboards see
                                        footnotes in USB doc */
#define SISTER_KEY_INTERNATIONAL2  136
#define SISTER_KEY_INTERNATIONAL3  137 /**< Yen */
#define SISTER_KEY_INTERNATIONAL4  138
#define SISTER_KEY_INTERNATIONAL5  139
#define SISTER_KEY_INTERNATIONAL6  140
#define SISTER_KEY_INTERNATIONAL7  141
#define SISTER_KEY_INTERNATIONAL8  142
#define SISTER_KEY_INTERNATIONAL9  143
#define SISTER_KEY_LANG1  144 /**< Hangul/English toggle */
#define SISTER_KEY_LANG2  145 /**< Hanja conversion */
#define SISTER_KEY_LANG3  146 /**< Katakana */
#define SISTER_KEY_LANG4  147 /**< Hiragana */
#define SISTER_KEY_LANG5  148 /**< Zenkaku/Hankaku */
#define SISTER_KEY_LANG6  149 /**< reserved */
#define SISTER_KEY_LANG7  150 /**< reserved */
#define SISTER_KEY_LANG8  151 /**< reserved */
#define SISTER_KEY_LANG9  152 /**< reserved */
#define SISTER_KEY_ALTERASE  153    /**< Erase-Eaze */
#define SISTER_KEY_SYSREQ  154
#define SISTER_KEY_CANCEL  155      /**< AC Cancel */
#define SISTER_KEY_CLEAR  156
#define SISTER_KEY_PRIOR  157
#define SISTER_KEY_RETURN2  158
#define SISTER_KEY_SEPARATOR  159
#define SISTER_KEY_OUT  160
#define SISTER_KEY_OPER  161
#define SISTER_KEY_CLEARAGAIN  162
#define SISTER_KEY_CRSEL  163
#define SISTER_KEY_EXSEL  164
#define SISTER_KEY_KP_00  176
#define SISTER_KEY_KP_000  177
#define SISTER_KEY_THOUSANDSSEPARATOR  178
#define SISTER_KEY_DECIMALSEPARATOR  179
#define SISTER_KEY_CURRENCYUNIT  180
#define SISTER_KEY_CURRENCYSUBUNIT  181
#define SISTER_KEY_KP_LEFTPAREN  182
#define SISTER_KEY_KP_RIGHTPAREN  183
#define SISTER_KEY_KP_LEFTBRACE  184
#define SISTER_KEY_KP_RIGHTBRACE  185
#define SISTER_KEY_KP_TAB  186
#define SISTER_KEY_KP_BACKSPACE  187
#define SISTER_KEY_KP_A  188
#define SISTER_KEY_KP_B  189
#define SISTER_KEY_KP_C  190
#define SISTER_KEY_KP_D  191
#define SISTER_KEY_KP_E  192
#define SISTER_KEY_KP_F  193
#define SISTER_KEY_KP_XOR  194
#define SISTER_KEY_KP_POWER  195
#define SISTER_KEY_KP_PERCENT  196
#define SISTER_KEY_KP_LESS  197
#define SISTER_KEY_KP_GREATER  198
#define SISTER_KEY_KP_AMPERSAND  199
#define SISTER_KEY_KP_DBLAMPERSAND  200
#define SISTER_KEY_KP_VERTICALBAR  201
#define SISTER_KEY_KP_DBLVERTICALBAR  202
#define SISTER_KEY_KP_COLON  203
#define SISTER_KEY_KP_HASH  204
#define SISTER_KEY_KP_SPACE  205
#define SISTER_KEY_KP_AT  206
#define SISTER_KEY_KP_EXCLAM  207
#define SISTER_KEY_KP_MEMSTORE  208
#define SISTER_KEY_KP_MEMRECALL  209
#define SISTER_KEY_KP_MEMCLEAR  210
#define SISTER_KEY_KP_MEMADD  211
#define SISTER_KEY_KP_MEMSUBTRACT  212
#define SISTER_KEY_KP_MEMMULTIPLY  213
#define SISTER_KEY_KP_MEMDIVIDE  214
#define SISTER_KEY_KP_PLUSMINUS  215
#define SISTER_KEY_KP_CLEAR  216
#define SISTER_KEY_KP_CLEARENTRY  217
#define SISTER_KEY_KP_BINARY  218
#define SISTER_KEY_KP_OCTAL  219
#define SISTER_KEY_KP_DECIMAL  220
#define SISTER_KEY_KP_HEXADECIMAL  221
#define SISTER_KEY_LCTRL  224
#define SISTER_KEY_LSHIFT  225
#define SISTER_KEY_LALT  226 /**< alt option */
#define SISTER_KEY_LGUI  227 /**< windows command (apple) meta */
#define SISTER_KEY_RCTRL  228
#define SISTER_KEY_RSHIFT  229
#define SISTER_KEY_RALT  230 /**< alt gr option */
#define SISTER_KEY_RGUI  231 /**< windows command (apple) meta */

// define a keyboard state holder
struct KeyboardStateHolder {
  uint8_t *keyboardState;
};

// define global of the state
extern struct KeyboardStateHolder g_KeyboardState;

/* key callback of the keyboard which checks for the given KEY
 * @NOTE: Checks if key was pressed
 */
bool getKeyInput(int key);

/* key callback of the keyboard which checks for the given KEY
 * @NOTE: Checks if key was pressed similar to getKeyInput
 */
bool getKeyInputDown(int key);

/* key callback of the keyboard which checks for the given KEY
 * @NOTE: Check if key was released
 */
bool getKeyInputUp(int key);

} 
#endif