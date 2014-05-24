// consts.h
/*
   David Shrock
   CS447 - PSU
   2004.07.17

   some constants and macros

   materials constants are found in materials.h
*/
#ifndef _A_CONSTS_H_
#define _A_CONSTS_H_

// Win32 keyboard input using GetAsyncKeyState explained by Hawkins, Astle p482.
#define KEY_DOWN(vk_code)  ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)    ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

const int    SCREEN_WD      = 1024;
const int    SCREEN_HT      = 768;

// pi: as defined in the Cambridge Handbook of Physics Formulas, p18
const double PI        = 3.14159265358979323846264338327950288419716939937510;

// some popular angles to evaluate now for speed
const double TWO_PI    = PI * 2; 
const double PI_OVER_2 = PI * 1/2;
const double PI_OVER_4 = PI * 1/4;       
const double PI_OVER_8 = PI * 1/8;  
       
// movement rates
const float SPEED = 1.76f;                           // movement rate
const float ANGLE_FACTOR =(float) PI * 0.24f;       // rate of turning
#endif
