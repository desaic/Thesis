#ifndef VECTOR3I_H
#define VECTOR3I_H
///@brief vector of 3 integers
class Vector3i
{
public:
  static const Vector3i ZERO;

  Vector3i( int f = 0 );
  Vector3i( int x, int y, int z );
  // copy constructors
  Vector3i( const Vector3i& rv );

  // assignment operators
  Vector3i& operator = ( const Vector3i& rv );
  const int& operator [] ( int i ) const;
  int& operator [] ( int i );

private:
  int val[3];
 
};
#endif
