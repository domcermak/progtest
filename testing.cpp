#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;

class CVATRegister
{
public:
    CVATRegister ( void ) : m_register_item_cnt ( 0 )
    {
    }

    bool RegisterCompany ( const string & name )
    {
        T_Register member;
        if ( isMember ( name ) )
            return false;
        member.m_original_string = name;
        member.m_transformed_string = stringTransform ( name );

        //testf ( member.m_transformed_string );
        m_register_array.push_back ( member );
        m_register_item_cnt++;
        qSort_name ( m_register_array, 0, m_register_item_cnt - 1 );
        testf ( "finished" );
        return true;
    }
    /*
    bool AddIssued ( const CInvoice & x );
    bool AddAccepted ( const CInvoice & x );
    bool DelIssued ( const CInvoice & x );
    bool DelAccepted ( const CInvoice & x );
    list<CInvoice> Unmatched ( const string & company,
    const CSortOpt & sortBy ) const;
    */
    
    // testing
    void print_sorted ( void )
    {
        for ( int i = 0; i < m_register_item_cnt; i++ )
        {
            cout << i + 1 << ".  " << m_register_array [ i ].m_original_string << "  |  "
                << m_register_array [ i ].m_transformed_string << endl << endl;
        }
    }

    void testf ( void )
    {
        cout << "Test alert!" << endl;
    }

    void testf ( int i )
    {
        cout << "Test alert " << i << " !" << endl;
    }

    void testf ( unsigned int i )
    {
        cout << "Test alert " << i << " !" << endl;
    }

    void testf ( string str )
    {
        cout << str << endl;
    }
private:
    struct T_Register
    {
        string m_original_string; // from input
        string m_transformed_string; // to_lower + spaces reduced
        ~T_Register ( void )
        {
            m_original_string.clear ();
            m_transformed_string.clear ();
        }
    };

    vector <T_Register> m_register_array;
    int m_register_item_cnt; // -1 value enabled

    //----------------------------------------------------------------------
    bool isUpperAlpha ( char c )
    {
        return ( c >= 'A' && c <= 'Z' ) ? true : false;
    }

    bool isSpace ( char c )
    {
        return ( c == ' ' ) ? true : false;
    }

    void toLower ( char & c )
    {
        unsigned int diff = 'a' - 'A';
        c += diff;
    }

    //-------------------------------------------------------------------------
    void qSort_name ( vector<T_Register> & arr, int left, int right ) // arr, 0, -1
    {
        int pivot = ( m_register_item_cnt - 1 ) / 2;
        int head = left, tail = right;

        // partition
        while ( head <= tail )
        {
            //cout << "head: " << head << endl << "tail: "<< tail << endl;
            while ( 0 > strcmp ( arr [ head ].m_transformed_string.c_str (), arr [ pivot ].m_transformed_string.c_str () ) )
                head++;
            while ( 0 < strcmp ( arr [ tail ].m_transformed_string.c_str (), arr [ pivot ].m_transformed_string.c_str () ) )
                tail--;
            if ( head <= left )
            {
                T_Register tmp;
                tmp = arr [ head ];
                arr [ head ] = arr [ tail ];
                arr [ tail ] = tmp;
                head++;
                tail--;
            }
        }
        // recursion
        if ( left < head )  qSort_name ( arr, left, tail );
        if ( right > tail ) qSort_name ( arr, head, right );
    }
    
    int binarySearch ( string str, int head, int tail )
    {
        int ret_val, tip_point = ( head + tail ) / 2;
        
        if ( head >= tail ) return -1;
        ret_val = strcmp ( str.c_str (), m_register_array [ tip_point ].m_transformed_string.c_str () );
        if ( ret_val > 0 ) return binarySearch ( str, head, tip_point - 1 );
        else if ( ret_val < 0 ) return binarySearch ( str, tip_point + 1, tail );
        else return tip_point; // found
    }
    
    string stringTransform ( string name ) // space reduction, case to lower
    {
        char * tmp, *help;

        tmp = new char [ name.size () ];
        strcpy ( tmp, name.c_str () );
        
        for ( unsigned int i = 0; i < name.length (); i++ ) // char to lower
            if ( isUpperAlpha ( tmp [ i ] ) ) toLower ( tmp [ i ] );
        
        while ( isSpace ( tmp [ 0 ] ) ) // head
        {
            int len = strlen ( tmp ) + 1;
            help = new char [ len ];
            strcpy ( help, tmp );
            delete tmp;
            tmp = new char [ len - 1 ];
            for ( int i = 0; i < len; i++ )
                tmp [ i ] = help [ i + 1 ];
            delete help;
        }

        for ( int i = 0; i < ( int ) name.length () - 1; i++ ) // middle
        {
            if ( isSpace ( name.c_str () [ i ] ) && isSpace ( name.c_str () [ i + 1 ] ) )
            {
                int len = strlen ( tmp ) + 1;

                help = new char [ len ];
                strcpy ( help, tmp );
                delete tmp;
                tmp = new char [ len - 1 ]; // length decrease
                /*for ( int k = 0; k <= i; k++ )
                    tmp [ k ] = help [ k ];*/
                for ( int k = i + 1; k < len; k++ )
                    tmp [ k ] = help [ k + 1 ];
                delete help;
            }
        }
        testf ( tmp );
        while ( isSpace ( tmp [ strlen ( tmp ) ] ) ) // tail
        {
            int len = strlen ( tmp ) + 1;
            help = new char [ len ];
            strcpy ( help, tmp );
            delete tmp;
            tmp = new char [ len - 1 ];
            for ( int i = 0; i < len - 1; i++ )
                tmp [ i ] = help [ i ];
            delete help;
        }
        //testf ( tmp );

        name.copy ( tmp, strlen ( tmp ) + 1 );
        return name;
    }

    bool isMember ( string name )
    {
        int val;

        name = stringTransform ( name );
        val = binarySearch ( name, 0, m_register_item_cnt - 1 );
        return ( val >= 0 ) ? true : false;
    }
};

int main ( void )
{
    CVATRegister r;
    //assert ( r.RegisterCompany ( "first Company" ) );
    assert ( r.RegisterCompany ( "         Second     Company        " ) );

    //r.print_sorted ();
    
    //assert ( r.RegisterCompany ( "ThirdCompany, Ltd." ) );
    //assert ( r.RegisterCompany ( "Third Company, Ltd." ) );
    //assert ( !r.RegisterCompany ( "Third Company, Ltd." ) );
    //assert ( !r.RegisterCompany ( " Third  Company,  Ltd.  " ) );
    return 0;
}
