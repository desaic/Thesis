#include "Vector3i.h"
const Vector3i Vector3i::ZERO = Vector3i(0,0,0);
Vector3i::Vector3i( int f )
{
  val[0] = f;
  val[1] = f;
  val[2] = f;
}

Vector3i::Vector3i( int x, int y, int z )
{
  val[0] = x;
  val[1] = y;
  val[2] = z;
}

// copy constructors
Vector3i::Vector3i( const Vector3i& rv )
{
  val[0] = rv[0];
  val[1] = rv[1];
  val[2] = rv[2];
}


// assignment operators
Vector3i& Vector3i::operator = ( const Vector3i& rv )
{
  val[0] = rv[0];
  val[1] = rv[1];
  val[2] = rv[2];
  return *this;
}

const int& Vector3i::operator [] ( int i ) const
{
  return val[i];
}

int& Vector3i::operator [] ( int i )
{
  return val[i];
}
