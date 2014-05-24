// imageFile.h
/*
    David Shrock
    CS447 - PSU
    2004.07.04

    header for imageFile.cpp
	
    Image file format handler for bitmap images.
    Intended use is with 3D graphics programming such as OpenGL. Loading
    functions only use file header elements that is necessary to display
    image.

    Targa format description outlined in 'Bitmapped Graphics Programming
      In C++' by Marv Luse (1993) p657.
*/

#ifndef _IMAGE_FILE_H
#define _IMAGE_FILE_H

// ***************************************
// structures

/* 
   TargaFile - Targa image file format
   for memory saving, only using necessary header from Targa file.
*/
struct TargaFile
{
  short int     width;           // of image in pixels (lo - hi)
  short int     height;          // of image in pixels (lo - hi)
  unsigned char type;            // 2= uncompressed RGB, 3= unc grayscale
  unsigned char bitCount;        // number of bits- 16, 24, 32
  char          unused[2];       // force memory alignment
  unsigned char *data;           // image data
};

// ***************************************
// prototypes

// send filename and TargaFile to load into
// returns 1 for file loaded, 0 for not
int loadTargaFile(char *, TargaFile *);

int unloadTarga(TargaFile *);

#endif
