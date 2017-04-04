#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <complex>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

ios_base & dummy_polynomial_manipulator ( ios_base & x )
{ 
  return x;
}

ios_base & ( * ( polynomial_variable ( const string & varName ) ) ) ( ios_base & x )
{
  return dummy_polynomial_manipulator;
}
class CPolynomial
{
  public:
    // default constructor
      CPolynomial ( void )
      {
          m_polynom.push_back ( 0 );
      }
    // operator +
      CPolynomial operator + ( const CPolynomial pol_b ) const
      {
          CPolynomial res;
          size_t size;
          
          size = ( pol_b.Degree () > this->Degree () ) ? pol_b.Degree () : this->Degree ();
          for ( size_t i = 0; i <= size; i++ )
          {
              res [ i ] =
                  ( i <= pol_b.Degree () ? pol_b [ i ] : 0 ) 
                  + ( i <= this->Degree () ? m_polynom [ i ] : 0 );
          }
          return res;
      }

    // operator -
      CPolynomial operator - ( const CPolynomial pol_b ) const
      {
          CPolynomial res;
          size_t size;

          size = ( pol_b.Degree () > this->Degree () ) ? pol_b.Degree () : this->Degree ();
          for ( size_t i = 0; i <= size; i++ )
          {
              res [ i ] =
                  ( i <= this->Degree () ? m_polynom [ i ] : 0 )
                  - ( i <= pol_b.Degree () ? pol_b [ i ] : 0 );
          }
          return res;
      }
    // operator * (polynomial, double)
      CPolynomial operator * ( const double times ) const
      {
          CPolynomial res;

          for ( size_t i = 0; i <= this->Degree (); i++ )
          {
              res [ i ] = m_polynom [ i ] * times;
          }
          return res;
      }

      CPolynomial operator * ( const CPolynomial pol_b ) const
      {
          CPolynomial res;
          int item_cnt, size_this, size_b;

          size_this = this->Degree ();
          size_b = pol_b.Degree ();
          item_cnt = size_this + size_b;

          for ( int i = 0; i <= item_cnt; i++ )
              res [ i ] = 0;

          for ( size_t i = 0; i <= this->Degree (); i++ )
          {
              for ( size_t j = 0; j <= pol_b.Degree (); j++ )
              {
                  res [ i + j ] += m_polynom [ i ] * pol_b [ j ];
              }
          }
          return res;
      }

    // operator ==
      bool operator == ( const CPolynomial pol_b ) const
      {
          if ( pol_b.Degree () != this->Degree () )
              return false;
          for ( size_t i = 0; i <= this->Degree (); i++ )
          {
              if ( m_polynom [ i ] != pol_b [ i ] )
                  return false;
          }
          return true;
      }
    // operator !=
      bool operator != ( const CPolynomial pol_b ) const
      {
          if ( pol_b.Degree () != this->Degree () )
              return true;
          for ( size_t i = 0; i <= this->Degree (); i++ )
              if ( m_polynom [ i ] != pol_b [ i ] )
                  return true;
          return false;
      }
    // operator []
      double & operator [] ( const int koef)
      {
          if ( ( size_t ) koef >= m_polynom.size () )
              for ( size_t i = m_polynom.size (); i <= ( size_t ) koef; i++ )
                  m_polynom.push_back ( 0 );
          return m_polynom [ koef ];
      }

      double operator [] ( const int koef ) const
      {
          return m_polynom [ koef ];
      }
    // operator ()
      double operator () ( const double x ) const
      {
          double res = 0;
          
          if ( x == 0 )
              return m_polynom [ 0 ];
          for ( size_t i = 0; i <= this->Degree (); i++ )
              res += m_polynom [ i ] * expontent ( x, i );
          return res;
      }
    // operator <<
      friend ostream & operator << ( ostream & ostr, const CPolynomial pol_b )
      {
          int size = pol_b.Degree ();

          if ( size == 0 && pol_b [ size ] >= 0 ) { ostr << pol_b [ size ];          return ostr; }
          if ( size == 0 && pol_b [ size ] < 0 )  { ostr << "- " << -pol_b [ size ]; return ostr; }

          for ( int i = size; i >= 0; --i )
          {
              if ( pol_b [ i ] == 0 ) continue;
              if ( i == size && pol_b [ i ] == 1 )                             { ostr << "x^" << i;                           continue; }
              if ( i == size && pol_b [ i ] == -1 )                            { ostr << "- x^" << i;                         continue; }
              if ( i == size && pol_b [ i ] > 0 )                              { ostr << pol_b [ i ] << "*x^" << i;           continue; }
              if ( i == size && pol_b [ i ] < 0 )                              { ostr << "- " << -pol_b [ i ] << "*x^" << i;  continue; }
              if ( i < size && i > 0 && pol_b [ i ] != 1 && pol_b [ i ] > 0 )  { ostr << " + " << pol_b [ i ] << "*x^" << i;  continue; }
              if ( i < size && i > 0 && pol_b [ i ] != -1 && pol_b [ i ] < 0 ) { ostr << " - " << -pol_b [ i ] << "*x^" << i; continue; }
              if ( i < size && i > 0 && pol_b [ i ] == 1 )                     { ostr << " + x^" << i;                        continue; }
              if ( i < size && i > 0 && pol_b [ i ] == -1 )                    { ostr << " - x^" << i;                        continue; }
              if ( i == 0 && pol_b [ i ] > 0 )                                 { ostr << " + " << pol_b [ i ];                continue; }
              if ( i == 0 && pol_b [ i ] < 0 )                                 { ostr << " - " << -pol_b [ i ];               continue; }
          }
          return ostr;
      }
    // Degree (), returns unsigned value
      unsigned int Degree ( void ) const
      {
          size_t size = m_polynom.size() - 1;
          while ( m_polynom [ size ] == 0 && size > 0 )
              size--;
          return size;
      }

  private:
    // todo
      vector<double> m_polynom;

      double expontent ( const double x_value, const int koef ) const
      {
          double res = x_value;
          if ( koef == 0 )
              return 1;
          for ( int i = 1; i < koef; i++ )
              res *= x_value;
          return res;
      }
};

#ifndef __PROGTEST__
bool               smallDiff                               ( double            a,
                                                             double            b )
{
    return fabs ( a / b - 1 ) < 10e-9;
}

bool               dumpMatch                               ( const CPolynomial & x,
                                                             const vector<double> & ref )
{   
    if ( ref.size () != x.Degree () + 1 ) return false;
    for ( size_t i = 0; i <= x.Degree (); i++ )
    {
        if ( x [ i ] != ref [ i ] )
            return false;
    }
    return true;
}

int                main                                    ( void )
{
  CPolynomial a, b, c;
  ostringstream out;

  a [ 0 ] = -10;
  a [ 1 ] = 3.5;
  a [ 3 ] = 1;  

  /*
  assert ( smallDiff ( a ( 2 ), 5 ) );
  out . str ("");
  out << a;
  assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
  a = a * -2;
  assert ( a . Degree () == 3
           && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  out . str ("");
  out << a;
  assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  out . str ("");
  out << b;
  assert ( out . str () == "0" );
  b[5] = -1;
  out . str ("");
  out << b;
  assert ( out . str () == "- x^5" );
  c = a + b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a - b;
  assert ( c . Degree () == 5
           && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
  c = a * b;
  assert ( c . Degree () == 8
           && dumpMatch ( c, vector<double>{ 0.0, -0.0, 0.0, -0.0, -0.0, -20.0, 7.0, 0.0, 2.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
  assert ( a != b );
  b[5] = 0;
  assert ( !(a == b) );
  a = a * 0;
  assert ( a . Degree () == 0
           && dumpMatch ( a, vector<double>{ 0.0 } ) );

  assert ( a == b );

  // bonus
  a[2] = 4;
  a[1] = -3;
  b[3] = 7;
  out . str ("");
  out << polynomial_variable ( "y" ) << "a=" << a << ", b=" << b;
  assert ( out . str () == "a=4*y^2 - 3*y^1, b=7*y^3" );

  out . str ("");
  out << polynomial_variable ( "test" ) << c;
  assert ( out . str () == "2*test^8 + 7*test^6 - 20*test^5" );
  */
  return 0;
}
#endif /* __PROGTEST__ */
