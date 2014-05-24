// materials.h
/*
    David Shrock
    CS447 - PSU
    2004.07.26

    materials constansts

    I used Photoshop to pick from color cube and then tested with trial
    and error.
*/
#ifndef _A_MATERIALS_CONSTS_H_
#define _A_MATERIALS_CONSTS_H_

// **********************************************
// metals
// **********
// shiny gold - polished brass/gold
const float SHINY_GOLDAmbient[] = { 0.10f, 0.09f, 0.08f, 1.00f };
const float SHINY_GOLDDiff[] =    { 0.78f, 0.62f, 0.50f, 1.00f };
const float SHINY_GOLDSpec[] =    { 0.80f, 0.80f, 0.80f, 1.00f };
const float SHINY_GOLDShiny = 12.0f;               
const float SHINY_GOLDColor[]   = { 0.72f, 0.60f, 0.52f, 1.00f };

// ************************************************
// stones
// ***********
// stone for the walls
const float STONE_WALLAmbient[] = { 0.16f, 0.16f, 0.16f, 1.00f };
const float STONE_WALLDiff[]    = { 0.88f, 0.88f, 0.88f, 1.00f };
const float STONE_WALLSpec[]    = { 0.22f, 0.20f, 0.20f, 1.00f };
const float STONE_WALLShiny     = 80.0f;

// ***************************************************
// other colors
// *************
// red
const float REDAmbient[] = { 0.15f, 0.06f, 0.08f, 1.00f };
const float REDDiff[] =    { 0.88f, 0.22f, 0.30f, 1.00f };
const float REDSpec[] =    { 0.60f, 0.60f, 0.60f, 1.00f };
const float REDShiny = 30.0f;
const float REDColor[]   = { 0.92f, 0.40f, 0.42f, 1.00f };

// white
const float WHITEAmbient[] = { 0.12f, 0.12f, 0.12f, 1.00f };
const float WHITEDiff[] =    { 0.88f, 0.88f, 0.88f, 1.00f };
const float WHITESpec[] =    { 0.20f, 0.20f, 0.20f, 1.00f };
const float WHITEShiny = 80.0f;
const float WHITEColor[] =   { 1.0f, 1.0f, 1.0f, 1.0f };

#endif
