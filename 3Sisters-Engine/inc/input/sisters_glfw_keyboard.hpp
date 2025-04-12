#pragma once

#ifndef SISTERS_GLFW_KEYBOARD_HPP
#define SISTERS_GLFW_KEYBOARD_HPP

// include boolean
#include <stdbool.h>

/* Host several macros and functions to allow for keyboard inputs
*/
namespace GLFW{
    
//? Engine renamed GLFW US key codes, refer to https://www.glfw.org/docs/3.3/group__keys.html 

#define SISTER_KEY_SPACE 32
#define SISTER_KEY_APOSTROPHE 39 /* ' */
#define SISTER_KEY_COMMA 44 /* , */
#define SISTER_KEY_MINUS 45 /* - */
#define SISTER_KEY_PERIOD 46 /* . */
#define SISTER_KEY_SLASH 47 /* / */
#define SISTER_KEY_0 48
#define SISTER_KEY_1 49
#define SISTER_KEY_2 50
#define SISTER_KEY_3 51
#define SISTER_KEY_4 52
#define SISTER_KEY_5 53
#define SISTER_KEY_6 54
#define SISTER_KEY_7 55
#define SISTER_KEY_8 56
#define SISTER_KEY_9 57
#define SISTER_KEY_SEMICOLON 59 /* ; */
#define SISTER_KEY_EQUAL 61 /* = */
#define SISTER_KEY_A 65
#define SISTER_KEY_B 66
#define SISTER_KEY_C 67
#define SISTER_KEY_D 68
#define SISTER_KEY_E 69
#define SISTER_KEY_F 70
#define SISTER_KEY_G 71
#define SISTER_KEY_H 72
#define SISTER_KEY_I 73
#define SISTER_KEY_J 74
#define SISTER_KEY_K 75
#define SISTER_KEY_L 76
#define SISTER_KEY_M 77
#define SISTER_KEY_N 78
#define SISTER_KEY_O 79
#define SISTER_KEY_P 80
#define SISTER_KEY_Q 81
#define SISTER_KEY_R 82
#define SISTER_KEY_S 83
#define SISTER_KEY_T 84
#define SISTER_KEY_U 85
#define SISTER_KEY_V 86
#define SISTER_KEY_W 87
#define SISTER_KEY_X 88
#define SISTER_KEY_Y 89
#define SISTER_KEY_Z 90
#define SISTER_KEY_LEFT_BRACKET 91 /* [ */
#define SISTER_KEY_BACKSLASH 92 /* \ */
#define SISTER_KEY_RIGHT_BRACKET 93 /* ] */
#define SISTER_KEY_GRAVE_ACCENT 96 /* ` */
#define SISTER_KEY_WORLD_1 161 /* non-US #1 */
#define SISTER_KEY_WORLD_2 162 /* non-US #2 */
#define SISTER_KEY_ESCAPE 256
#define SISTER_KEY_ENTER 257
#define SISTER_KEY_TAB 258
#define SISTER_KEY_BACKSPACE 259
#define SISTER_KEY_INSERT 260
#define SISTER_KEY_DELETE 261
#define SISTER_KEY_RIGHT 262
#define SISTER_KEY_LEFT 263
#define SISTER_KEY_DOWN 264
#define SISTER_KEY_UP 265
#define SISTER_KEY_PAGE_UP 266
#define SISTER_KEY_PAGE_DOWN 267
#define SISTER_KEY_HOME 268
#define SISTER_KEY_END 269
#define SISTER_KEY_CAPS_LOCK 280
#define SISTER_KEY_SCROLL_LOCK 281
#define SISTER_KEY_NUM_LOCK 282
#define SISTER_KEY_PRINT_SCREEN 283
#define SISTER_KEY_PAUSE 284
#define SISTER_KEY_F1 290
#define SISTER_KEY_F2 291
#define SISTER_KEY_F3 292
#define SISTER_KEY_F4 293
#define SISTER_KEY_F5 294
#define SISTER_KEY_F6 295
#define SISTER_KEY_F7 296
#define SISTER_KEY_F8 297
#define SISTER_KEY_F9 298
#define SISTER_KEY_F10 299
#define SISTER_KEY_F11 300
#define SISTER_KEY_F12 301
#define SISTER_KEY_F13 302
#define SISTER_KEY_F14 303
#define SISTER_KEY_F15 304
#define SISTER_KEY_F16 305
#define SISTER_KEY_F17 306
#define SISTER_KEY_F18 307
#define SISTER_KEY_F19 308
#define SISTER_KEY_F20 309
#define SISTER_KEY_F21 310
#define SISTER_KEY_F22 311
#define SISTER_KEY_F23 312
#define SISTER_KEY_F24 313
#define SISTER_KEY_F25 314
#define SISTER_KEY_KP_0 320
#define SISTER_KEY_KP_1 321
#define SISTER_KEY_KP_2 322
#define SISTER_KEY_KP_3 323
#define SISTER_KEY_KP_4 324
#define SISTER_KEY_KP_5 325
#define SISTER_KEY_KP_6 326
#define SISTER_KEY_KP_7 327
#define SISTER_KEY_KP_8 328
#define SISTER_KEY_KP_9 329
#define SISTER_KEY_KP_DECIMAL 330
#define SISTER_KEY_KP_DIVIDE 331
#define SISTER_KEY_KP_MULTIPLY 332
#define SISTER_KEY_KP_SUBTRACT 333
#define SISTER_KEY_KP_ADD 334
#define SISTER_KEY_KP_ENTER 335
#define SISTER_KEY_KP_EQUAL 336
#define SISTER_KEY_LEFT_SHIFT 340
#define SISTER_KEY_LEFT_CONTROL 341
#define SISTER_KEY_LEFT_ALT 342
#define SISTER_KEY_LEFT_SUPER 343
#define SISTER_KEY_RIGHT_SHIFT 344
#define SISTER_KEY_RIGHT_CONTROL 345
#define SISTER_KEY_RIGHT_ALT 346
#define SISTER_KEY_RIGHT_SUPER 347
#define SISTER_KEY_MENU 348
#define SISTER_KEY_LAST SISTER_KEY_MENU

// define keyboard structure
extern bool g_KeyboardState[SISTER_KEY_LAST];

/* key callback of the keyboard which checks for the given key
* @NOTE: Checks if key was pressed
*/
bool getKeyInput(int key);

/* key callback of the keyboard which checks for the given key
* @NOTE: Checks if key was pressed, similar to getKeyInput
*/
bool getKeyInputDown(int key);

/* key callback of the keyboard which checks for the given key
* @NOTE: Checks if key was released
*/
bool getKeyInputUp(int key);

}
#endif