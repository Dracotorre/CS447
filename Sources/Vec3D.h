// Vec3D.h
/*
    David Shrock
    CS447 - PSU
    2004.07.04

    header for vec3d.cpp

    3D Vector includes both cartesian and polar coordinates.
    Polar coords are currently not optimized.

    remember: 
    cartesian coordinates are r = (x, y, z)
    spherical polar coordinates are (r, theta, phi)
    theta(0) is pointing at z-axis
    phi(0) is pointing at x-axis
    theta and phi should only be between 0-pi radians
   
    reminder for me:
    do more optimizing

    2004.07.04: normalize() to get unit normal, equality ops added, scalarT
    2003.12.15: original
*/

#ifndef _VEC3D_H_           
#define _VEC3D_H_
//#include <iostream>

// define a scalarT for easy type changing. Need double for astronomy programs,
//                 but float is good enough for graphics.
typedef float scalarT;

// define the polar coordinates structure
struct polar
{
  double theta;          // azimuth of vector  (yaw)
  double phi;            // altitude of vector (pitch)
  scalarT r;              // radius of vector
};

class Vec3D
{
  public:
    Vec3D();                                // default constructor
    Vec3D(scalarT, scalarT, scalarT);       // cartesian constructor (x, y, z)
    Vec3D(const polar &);                   // polar constructor
    Vec3D(const Vec3D &);                   // copy constructor
    ~Vec3D();                               // destructor
    Vec3D & operator =(const Vec3D &);      // = operator overload

    // get the polar coordinates
    // send: 'r' for radius, 't' for theta, or 'p' for phi
    double getp(const char);

    // makes the unit normal vector of this vector (normalizes)
    int unitNorm(void);
     
    // get vector components x, y, or z
    // send: 0 for x, 1 for y, 2 for z
    scalarT operator [](int index) const;
      
   // adds incomming Vec3D with current
   Vec3D & operator +=(const Vec3D &);
     
    // subtracts incomming Vec3D with current
    Vec3D & operator -=(const Vec3D &);
     
    // multiplies incomming Vec3D to current. cross product to this
    Vec3D & operator *=(const Vec3D &);
      
    // scalarT multiplication to this
    Vec3D & operator *=(const scalarT &);

    // adds two Vec3D together
    friend Vec3D operator +(const Vec3D &, const Vec3D &);
     
    // subtracts two Vec3D together
    friend Vec3D operator -(const Vec3D &, const Vec3D &);
     
    // scalarT multiply vector
    friend Vec3D operator *(const scalarT &, const Vec3D &);
     
    // scalarT multiply vector
    friend Vec3D operator *(const Vec3D &, const scalarT &);
     
    // cross product vector multiplication - returns Normal (not unit)
    friend Vec3D operator *(const Vec3D &, const Vec3D &);
     
    // scalarT division. only one allowed
    friend Vec3D operator /(const Vec3D &, const scalarT &);

    // equality ops
    friend bool operator != (const Vec3D &, const Vec3D &);

    friend bool operator == (const Vec3D &, const Vec3D &);
      
    // output operator
    //friend ostream & operator <<(ostream &, const Vec3D &);
    
    // returns the dot product of left dot right (finds distance)
    friend scalarT dotProduct(const Vec3D &left, const Vec3D &right);

    // returns the Euclidean norm of incomming Vec3D (length)
    friend double norm(const Vec3D &);
      

  private:
    scalarT comp_vec[3];              // components of vector: x,y,z
    polar pcoord;                     // polar coordinates of
    int polar_valid;                  // if polar coords is valid, not then calc

    // calculates polar coordinates from vector coordinates
    void calc_polar_angles(void);
      
};
#endif
