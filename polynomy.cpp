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
      CPolynomial operator + (CPolynomial pol_b)
      {
          CPolynomial res;
          int size;
          
          size = ( pol_b.Degree () > this->Degree () ) ? pol_b.Degree () : this->Degree ();
          for ( int i = 0; i <= size; i++ )
          {
              res [ i ] =
                  ( i <= pol_b.Degree () ? pol_b [ i ] : 0 ) 
                  + ( i <= this->Degree () ? m_polynom [ i ] : 0 );
          }
          return res;
      }

      void print_p ( void )
      {
          if ( this->Degree () == 0 )
          {
              cout << 0 << endl; 
              return;
          }

          for ( int i = this->Degree(); i >= 0; i-- )
          {
              if ( m_polynom [ i ] != 0 || i > 0)
                  cout << m_polynom [ i ] << "x^" << i << " ";
              if ( m_polynom [ i ] != 0 && i == 0 )
                  cout << m_polynom [ i ];
          }
          cout << endl;
      }
    // operator -
      CPolynomial operator - ( CPolynomial pol_b )
      {
          CPolynomial res;
          int size;

          size = ( pol_b.Degree () > this->Degree () ) ? pol_b.Degree () : this->Degree ();
          for ( int i = 0; i <= size; i++ )
          {
              res [ i ] =
                  ( i <= this->Degree () ? m_polynom [ i ] : 0 )
                  - ( i <= pol_b.Degree () ? pol_b [ i ] : 0 );
          }
          return res;
      }
    // operator * (polynomial, double)
      CPolynomial operator * ( double times )
      {
          CPolynomial res;

          for ( int i = 0; i <= this->Degree (); i++ )
          {
              res [ i ] = m_polynom [ i ] * times;
          }
          return res;
      }

      CPolynomial operator * ( CPolynomial pol_b )
      {
          CPolynomial res;
          int item_cnt, size_this, size_b;

          size_this = this->Degree ();
          size_b = pol_b.Degree ();
          item_cnt = size_this + size_b;

          for ( int i = 0; i <= item_cnt; i++ )
              res [ i ] = 0;

          for ( int i = 0; i <= this->Degree (); i++ ) 
          {
              for ( int j = 0; j <= pol_b.Degree (); j++ )
              {
                  res [ i + j ] += m_polynom [ i ] * pol_b [ j ];
              }
          }
          return res;
      }

    // operator ==
      bool operator == ( CPolynomial pol_b )
      {
          if ( pol_b.Degree () != this->Degree () )
              return false;
          for ( int i = 0; i <= this->Degree (); i++ )
          {
              if ( m_polynom [ i ] != pol_b [ i ] )
                  return false;
          }
          return true;
      }
    // operator !=
      bool operator != ( CPolynomial pol_b )
      {
          if ( pol_b.Degree () != this->Degree () )
              return true;
          for ( int i = 0; i <= this->Degree (); i++ )
          {
              if ( m_polynom [ i ] != pol_b [ i ] )
                  return true;
          }
          return false;
      }
    // operator []
      double & operator [] (int koef)
      {
          if ( koef >= m_polynom.size () )
          {
              for ( int i = m_polynom.size (); i <= koef; i++ )
                  m_polynom.push_back ( 0 );
          }
          return m_polynom [ koef ];
      }
    // operator ()
    // operator <<
    // Degree (), returns unsigned value
      unsigned int Degree ( void )
      {
          size_t size = m_polynom.size() - 1;
          while ( m_polynom [ size ] == 0 
              && size > 0 )
          {
              size--;
          }
          return size;
      }

  private:
    // todo
      vector<double> m_polynom;
};

#ifndef __PROGTEST__
bool               smallDiff                               ( double            a,
                                                             double            b )
{
  // DIY 
}

bool               dumpMatch                               ( const CPolynomial & x,
                                                             const vector<double> & ref )
{
  // DIY 
}

int                main                                    ( void )
{
  CPolynomial a, b, c;
  ostringstream out;

  a [ 0 ] = -10;
  a [ 1 ] = 3.5;
  a [ 2 ] = 2;
  //a.print_p ();
  //cout << a.Degree () << endl;
  a [ 3 ] = 1;

  b [ 0 ] = -10;
  b [ 1 ] = 3.5;
  b [ 2 ] = 2;
  b [ 3 ] = 1;
  b [ 7 ] = 5;
  
  c [ 0 ] = 0;
  c [ 1 ] = 0;
  
  ( a*b ).print_p ();

  /*( a != b ) ? 
      ( cout << "not equal" << endl ) 
      : ( cout << "equal" << endl );*/
  /*( a == b ) ?
      ( cout << "equal" << endl )
      : ( cout << "not equal" << endl );*/
  
  /*a.print_p ();
  cout << a.Degree () << endl;
  ( a - a ).print_p ();
  cout << ( a - a ).Degree () << endl;
*/
  //assert ( smallDiff ( a ( 2 ), 5 ) );
  //out . str ("");
  //out << a;
  //assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
  //a = a * -2;
  //assert ( a . Degree () == 3
  //         && dumpMatch ( a, vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  //out . str ("");
  //out << a;
  //assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  //out . str ("");
  //out << b;
  //assert ( out . str () == "0" );
  //b[5] = -1;
  //out . str ("");
  //out << b;
  //assert ( out . str () == "- x^5" );
  //c = a + b;
  //assert ( c . Degree () == 5
  //         && dumpMatch ( c, vector<double>{ 20.0, -7.0, 0.0, -2.0, 0.0, -1.0 } ) );

  //out . str ("");
  //out << c;
  //assert ( out . str () == "- x^5 - 2*x^3 - 7*x^1 + 20" );
  //c = a - b;
  //assert ( c . Degree () == 5
  //         && dumpMatch ( c, vector<double>{ 20.0, -7.0, -0.0, -2.0, -0.0, 1.0 } ) );

  //out . str ("");
  //out << c;
  //assert ( out . str () == "x^5 - 2*x^3 - 7*x^1 + 20" );
  //c = a * b;
  //assert ( c . Degree () == 8
  //         && dumpMatch ( c, vector<double>{ 0.0, -0.0, 0.0, -0.0, -0.0, -20.0, 7.0, 0.0, 2.0 } ) );

  //out . str ("");
  //out << c;
  //assert ( out . str () == "2*x^8 + 7*x^6 - 20*x^5" );
  //assert ( a != b );
  //b[5] = 0;
  //assert ( !(a == b) );
  //a = a * 0;
  //assert ( a . Degree () == 0
  //         && dumpMatch ( a, vector<double>{ 0.0 } ) );

  //assert ( a == b );

  //// bonus
  //a[2] = 4;
  //a[1] = -3;
  //b[3] = 7;
  //out . str ("");
  //out << polynomial_variable ( "y" ) << "a=" << a << ", b=" << b;
  //assert ( out . str () == "a=4*y^2 - 3*y^1, b=7*y^3" );

  //out . str ("");
  //out << polynomial_variable ( "test" ) << c;
  //assert ( out . str () == "2*test^8 + 7*test^6 - 20*test^5" );
  return 0;
}
#endif /* __PROGTEST__ */
