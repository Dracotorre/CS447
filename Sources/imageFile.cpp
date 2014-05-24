// imageFile.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.04

    Image file format handler for bitmap images.
    Intended use is with 3D graphics programming such as OpenGL.
*/


#include <stdio.h>
#include "imageFile.h"

/* *************************************
   loadTargaFile
   loads a targa image file from disk.
   returns 1 for image loaded, 0 for not
*/
int loadTargaFile(char *filename, TargaFile *targaFile)
{
  FILE            *fPtr;          // file pointer
  int result = 0;                 // to return
  long            imageSize;      // size of Targa image
  long            index;          // counter variable for in image
  int             colorMode;      // 4= RGBA, 3=RGB
  short int       badSInt;        // holder for targa header to skip
  unsigned char   badUChar;       // holder for targa header we wish to skip
  unsigned char   tempSwap;       // holder for swapping colors from BGR to RGB

  // open file
  fPtr = fopen(filename, "rb");
  if (fPtr)
  {
    // skip header data don't need
    fread(&badUChar, sizeof(unsigned char), 1, fPtr);
    fread(&badUChar, sizeof(unsigned char), 1, fPtr);

    // get image type
    fread(&targaFile->type, sizeof(unsigned char), 1, fPtr);

    // make sure type is useful for OpenGL, otherwise skip and close
    if ((targaFile->type == 2 || targaFile->type == 3))
    {
      // skip more header we don't need
      fread(&badSInt, sizeof(short int), 1, fPtr);
      fread(&badSInt, sizeof(short int), 1, fPtr);
      fread(&badUChar, sizeof(unsigned char), 1, fPtr);
      fread(&badSInt, sizeof(short int), 1, fPtr);
      fread(&badSInt, sizeof(short int), 1, fPtr);

      // get image dimensions
      fread(&targaFile->width, sizeof(short int), 1, fPtr);
      fread(&targaFile->height, sizeof(short int), 1, fPtr);

      // get image depth
      fread(&targaFile->bitCount, sizeof(unsigned char), 1, fPtr);

      // skip last header char
      fread(&badUChar, sizeof(unsigned char), 1, fPtr);

      // calculate color mode and image size
      colorMode = targaFile->bitCount / 8;
      imageSize = targaFile->width * targaFile->height * colorMode;

      // allocate memory for image data
      targaFile->data = new unsigned char[(sizeof(unsigned char) * imageSize)];

      // read in image data
      fread(targaFile->data, sizeof(unsigned char), imageSize, fPtr);

      // swap colors from BGR to RGB for OpenGL
      for (index = 0; index < imageSize; index += colorMode)
      {
        tempSwap = targaFile->data[index];
        targaFile->data[index] = targaFile->data[index + 2];
        targaFile->data[index + 2] = tempSwap;
      }
      result = 1;
    }
    else targaFile->data = NULL;
    fclose(fPtr);
  }
  else targaFile->data = NULL;

  return result;
}

// ****************************************************************
// unloadTarga
int unloadTarga(TargaFile *targaFile)
{
  if (targaFile->data) delete [] targaFile->data;
  targaFile->data = NULL;

  return 1;
}
