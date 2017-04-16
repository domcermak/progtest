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
    CVATRegister ( void )
    {
        m_reg_size = 100;
        m_reg_items_cnt = 0;
        m_reg_array = new T_Registry [ m_reg_size ];
    }

    bool RegisterCompany ( const string & name )
    {
        // search if !inserted
        for ( unsigned int i = 0; i < m_reg_items_cnt; i++ )
        {
            if ( check_if_inserted ( name ) )
                return false;
        }
        if ( m_reg_items_cnt >= m_reg_size )
            array_realloc ( m_reg_array, m_reg_size );







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
        char * m_reg_name;

        T_Issued * m_iss_array;
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
        arr_size = tmp_array;
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
        arr_size = tmp_array;
        delete arr;
        arr = tmp_array;
    }
    // binary search / sort -----------------------------------------------------
    int qsearch_name ( string name_to_find, 
                       unsigned int from_,
                       unsigned int to_ /* item_cnt - 1 */ )
    {
        int ret_val;
        unsigned int search_pos = ( ( to_ - from_ ) + 1 ) / 2; // size / 2

        ret_val = strcmp ( name_to_find, this->m_reg_array [ search_pos ].m_reg_name );
        if ( !ret_val )
            return search_pos;
        if ( ( ret_val > 0 || ret_val < 0 ) && to_ == from_ )
            return -1;
        ( ret_val > 0 )
            ? return qsearch_name ( name_to_find, search_pos + 1, to_ )
            : return qsearch_name ( name_to_find, from_, search_pos - 1 );
    } // returns -1 if !found

    void qsort_name ( T_Registry *& arr, unsigned int size );

    //---------------------------------------------------------------------------
    bool is_upper_alpha ( char c )
    {
        ( c >= 'A' && c <= 'Z' ) ? return true : return false;
    }

    bool is_space ( char c )
    {
        ( c == " " ) ? return true : return false;
    }
    
    void to_lower_case ( const string str )
    {
        for ( unsigned int i = 0; i < strlen ( str ); i++ )
            if ( is_upper_alpha ( str [ i ] ) )
                str [ i ] += 'a' - 'A';
    }

    void spaces_reduction ( const string str, unsigned int size )
    {
        bool other_noticed = false;
        
        int * spaces = new int [ size - 1 ],
            last_set = -1;

        for ( unsigned int i = 0; i < size - 1; i++ )
        {
            if ( is_space ( str [ i ] )
                && is_space ( str [ i + 1 ] ) )
            {
                spaces [ last_set++ ] = i;
            }
        }
        // todo
    }

    bool check_if_inserted ( const string str )
        // case sensitivity : disabled
        // more spaces : reduce
    {
        unsigned int size = strlen ( str );

        to_lower_case ( str );
        spaces_reduction ( str, size );
        if ( -1 != qsearch_name ( str, 0, this->m_reg_items_cnt - 1 ) )
            return true;
    }
};

int main ( void )
{
    return 0;
}
