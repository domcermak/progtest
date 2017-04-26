#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
using namespace std;
#endif /* __PROGTEST__ */


class OrderingDoesNotExistException
{
};

class DuplicateMatchException
{
};

template <typename _M>
class CContest
{
public:
    // default constructor
    CContest ( void ) { }
    // destructor
    ~CContest ( void ) { }
    // AddMatch
    CContest & AddMatch ( const string & contestant1, const string & contestant2, const _M & result )
    {
        for ( int i = 0; i < v_matchList.size (); i++ )
        {
            if ( ( v_matchList [ i ].m_contestant1 == contestant1 && v_matchList [ i ].m_contestant2 == contestant2 )
                || ( v_matchList [ i ].m_contestant1 == contestant2 && v_matchList [ i ].m_contestant2 == contestant1 ) )
                throw new DuplicateMatchException ();
        }
        v_matchList.push_back ( C_Match ( contestant1, contestant2, result ) );
        return *this;
    }
    // IsOrdered ( comparator )
    template <typename T>
    bool IsOrdered ( T comparator ) const
    {
        for ( int i = 0; i < v_matchList.size (); i++ )
        {
            int found1 = -1, found2 = -1;
            for ( int k = 0; k < v_pointList.size (); k++ )
            {
                if ( v_matchList [ i ].m_contestant1 == v_pointList [ k ].m_name )
                    found1 = k;
                if ( v_matchList [ i ].m_contestant2 == v_pointList [ k ].m_name )
                    found2 = k;
            }
            if ( found1 == -1 )
            {
                v_pointList.push_back ( C_Points ( v_matchList [ i ].m_contestant1 ) );
                found1 = v_pointList.size () - 1;
            }
            if ( found2 == -1 )
            {
                v_pointList.push_back ( C_Points ( v_matchList [ i ].m_contestant2 ) );
                found2 = v_pointList.size () - 1;
            }
            if ( !comparator ( v_matchList [ i ].m_result ) )
                return false;
            else if ( comparator ( v_matchList [ i ].m_result ) > 0 )
                v_pointList [ found1 ].m_loosers.push_back ( found2 );
            else if ( comparator ( v_matchList [ i ].m_result ) < 0 )
                v_pointList [ found2 ].m_loosers.push_back ( found1 );
        }

        /*for ( int i = 0; i < v_pointList.size (); i++ )
        {
            if ( !Add_points ( i ) ) return false;
            for ( int k = 0; k < v_pointList.size (); k++ )
                if ( v_pointList [ k ].m_state == 1 )
                {
                    v_pointList [ k ].m_points++;
                    v_pointList [ k ].m_state = -1;
                }
        }*/

        // print
        for ( int i = 0; i < v_pointList.size (); i++ )
        {
            cout << v_pointList [ i ].m_name << " : ";
            for ( int k = 0; k < v_pointList [ i ].m_loosers.size (); k++ )
            {
                cout << v_pointList [ v_pointList [ i ].m_loosers [ k ] ].m_name << ", ";
            }
            cout << endl;
        }

        for ( int i = 0; i < v_pointList.size (); i++ )
            for ( int k = 0; k < v_pointList.size () - i - 1; k++ )
            {
                C_Points tmp;
                if ( v_pointList [ i ].m_points == v_pointList [ k ].m_points )
                    return false;
                if ( v_pointList [ i ].m_points < v_pointList [ k ].m_points )
                {
                    tmp = v_pointList [ i ];
                    v_pointList [ i ] = v_pointList [ k ];
                    v_pointList [ k ] = tmp;
                }
            }
        return true;
    }
    // Results ( comparator )
    template <typename T>
    void Results ( T comparator )
    {
        list <string> li;
    }
private:
    struct C_Points
    {
        C_Points ( void ) { }
        C_Points ( string name ) : m_name ( name ), m_points ( 0 ), m_state ( -1 ) { }
        ~C_Points ( void )
        {
            m_name.clear ();
            m_loosers.clear ();
        }
        C_Points & operator = ( C_Points & member )
        {
            C_Points ret;
            
            ret.m_loosers.resize ( member.m_loosers.size () );
            for ( int i = 0; i < member.m_loosers.size (); i++ )
                ret.m_loosers [ i ] = member.m_loosers [ i ];

            ret.m_name = new char [ member.m_name.size() ];
            ret.m_name.copy ( member.m_name.c_str (), member.m_name.size () );

            ret.m_points = member.m_points;
            ret.m_state = member.m_state;

            return ret;
        }

        vector <int> m_loosers;
        string m_name;
        int m_points;
        int m_state; // -1 = not visited, 0 = active, 1 = visited
    };

    struct C_Match
    {
        C_Match ( string str1, string str2, _M res )
            : m_contestant1 (str1), m_contestant2 (str2), m_result (res) { }
        string m_contestant1;
        string m_contestant2;
        _M m_result;

        ~C_Match ( void )
        {
            m_contestant1.clear ();
            m_contestant2.clear ();
        }
    };
    vector <C_Match> v_matchList;
    mutable vector <C_Points> v_pointList;

    //------------------------------------------------------------------------------
    bool Add_points ( int & i ) const
    {
        C_Points & member = v_pointList [ i ];

        if ( !member.m_state )
            return false;
        if ( member.m_state == 1 )
            return true;
        member.m_state = 0;
        for ( int k = 0; k < member.m_loosers.size (); k++ )
            if ( !Add_points ( member.m_loosers [ k ] ) ) return false;
        member.m_state = 1;
        return true;
    }
};
#ifndef __PROGTEST__
struct CMatch
{
  public:
                             CMatch                        ( int               a, 
                                                             int               b )
                             : m_A ( a ), 
                               m_B ( b )
    {
    }
    
    int                      m_A;
    int                      m_B;
};

class HigherScoreThreshold
{
  public:
                             HigherScoreThreshold          ( int diffAtLeast )
                             : m_DiffAtLeast ( diffAtLeast )
    {
    }
    int                      operator ()                   ( const CMatch & x ) const
    {
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
  private:
    int            m_DiffAtLeast;    
};

int                HigherScore                             ( const CMatch    & x )
{
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int                main                                    ( void )
{
  CContest<CMatch>  x;
  
  x . AddMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . AddMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . AddMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . AddMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . AddMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . AddMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . IsOrdered ( HigherScore ) );
  /*try
  {
    list<string> res = x . Results ( HigherScore );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const OrderingDoesNotExistException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
  */
  x . AddMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . IsOrdered ( HigherScore ) );
  /*try
  {
    list<string> res = x . Results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == NULL );
  }

  
  assert ( ! x . IsOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . Results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const OrderingDoesNotExistException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
  
  assert ( x . IsOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . Results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == NULL );
  }
  
  CContest<bool>  y;
  
  y . AddMatch ( "Python", "PHP", true )
    . AddMatch ( "PHP", "Perl", true )
    . AddMatch ( "Perl", "Bash", true )
    . AddMatch ( "Bash", "JavaScript", true )
    . AddMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . IsOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . Results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == NULL );
  }
    
  y . AddMatch ( "PHP", "JavaScript", false );
  assert ( !y . IsOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . Results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const OrderingDoesNotExistException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
  
  try
  {
    y . AddMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const DuplicateMatchException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }
  
  try
  {
    y . AddMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == NULL );
  }
  catch ( const DuplicateMatchException & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == NULL );
  }*/
  return 0;
}
#endif /* __PROGTEST__ */
