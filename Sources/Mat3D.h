// mat3d.h
/*
    David Shrock
    CS447 - PSU
    2004.07.17

    header for mat3d.cpp

    3D transformation matrices for vectors (Vec3D)
    includes addition, scalar multiple.

    updated for CS447, fixed an error. 
    first draft: 2003.12.21

    column vectors:  matrix[element][vector]
*/

#ifndef MAT3D_H_
#define MAT3D_H_

#include "Vec3D.h"

class Mat3D
{
  public:
    Mat3D();
    Mat3D(const Vec3D &e1, const Vec3D &e2, const Vec3D &e3);
    Mat3D(const Mat3D &);
    ~Mat3D();
    Mat3D & operator =(const Mat3D &);        // = op overload

    Mat3D & operator +=(const Mat3D &);
      // adds incomming matrix with current
      // returns result
    Mat3D & operator -=(const Mat3D &);
      // subtracts incomming matrix with current
      // returns result
    Mat3D & operator *=(const Mat3D &);
      // matrix multiplication
    Mat3D & operator *=(const scalarT);
      // scalar multiplication to this

    friend Vec3D col(const Mat3D &matrix, int index);
      // component access of columns
      // send:  matrix, index: x=0, y=1, z=3
    friend Vec3D row(const Mat3D &matrix, int index);
      // component access of rows
      // send: matrix, index: x=0, y=1, z=3
    friend Mat3D id(void);
      // creates identity matrix

    friend Mat3D rot_x(const double);  // elementary rotation matrix,
    friend Mat3D rot_y(const double);  //  send angle in radians
    friend Mat3D rot_z(const double);

    // create transpose of sent matrix
    friend Mat3D transpose(const Mat3D &);  

    // adds two Mat3D together
    friend Mat3D operator +(const Mat3D &, const Mat3D &);
    
    // subtracts two Mat3D together
    friend Mat3D operator -(const Mat3D &, const Mat3D &);
      
    // scalar multiply matrix (scalar * matrix)
    friend Mat3D operator *(const scalarT, const Mat3D &);
     
    // scalar multiply (matrix * scalar)
    friend Mat3D operator *(const Mat3D &, const scalarT);
      
    // matrix multiplication
    friend Mat3D operator *(const Mat3D &, const Mat3D &);  
    friend Vec3D operator *(const Mat3D &, const Vec3D &);
    friend Vec3D operator *(const Vec3D &, const Mat3D &);

    // scalar division. only one allowed
    friend Mat3D operator /(const Mat3D &, const scalarT);
      
    //friend ostream & operator <<(ostream &, const Mat3D &);

  private:
    scalarT mat[3][3];               // elements of matrix
};
#endif
