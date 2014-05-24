// Vec3D.cpp
/*
    David Shrock
    CS447 - PSU
    2004.07.04

    3D Vector includes both cartesian and polar coordinates.
    Currently, the polar coordinates are slow due to using trig functions.
    Note to me: consider using aproximations with MacLauren and Taylor
    series in future for faster ops.

    remember (see Cambridge Handbook of Physics Formulas, p21 for diagram):
    cartesian coordinates are r = (x, y, z)
    spherical polar coordinates are (r, theta, phi)
    theta(0) is pointing at +z
    phi(0) is pointing at +x
    theta and phi should only be between 0-pi radians
*/

#include <cmath>
using std::cos;
using std::sin;
#include "consts.h"         // for pi
#include "Vec3D.h"          // includes iostream
//#include <iomanip>
//using std::setw;
//using std::setprecision;

// **********************************
// default constructor
Vec3D::Vec3D()
{
  comp_vec[0] = 0.0;
  comp_vec[1] = 0.0;
  comp_vec[2] = 0.0;
  pcoord.theta = 0.0;
  pcoord.phi = 0.0;
  pcoord.r = 0.0;
  polar_valid = 0;
}

// **************************************
// vector component constructor
Vec3D::Vec3D(scalarT x, scalarT y, scalarT z)
{
  comp_vec[0] = x;
  comp_vec[1] = y;
  comp_vec[2] = z;
  pcoord.theta = 0.0;
  pcoord.phi = 0.0;
  pcoord.r = 0.0;
  polar_valid = 0;
}

// ***************************************
// polar constructor
Vec3D::Vec3D(const polar &pc)
{
  double sin_theta, cos_theta, sin_phi, cos_phi;

  polar_valid = 1;
  pcoord.theta = pc.theta;
  pcoord.phi = pc.phi;
  pcoord.r = pc.r;

  // try some common sin/cos values for more precision and simplicity
  if (pc.theta == PI)
  {
    sin_theta = 0.0;
    cos_theta = -1.0;
  }
  else if (pc.theta == PI/2)
  {
    sin_theta = 1.0;
    cos_theta = 0.0;
  }
  else
  {
    sin_theta = sin(pc.theta);
    cos_theta = cos(pc.theta);
  }
  if (pc.phi == PI)
  {
    sin_phi = 0.0;
    cos_phi = -1.0;
  }
  else if (pc.phi == PI/2)
  {
    sin_phi = 1.0;
    cos_phi = 0.0;
  }
  else 
  {
    sin_phi = sin(pc.phi);
    cos_phi = cos(pc.phi);
  }

  // compute components from polar - see Cambridge Handbook p21
  comp_vec[0] = (scalarT)(pcoord.r * sin_theta * cos_phi);
  comp_vec[1] = (scalarT)(pcoord.r * sin_theta * sin_phi);
  comp_vec[2] = (scalarT)(pcoord.r * cos_theta);
}

// **************************************
// copy constructor
Vec3D::Vec3D(const Vec3D &source)
{
  int i = 0;
  for (i = 0; i < 3; ++i)
  {
    comp_vec[i] = source.comp_vec[i];
  }
  pcoord.theta = source.pcoord.theta;
  pcoord.phi = source.pcoord.phi;
  pcoord.r = source.pcoord.r;
  polar_valid = source.polar_valid;
}

// **************************************
// destructor
Vec3D::~Vec3D()
{
  // don't need to do anything
}


// **************************************************************************
// = operator overload
Vec3D & Vec3D::operator =(const Vec3D &source)
{
  int i = 0;
  
  if (this != &source)
  {
    // don't need to destroy anything, just copy
    for (i = 0; i < 3; ++i)
    {
      comp_vec[i] = source.comp_vec[i];
    }
    pcoord.theta = source.pcoord.theta;
    pcoord.phi = source.pcoord.phi;
    pcoord.r = source.pcoord.r;
    polar_valid = source.polar_valid;
  }
  return (*this);
}


// **************************************************************************
// getp
// gets the polar coordinates
double Vec3D::getp(const char index)
{
  double result = -1.0;        // to return

  if (polar_valid == 0) 
  {
    calc_polar_angles();
    polar_valid = 1;
  }

  if (index == 'r') result = pcoord.r;
  else if (index == 't') result = pcoord.theta;
  else if (index == 'p') result = pcoord.phi;
  return result;
}


// **************************************************************************
// uNormal
// makes ths vector a unit vector (normalize)
int Vec3D::unitNorm(void) 
{
  // faster to multiply than divide, so factr = 1 / length
  double factr = 1 / (sqrt((double)(comp_vec[0] * comp_vec[0] + comp_vec[1] *
	                  comp_vec[1] + comp_vec[2] * comp_vec[2])));

  comp_vec[0] =  (scalarT)(comp_vec[0] * factr);
  comp_vec[1] =  (scalarT)(comp_vec[1] * factr);
  comp_vec[2] =  (scalarT)(comp_vec[2] * factr);
  return 1;
}

// **************************************************************************
// operator []
// get vector components x, y, or z
// send: 0 for x, 1 for y, 2 for z
scalarT Vec3D::operator [](int index) const
{
  if (index < 0) index = 0;
  else if (index > 2) index = 2;
  return comp_vec[index];
}

// **************************************************************************
// operator +=
// adds incomming Vec3D with current
// returns result (this)
Vec3D & Vec3D::operator +=(const Vec3D &vec2)
{
  polar_valid = 0;         // editing components, so polar is now invalid
  comp_vec[0] += vec2.comp_vec[0];
  comp_vec[1] += vec2.comp_vec[1];
  comp_vec[2] += vec2.comp_vec[2];
  return *this;
}

// **************************************************************************
// operator -=
// subtracts incomming Vec3D with current - cross product
// returns result (this)
Vec3D & Vec3D::operator -=(const Vec3D &vec2)
{
  polar_valid = 0;          // editing components, so polar is invalid
  comp_vec[0] -= vec2.comp_vec[0];
  comp_vec[1] -= vec2.comp_vec[1];
  comp_vec[2] -= vec2.comp_vec[2];
  return *this;
}

// **************************************************************************
// operator *=  (cross product to this)
// multiplies incomming Vec3D to current (this)
// returns result
Vec3D & Vec3D::operator *=(const Vec3D &rt)
{
  scalarT t0, t1, t2;    // temp holders
  polar_valid = 0;
  t0 = comp_vec[1] * rt.comp_vec[2] - comp_vec[2] * rt.comp_vec[1];
  t1 = comp_vec[2] * rt.comp_vec[0] - comp_vec[0] * rt.comp_vec[2];
  t2 = comp_vec[0] * rt.comp_vec[1] - comp_vec[1] * rt.comp_vec[0];
  comp_vec[0] = t0;
  comp_vec[1] = t1;
  comp_vec[2] = t2;
  return *this;
}

// **************************************************************************
// operator *=     scalar product to this
Vec3D & Vec3D::operator *=(const scalarT &scalar)
{
  comp_vec[0] *= scalar;
  comp_vec[1] *= scalar;
  comp_vec[2] *= scalar;
  return *this;
}

// ***********************************************
// friends
// **************************************************************************
// operator +
// adds two vectors together
Vec3D operator +(const Vec3D &lt, const Vec3D &rt)
{
  // since we have a constructor, might as well use it
  return Vec3D(lt.comp_vec[0] + rt.comp_vec[0],
               lt.comp_vec[1] + rt.comp_vec[1],
               lt.comp_vec[2] + rt.comp_vec[2]);
}

// **************************************************************************
// operator -
// subtracts two vectors together
Vec3D operator -(const Vec3D &lt, const Vec3D &rt)
{
  return Vec3D(lt.comp_vec[0] - rt.comp_vec[0],
               lt.comp_vec[1] - rt.comp_vec[1],
               lt.comp_vec[2] - rt.comp_vec[2]);
}


// *************************************************************************
// operator *   scalar first element
// scalar multiply vector
Vec3D operator *(const scalarT &sleft, const Vec3D &vec)
{
  return Vec3D(sleft * vec.comp_vec[0],
               sleft * vec.comp_vec[1],
               sleft * vec.comp_vec[2]);
}

// **************************************************************************
// operator *   scalar 2nd element
Vec3D operator *(const Vec3D &vec, const scalarT &sright)
{
  return Vec3D(sright * vec.comp_vec[0],
               sright * vec.comp_vec[1],
               sright * vec.comp_vec[2]);
}

// **************************************************************************
// operator *    vector cross product
Vec3D operator *(const Vec3D &l, const Vec3D &r)
{
  return Vec3D(l.comp_vec[1] * r.comp_vec[2] - l.comp_vec[2] * r.comp_vec[1],
               l.comp_vec[2] * r.comp_vec[0] - l.comp_vec[0] * r.comp_vec[2],
               l.comp_vec[0] * r.comp_vec[1] - l.comp_vec[1] * r.comp_vec[0] );
}

// **************************************************************************
// operator /     scalar division
// vector / scalar
Vec3D operator /(const Vec3D &vec, const scalarT &sright)
{
  scalarT temp = 1 / sright;
  return Vec3D(vec.comp_vec[0] * temp,
               vec.comp_vec[1] * temp,
               vec.comp_vec[2] * temp);
}

// **************************************************************************
// inequality op
bool operator != (const Vec3D &lt, const Vec3D &rt)
{
  if (lt.comp_vec[0] == rt.comp_vec[0] &&
      lt.comp_vec[1] == rt.comp_vec[1] &&
      lt.comp_vec[2] == rt.comp_vec[2] )
    return false;
  else return true;
}

// **************************************************************************
// equality op
bool operator == (const Vec3D &lt, const Vec3D &rt)
{
  // use some tolerance
  scalarT tol = 0.0004f;
  if ( (lt.comp_vec[0] < rt.comp_vec[0] + tol) &&
       (lt.comp_vec[0] > rt.comp_vec[0] - tol) &&
       (lt.comp_vec[1] < rt.comp_vec[1] + tol) &&
       (lt.comp_vec[1] > rt.comp_vec[1] - tol) &&
       (lt.comp_vec[2] < rt.comp_vec[2] + tol) &&
       (lt.comp_vec[2] > rt.comp_vec[2] - tol) )
    return true;
  else return false;
}

// **************************************************************************
// operator <<         output stream
/*
ostream & operator <<(ostream &out, const Vec3D &vec)
{
  int w = out.width();
  int p = out.precision();
  
  out << setw(0) << "(";
  out << setw(w) << setprecision(p) << vec.comp_vec[0] << setw(0) << ",";
  out << setw(w) << setprecision(p) << vec.comp_vec[1] << setw(0) << ",";
  out << setw(w) << setprecision(p)  << vec.comp_vec[2] << setw(0) << ")";
  return out;
}
*/
// **************************************************************************
// dot
scalarT dotProduct(const Vec3D &left, const Vec3D &right)
{
  return left.comp_vec[0] * right.comp_vec[0] +
         left.comp_vec[1] * right.comp_vec[1] +
         left.comp_vec[2] * right.comp_vec[2];
}

// **************************************************************************
// norm
// returns the Euclidean norm (length of norm) of incomming vector
double norm(const Vec3D &vec)
{
  return sqrt(dotProduct(vec, vec));
}

// *************************************************************************
// private functions
// ***************************************************************
// ************************************************************************
// calc_polar_angles
// calculates the polar angles
void Vec3D::calc_polar_angles(void)
{
  // length of projection in x-y plane:
  const double rhosqr = comp_vec[0] * comp_vec[0] + comp_vec[1] * comp_vec[1];
  const double rho = sqrt(rhosqr);

  // find norm of vector (radius)
  pcoord.r = (scalarT) sqrt(rhosqr + comp_vec[2] * comp_vec[2]);
  // find azimuth of vector
  if ((comp_vec[0] == 0.0) && (comp_vec[1] == 0.0)) pcoord.phi = 0.0;
  else pcoord.phi = atan2(comp_vec[1], comp_vec[0]);
  if (pcoord.phi < 0.0) pcoord.phi += (2.0 * PI);
  // find altitude of vector
  if ((comp_vec[2] == 0.0) && (rho == 0.0)) pcoord.theta = 0.0;
  else pcoord.theta = acos(comp_vec[2] / pcoord.r);
  return;
}
