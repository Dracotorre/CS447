// TgaList.h
/*
    David Shrock
    CS447 - PSU
    2004.07.13

  header for TgaList.cpp

  List of Targa image files.
  Data structure of array that loads/unloads and holds the Targa images that
  are needed to save memory with only having one copy. Use getPtr() to have a
  pointer to the image data to use on object. Array chosen for peformance and
  avoiding pointer indirection. But in order to achieve this goal, this data
  structure should only be created at world/level initialization. Avoid adding
  and removing elements during run-time rendering as much as possible.

  Data structure for TgaList is a linear linked list.

  To use: start by sending the number of Targa images required for the
  current world in the constructor (or largest for memory and combine with a
  manager.) Then use add() which will return the index.
*/
#ifndef _TGA_LIST_H
#define _TGA_LIST_H

#include "imageFile.h"              // to load/unload Targa images

class TgaList
{
  public:
    TgaList();                             // default constructor: 4 empty images
    TgaList(int);                          // primary constructor: send size
    TgaList(const TgaList &);              // copy constructor
    ~TgaList();                            // destructor
    TgaList & operator =(const TgaList &);

    /* add an image.
       First adds by filling empty elements, but once filled then dynamically
       increases size and adds another. Be careful about performance.
       send:   file name
       return: index of new image or -1 for no file found
    */
    int add(char *);  

    // get ptr to image data, send index
    unsigned char* getPtr(const int);

    // get image dimensions
    int getHeight(const int);
    int getWidth(const int);

    // return size of list
    int getSize(void) { return m_size; } 

    /* replace an image
       Will only replace if same image size to maximize memory use.
       Note, old pointers will now point to a different image.
       send:   index, file name
       return: 1 for success, -1 for no file found, 0 for not replaced
   */
   int replace(const int, char *);

  private:
    int m_size;                    // number of total elements
    int m_cur;                     // current index counter to last filled item
    TargaFile *m_list;

    void initTargaFiles(void);     // initialize the struct in the list
};
#endif
