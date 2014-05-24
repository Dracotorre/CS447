// TgaList.h
/*
    David Shrock
    CS447 - PSU
    2004.07.13

    implementation of TgaList
*/

#include "TgaList.h"


// default constructor
TgaList::TgaList()
{
  m_size = 4;
  m_cur = -1;
  m_list = new TargaFile[m_size];
  initTargaFiles();
}

// primary constructor
TgaList::TgaList(int elements)
{
  m_size = elements;
  m_cur = 0;
  m_list = new TargaFile[m_size];
  initTargaFiles();
}

// copy constructor
TgaList::TgaList(const TgaList &source)
{
  m_size = source.m_size;
  m_cur = source.m_cur;
  if (source.m_list)
  {
    m_list = new TargaFile[m_size];
    for (int i = 0; i < m_size; ++i)
    {
      m_list[i].width = source.m_list[i].width;
      m_list[i].height = source.m_list[i].height;
      m_list[i].type = source.m_list[i].type;
      m_list[i].bitCount = source.m_list[i].bitCount;
      if (source.m_list[i].data)
      {
        m_list[i].data = new unsigned char[sizeof(source.m_list[i].data)];
      }
      else m_list[i].data = 0;       // NULL
    }
  }
  else m_list = 0;                   // NULL
}

// destructor
TgaList::~TgaList()
{
  if (m_list)
  {
    for (int i = 0; i < m_size; ++i)
    {
      unloadTarga(&m_list[i]);
    }
    delete [] m_list;
  }
}

// = operator
TgaList & TgaList::operator =(const TgaList &source)
{
  if (this != &source)
  {
    m_size = source.m_size;
    m_cur = source.m_cur;
    if (source.m_list)
    {
      m_list = new TargaFile[m_size];
      for (int i = 0; i < m_size; ++i)
      {
        m_list[i].width = source.m_list[i].width;
        m_list[i].height = source.m_list[i].height;
        m_list[i].type = source.m_list[i].type;
        m_list[i].bitCount = source.m_list[i].bitCount;
        if (source.m_list[i].data)
        {
          m_list[i].data = new unsigned char[sizeof(source.m_list[i].data)];
        }
        else m_list[i].data = 0;
      }
    }
    else m_list = 0;
  }
  return(*this);
}

// ***********************************************************************
/* add
   First adds by filling empty elements, but once filled then dynamically
   increases size and adds another. Be careful about performance.
   send:   file name
   return: index of new image or -1 for no file found
*/ 
int TgaList::add(char *fname)
{
  TgaList temp;                 // copy holder if need to change size of array
  int i = 0;
  ++m_cur;
  int check = 0;                // if file loaded ok

  if (m_cur >= m_size)
  {
    // need to increase array size to hold more
    temp = *this;

    for (i = 0; i < m_size; ++i)
    {
      unloadTarga(&m_list[i]);
    }
    delete [] m_list;

    m_size = m_cur + 1;
    m_list = new TargaFile[m_size];
    
    for (i = 0; i < temp.m_size; ++i)
    {
      m_list[i].width = temp.m_list[i].width;
      m_list[i].height = temp.m_list[i].height;
      m_list[i].type = temp.m_list[i].type;
      m_list[i].bitCount = temp.m_list[i].bitCount;
      if (temp.m_list[i].data)
      {
        m_list[i].data = new unsigned char[sizeof(temp.m_list[i].data)];
      }
      else m_list[i].data = 0;
    }
  }
  check = loadTargaFile(fname, &m_list[m_cur]);

  if (check == 1) return m_cur;
  else return check;
}

// ************************************************************
// get ptr to image data, send index
unsigned char* TgaList::getPtr(const int index)
{
  if (index < 0 || index > m_cur) return 0;

  return m_list[index].data;
}

// ***********************************************************
// get image dimensions
int TgaList::getHeight(const int index)
{
  if (index < 0 || index > m_cur) return 0;

  return m_list[index].height;
}

int TgaList::getWidth(const int index)
{
  if (index < 0 || index > m_cur) return 0;

  return m_list[index].width;
}

// **********************************************************************
/* replace
   Will only replace if same image size to maximize memory use.
   Copies new data into same memory space.
   send:   index, file name
   return: 1 for success, -1 for no file found, 0 for not replaced
*/
int TgaList::replace(const int index, char *fname)
{
  int check = 0;                   // if temp targa loaded ok
  int result = 0;                  // to return
  TargaFile temp;
  int tsize, lsize = 0;            // image size of temp, list element
  int colorMode = 0;               // to calculate image size
  if (index < 0 || index > m_cur) result = 0;
  else
  {
    check = loadTargaFile(fname, &temp);
    if (check == 1)
    {
      colorMode = temp.bitCount / 8;
      tsize = temp.width * temp.height * colorMode;
      colorMode = m_list[index].bitCount / 8;
      lsize = m_list[index].width * m_list[index].height * colorMode;
      if (tsize == lsize)
      {
        // copy the new data over old data memory space
        for (int i = 0; i < tsize; ++i)
        {
          m_list[index].data[i] = temp.data[i];
        }
        // update width, height, etc in case different
        m_list[index].width = temp.width;
        m_list[index].height = temp.height;
        m_list[index].bitCount = temp.bitCount;
        m_list[index].type = temp.type;
        result = 1;
      }
      else result = 0;
    }
    else result = -1;
  }
  return result;
}

// *****************************************************************
// private functions
// *********************

// initTargaFiles
void TgaList::initTargaFiles(void)
{
  // initialize the struct
  for (int i = 0; i < m_size; ++i)
  {
    m_list[i].width = 0;
    m_list[i].height = 0;
    m_list[i].type = 0;
    m_list[i].bitCount = 0;
    m_list[i].data = 0;         // NULL
  }
  return;
}
