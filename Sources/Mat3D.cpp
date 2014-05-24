// Mat3D.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.17

    implementation of 3D matrix, Mat3D

   For future: create a 4D matrix that is a single-array of 16 elements
   to avoid column-major vs row-major confusion.
*/

#include <cmath>
using std::cos;
using std::sin;
#include "consts.h"
#include "Mat3D.h"          // also includes vec3d.h
//#include <iomanip>
//using std::setw;
//using std::setprecision;

// ****************************************
// default constructor
Mat3D::Mat3D()
{
  mat[0][0] = 1.0;      // set to identity
  mat[0][1] = 0.0;
  mat[0][2] = 0.0;
  mat[1][0] = 0.0;
  mat[1][1] = 1.0;
  mat[1][2] = 0.0;
  mat[2][0] = 0.0;
  mat[2][1] = 0.0;
  mat[2][2] = 1.0;

}

// ******************************************
// vector constructor from column vectors
Mat3D::Mat3D(const Vec3D &e1, const Vec3D &e2, const Vec3D &e3)
{
  mat[0][0] = e1[0];
  mat[1][0] = e1[1];
  mat[2][0] = e1[2];
  mat[0][1] = e2[0];
  mat[1][1] = e2[1];
  mat[2][1] = e2[2];
  mat[0][2] = e3[0];
  mat[1][2] = e3[1];
  mat[2][2] = e3[2];
}

// ******************************************
// copy constructor
Mat3D::Mat3D(const Mat3D &source)
{
  int i = 0;

  for (i = 0; i < 3; ++i)
  {
    mat[i][0] = source.mat[i][0];
    mat[i][1] = source.mat[i][1];
    mat[i][2] = source.mat[i][2];
  }
}

// ******************************************
// destructor
Mat3D::~Mat3D()
{
  // nothing to do
}

// ************************************************************************
// = operator overload
Mat3D & Mat3D::operator =(const Mat3D &source)
{
  int i = 0;

  if (this != &source)
  {
    for (i = 0; i < 3; ++i)
    {
      mat[i][0] = source.mat[i][0];
      mat[i][1] = source.mat[i][1];
      mat[i][2] = source.mat[i][2];
    }
  }
  return *this;
}

// ************************************************************************
// +=
// adds incomming matrix with current
Mat3D & Mat3D::operator +=(const Mat3D &m2)
{
  int i = 0;

  for (i = 0; i < 3; ++i)
  {
    mat[i][0] += m2.mat[i][0];
    mat[i][1] += m2.mat[i][1];
    mat[i][2] += m2.mat[i][2];
  }
  return *this;
}

// ************************************************************************
// -=
// subtracts incomming matrix with current
Mat3D & Mat3D::operator -=(const Mat3D &m2)
{
  int i = 0;

  for (i = 0; i < 3; ++i)
  {
    mat[i][0] -= m2.mat[i][0];
    mat[i][1] -= m2.mat[i][1];
    mat[i][2] -= m2.mat[i][2];
  }
  return *this;
}

// ************************************************************************
// *=    matrix * matrix
// matrix multiplication
Mat3D & Mat3D::operator *=(const Mat3D &m2)
{
  int i = 0, j = 0, k = 0;
  Mat3D temp;

  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 3; ++j)
    {
      temp.mat[i][j] = 0.0;                // reset
      for (k = 0; k < 3; ++k)
      {
        temp.mat[i][j] += mat[i][j] * m2.mat[k][j];
      }
    }
  }
  for (i = 0; i < 3; ++i)
  {
    mat[i][0] = temp.mat[i][0];
    mat[i][1] = temp.mat[i][1];
    mat[i][2] = temp.mat[i][2];
  }
  return *this;
}

// ************************************************************************
// *=        scalar
// scalar multiplication to this
Mat3D & Mat3D::operator *=(const scalarT scalar)
{
  int i = 0;
  
  for (i = 0; i < 3; ++i)
  {
    mat[i][0] *= scalar;
    mat[i][1] *= scalar;
    mat[i][2] *= scalar;
  }
  return *this;
}

// ************************************************************************
// Friends
// *****************

// *************************************************************************
// col
// component access of columns
// send:  matrix, index of vector component: x=0, y=1, z=3
Vec3D col(const Mat3D &matrix, int index)
{
  scalarT e1 = 0.0, e2 = 0.0, e3 = 0.0;       // elements of vector

  if (index >= 0 && index < 3)
  {
    e1 = matrix.mat[0][index];
    e2 = matrix.mat[1][index];
    e3 = matrix.mat[2][index];
  }
  return Vec3D(e1, e2, e3);
}

// ************************************************************************
// row
// component access of rows
// send: matrix, index: x=0, y=1, z=3
Vec3D row(const Mat3D &matrix, int index)
{
  scalarT e1 = 0.0, e2 = 0.0, e3 = 0.0;       // elements of vector

  if (index >= 0 && index < 3)
  {
    e1 = matrix.mat[index][0];
    e2 = matrix.mat[index][1];
    e3 = matrix.mat[index][2];
  }
  return Vec3D(e1, e2, e3);
}

// ************************************************************************
// id
// creates identity matrix
Mat3D id(void)
{
  Mat3D result;
  int i = 0, j = 0;

  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 3; ++j)
    {
      if (i == j) result.mat[i][j] = 1;
      else result.mat[i][j] = 0;
    }
  }
  return result;
}

// ************************************************************************
// rot_x
// creates elementary rotation matrix about x
Mat3D rot_x(const double angle)
{
  Mat3D result;
  double sAngle = 0.0;          // sin of angle
  double cAngle = 0.0;          // cos of angle
  
  // try some popular angles for better precision
  if (angle == PI) cAngle = -1.0;               // s already 0.0
  else if (angle == PI/2) sAngle = 1.0;         // c already 0.0
  else if (angle == 0.0) cAngle = 1.0;
  else
  {
    sAngle = sin(angle);
    cAngle = cos(angle);
  }
  result.mat[0][0] = 1.0f;
  result.mat[0][1] = 0.0f;
  result.mat[0][2] = 0.0f;
  result.mat[1][0] = 0.0f;
  result.mat[1][1] = (scalarT)cAngle;
  result.mat[1][2] = (scalarT)sAngle;
  result.mat[2][0] = 0.0f;
  result.mat[2][1] = (scalarT)-sAngle;
  result.mat[2][2] = (scalarT)cAngle;
  
  return result;
}

// ************************************************************************
// rot_y
// creates elementary rotation matrix about y
Mat3D rot_y(const double angle)
{
  Mat3D result;
  double sAngle = 0.0;          // sin of angle
  double cAngle = 0.0;          // cos of angle

  // try some popular angles for better precision
  if (angle == PI) cAngle = -1.0;               // s already 0.0
  else if (angle == PI/2) sAngle = 1.0;         // c already 0.0
  else if (angle == 0.0) cAngle = 1.0;
  else
  {
    sAngle = sin(angle);
    cAngle = cos(angle);
  }
  result.mat[0][0] = (scalarT)cAngle;
  result.mat[0][1] = 0.0f;
  result.mat[0][2] = (scalarT)-sAngle;
  result.mat[1][0] = 0.0f;
  result.mat[1][1] = 1.0f;
  result.mat[1][2] = 0.0f;
  result.mat[2][0] = (scalarT)sAngle;
  result.mat[2][1] = 0.0f;
  result.mat[2][2] = (scalarT)cAngle;

  return result;
}

// ************************************************************************
// rot_z
// creates elementary rotation matrix about z
Mat3D rot_z(const double angle)
{
  Mat3D result;
  double sAngle = 0.0;          // sin of angle
  double cAngle = 0.0;          // cos of angle

  // try some popular angles for better precision
  if (angle == PI) cAngle = -1.0;               // s already 0.0
  else if (angle == PI/2) sAngle = 1.0;         // c already 0.0
  else if (angle == 0.0) cAngle = 1.0;
  else
  {
    sAngle = sin(angle);
    cAngle = cos(angle);
  }
  result.mat[0][0] = (scalarT)cAngle;
  result.mat[0][1] = (scalarT)sAngle;
  result.mat[0][2] = 0.0f;
  result.mat[1][0] = (scalarT)-sAngle;
  result.mat[1][1] = (scalarT)cAngle;
  result.mat[1][2] = 0.0f;
  result.mat[2][0] = 0.0f;
  result.mat[2][1] = 0.0f;
  result.mat[2][2] = 1.0f;

  return result;
}

// *************************************************************************
// transpose
// create transpose of matrix
Mat3D transpose(const Mat3D &matrix)
{
  int i = 0, j = 0;
  Mat3D result;
  
  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 3; ++j)
    {
      result.mat[i][j] = matrix.mat[j][i];
    }
  }
  return result;
}

// *************************************************************************
// +   
// adds two Mat3D together
Mat3D operator +(const Mat3D &left, const Mat3D &right)
{
  Mat3D result;
  int i = 0, j = 0;

  for (i = 0; i < 3; ++j)
  {
    for (j = 0; j < 3; ++j)
    {
      result.mat[i][j] = left.mat[i][j] + right.mat[i][j];
    }
  }
  return result;
}

// ************************************************************************
// -
// subtracts two Mat3D together
Mat3D operator -(const Mat3D &left, const Mat3D &right)
{
  Mat3D result;
  int i = 0, j = 0;

  for (i = 0; i < 3; ++j)
  {
    for (j = 0; j < 3; ++j)
    {
      result.mat[i][j] = left.mat[i][j] - right.mat[i][j];
    }
  }
  return result;
}


// ************************************************************************
// *    scalar * matrix
// scalar multiply matrix
Mat3D operator *(const scalarT scalar, const Mat3D &matrix)
{
  Mat3D result;
  int i = 0, j = 0;

  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 3; ++j)
    {
      result.mat[i][j] = scalar * matrix.mat[i][j];
    }
  }
  return result;
}

// ************************************************************************
// *      matrix * scalar
Mat3D operator *(const Mat3D &matrix, const scalarT scalar)
{
  return (scalar * matrix);
}

// ************************************************************************
// *    matrix * matrix
// matrix multiplication
Mat3D operator *(const Mat3D &left, const Mat3D &right)
{
  Mat3D result;
  int i = 0, j = 0, k = 0;

  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 3; ++j)
    {
      result.mat[i][j] = 0.0;                // reset
      for (k = 0; k < 3; ++k)
      {
        result.mat[i][j]+= left.mat[i][j] * right.mat[k][j];
      }
    }
  }
  return result;
}

// ************************************************************************
// *    matrix * vector
Vec3D operator *(const Mat3D &matrix, const Vec3D &vector)
{
  scalarT e[3];           // elements of returning vec3d
  int i = 0, j = 0;
  
  for (i = 0; i < 3; ++i)
  {
    e[i] = 0.0;              // init
    for (j = 0; j < 3; ++j)
    {
      e[i] += matrix.mat[i][j] * vector[j];
    }
  }
  return Vec3D(e[0], e[1], e[2]);
}

// ************************************************************************
// *     vector * matrix
Vec3D operator *(const Vec3D &vector, const Mat3D &matrix)
{
  scalarT e[3];           // elements of returning vec3d
  int i = 0, j = 0;
  
  for (j = 0; j < 3; ++j)
  {
    e[j] = 0.0;              // init
    for (i = 0; i < 3; ++i)
    {
      e[j] += matrix.mat[i][j] * vector[i];
    }
  }
  return Vec3D(e[0], e[1], e[2]);
}

// ************************************************************************
//  /       scalar division
Mat3D operator /(const Mat3D &matrix, const scalarT scalar)
{
  Mat3D result;
  int i = 0, j = 0;

  for (i = 0; i < 3; ++i)
  {
    for (j = 0; j < 3; ++j)
    {
      result.mat[i][j] = matrix.mat[i][j] / scalar;
    }
  }
  return result;
}

// *************************************************************************
//  <<
// simple output
/*
ostream & operator <<(ostream &os, const Mat3D &matrix)
{
  int w = os.width();         // get the width if user defined
  int p = os.precision();
  int i = 0;
  
  for (i = 0; i < 3; ++i)
  {
    os << setw(0) << "(";
    os << setw(w) << setprecision(p) << matrix.mat[i][0] << setw(0) << ",";
    os << setw(w) << setprecision(p) << matrix.mat[i][1] << setw(0) << ",";
    os << setw(w) << setprecision(p) << matrix.mat[i][2] << setw(0) << "); ";
  }
  return os;
}
*/
