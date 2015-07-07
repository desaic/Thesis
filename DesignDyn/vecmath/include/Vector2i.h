#ifndef VECTOR2I_H
#define VECTOR2I_H
///@brief vector of 2 integers
class Vector2i
{
public:
  static const Vector2i ZERO;

  Vector2i( int f = 0 );
  Vector2i( int x, int y);
  // copy constructors
  Vector2i( const Vector2i& rv );

  // assignment operators
  Vector2i& operator = ( const Vector2i& rv );
  const int& operator [] ( int i ) const;
  int& operator [] ( int i );

private:
  int val[2];
 
};
#endif
