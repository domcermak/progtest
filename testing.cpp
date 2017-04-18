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
    CVATRegister ( void ) : m_reg_size ( 100 ), m_reg_items_cnt ( 0 )
    {
        m_reg_array = new T_Registry [ m_reg_size ];
    }

    bool RegisterCompany ( const string & name )
    {
        if ( check_if_inserted ( name.c_str () ) )
            return false;
        if ( m_reg_items_cnt >= m_reg_size )
            array_realloc ( m_reg_array, m_reg_size );
        
        strcpy ( this->m_reg_array [ m_reg_items_cnt ].m_original, name.c_str () );
        m_reg_items_cnt++;
        qsort_name ( m_reg_array, m_reg_items_cnt );
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
private:
    struct T_Issued
    {

    };

    struct T_Registry
    {
        char * m_original; // original input
        char * reduced; // prepared to search

        T_Issued * m_iss_array; // issued
        unsigned int m_iss_items_cnt;
        unsigned int m_iss_size;
    };

    T_Registry * m_reg_array;
    unsigned int m_reg_size;
    unsigned int m_reg_items_cnt;

    // realloc ---------------------------------------------------------------------------
    void array_realloc ( T_Registry *& arr, unsigned int & arr_size )
    {
        T_Registry * tmp_array;
        unsigned int tmp_size = 2 * arr_size;
        for ( unsigned int i = 0; i < arr_size; i++ )
        {
            tmp_array [ i ] = arr [ i ];
        }
        arr_size = tmp_size;
        delete arr;
        arr = tmp_array;
    }

    void array_realloc ( T_Issued *& arr, unsigned int & arr_size )
    {
        T_Issued * tmp_array;
        unsigned int tmp_size = 2 * arr_size;
        for ( unsigned int i = 0; i < arr_size; i++ )
        {
            tmp_array [ i ] = arr [ i ];
        }
        arr_size = tmp_size;
        delete arr;
        arr = tmp_array;
    }
    // binary search / sort -----------------------------------------------------
    // returns -1 if !found
    int qsearch_name ( char * name_to_find, 
                       unsigned int from_,
                       unsigned int to_ ) // to_ = items_cnt - 1
    {
        int ret_val;
        unsigned int search_pos = ( ( to_ - from_ ) + 1 ) / 2; // size / 2

        ret_val = strcmp ( name_to_find, this->m_reg_array [ search_pos ].reduced );
        if ( !ret_val )
            return search_pos;
        if ( ( ret_val > 0 || ret_val < 0 ) && to_ == from_ )
            return -1;
        
         return ( ret_val > 0 )
            ? qsearch_name ( name_to_find, search_pos + 1, to_ )
            : qsearch_name ( name_to_find, from_, search_pos - 1 );
    } 

    void qsort_name ( T_Registry *& arr, unsigned int size )
    {
        unsigned int pivot = size - 2;

        // todo
    }

    //---------------------------------------------------------------------------
    bool is_upper_alpha ( char c )
    {
        return ( c >= 'A' && c <= 'Z' ) ? true : false;
    }

    bool is_space ( char c )
    {
        return ( c == ' ' ) ? true : false;
    }
    
    void to_lower_case (  char *& str )
    {
        for ( unsigned int i = 0; i < strlen ( str ); i++ )
            if ( is_upper_alpha ( str [ i ] ) )
                str [ i ] += 'a' - 'A';
    }

    void spaces_reduction ( char *& str, unsigned int & size )
    {
        unsigned int i = 0;
        char * tmp;

        while ( is_space ( str [ i ] ) )
            i++;

        // spaces before
        if ( !i )
        {
            size -= i;
            tmp = new char [ size ];
            for ( unsigned int k = i; k < size; k++ )
                tmp [ k - i ] = str [ k ];
            delete str;
            str = tmp;
        }

        // spaces between
        for ( unsigned int k = 0; k < size; k++ )
        {
            if ( is_space ( str [ k ] ) 
                && is_space ( str [ k + 1 ] ) )
            {
                char * tmp = new char [ size-- ];
                for ( unsigned int f = 0; f < size; f++ )
                    tmp [ f ] = ( f < k ) ? str [ f ] : str [ f + 1 ];
                delete str;
                str = tmp;
            }
        }

        // spaces after
        if ( is_space ( str [ size - 1 ] ) )
        {
            char * tmp = new char [ size-- ];
            for ( unsigned int k = 0; k < size; k++ )
                tmp [ k ] = str [ k ];
            delete str;
            str = tmp;
        }
    }

    bool check_if_inserted ( const char * str )
        // case sensitivity : disabled
        // more spaces : reduce
    {
        char * non_const_str;
        unsigned int size = strlen ( str );

        non_const_str = new char [ size ];
        strcpy ( non_const_str, str );
        to_lower_case ( non_const_str );
        spaces_reduction ( non_const_str, size );
        
        if ( -1 != qsearch_name ( non_const_str, 0, this->m_reg_items_cnt - 1 ) )
            return true;

        strcpy ( this->m_reg_array [ m_reg_items_cnt ].reduced, non_const_str );
        return false;
    }
};

int main ( void )
{
    return 0;
}
