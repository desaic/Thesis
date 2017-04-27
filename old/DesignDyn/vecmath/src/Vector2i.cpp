#include "Vector2i.h"
const Vector2i Vector2i::ZERO = Vector2i(0,0);
Vector2i::Vector2i( int f )
{
  val[0] = f;
  val[1] = f;
}

Vector2i::Vector2i( int x, int y)
{
  val[0] = x;
  val[1] = y;
}

// copy constructors
Vector2i::Vector2i( const Vector2i& rv )
{
  val[0] = rv[0];
  val[1] = rv[1];
}


// assignment operators
Vector2i& Vector2i::operator = ( const Vector2i& rv )
{
  val[0] = rv[0];
  val[1] = rv[1];
  return *this;
}

const int& Vector2i::operator [] ( int i ) const
{
  return val[i];
}

int& Vector2i::operator [] ( int i )
{
  return val[i];
}
