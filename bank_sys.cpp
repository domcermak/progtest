#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */

class CBank
{
public:
  // default constructor
    CBank ( void )
    {
        m_reg_size = 100;
        m_item_cnt = 0;
        a_reg_array = new T_Register [ m_reg_size ];
    }

  // copy constructor
    CBank ( CBank & obj )
    {
        this->a_reg_array = new T_Register [ obj.m_reg_size ];
        this->m_reg_size = obj.m_reg_size;
        this->m_item_cnt = obj.m_item_cnt;
        
        for ( unsigned int i = 0; i < obj.m_item_cnt; i++ )
        {
            T_Register * reg_this = this->a_reg_array [ i ];
            T_Register * reg_obj = obj.a_reg_array [ i ];

            strcpy ( reg_this->m_acc_id, reg_obj->m_acc_id ); // string
            reg_this->m_init_balance = reg_obj->m_init_balance;
            reg_this->m_actual_balance = reg_obj->m_actual_balance;
            reg_this->a_trans_size = reg_obj->a_trans_size;
            reg_this->a_trans_cnt = reg_obj->a_trans_cnt;

            reg_this->a_trans_array = new T_Transaction [ reg_obj->a_trans_size ];
            for ( unsigned int k = 0; k < reg_obj->a_trans_cnt )
            {
                T_Transaction * trans_this = reg_this->a_trans_array [ k ];
                T_Transaction * trans_obj = reg_obj->a_trans_array [ k ];

                trans_this->m_src_acc_id = new char [ strlen ( trans_obj->m_src_acc_id ) ];
                trans_this->m_dest_acc_id = new char [ strlen ( trans_obj->m_dest_acc_id ) ];
                trans_this->m_sign = new char [ strlen ( trans_obj->m_sign ) ];

                strcpy ( trans_this->m_src_acc_id, trans_obj->m_src_acc_id );
                strcpy ( trans_this->m_dest_acc_id, trans_obj->m_dest_acc_id );
                strcpy ( trans_this->m_sign, trans_obj->m_sign );
                trans_this->m_cash_amount = trans_obj->m_cash_amount;
            }
        }
    }

  // destructor
    ~CBank ( void )
    {
        for (unsigned int i = 0;i<this->m_item_cnt;i++ )
        {
            for ( unsigned int k = 0; this->a_reg_array [ i ].a_trans_size; k++ )
            {
                delete [] this->a_reg_array [ i ].a_trans_array [ k ].m_src_acc_id;
                delete [] this->a_reg_array [ i ].a_trans_array [ k ].m_dest_acc_id;
                delete [] this->a_reg_array [ i ].a_trans_array [ k ].m_sign;
            }
            delete [] this->a_reg_array [ i ].a_trans_array;
            delete [] this->a_reg_array [ i ].m_acc_id;
        }
        delete [] this->a_reg_array;
    }
  
  // operator =
    CBank operator = ( CBank inst_b )
    {
        CBank ret_obj;

        this->a_reg_array = new T_Register [ inst_b.m_reg_size ];
        this->m_item_cnt = inst_b.m_item_cnt;
        this->m_reg_size = inst_b.m_reg_size;

        for ( unsigned int i = 0; i < inst_b.m_reg_size; i++ )
        {
            T_Register * this_reg_array_item, *that_reg_array_item;
            T_Transaction * this_trans_array_item, *that_trans_array_item;

            *this_reg_array_item = this->a_reg_array [ i ];
            *that_reg_array_item = inst_b.a_reg_array [ i ];
            
            this_reg_array_item->a_trans_cnt = that_reg_array_item->a_trans_cnt;
            this_reg_array_item->a_trans_size = that_reg_array_item->a_trans_size;
            strcpy ( this_reg_array_item->m_acc_id, that_reg_array_item->m_acc_id );
            this_reg_array_item->m_actual_balance = that_reg_array_item->m_actual_balance;
            this_reg_array_item->m_init_balance = that_reg_array_item->m_init_balance;

            this->a_reg_array [ i ].a_trans_array = new T_Transaction [ inst_b.a_reg_array [ i ].a_trans_size ];
            for ( unsigned int k = 0; k < inst_b.a_reg_array [ i ].a_trans_size; k++ )
            {
                *this_trans_array_item = this->a_reg_array [ i ].a_trans_array [ k ];
                *that_trans_array_item = inst_b.a_reg_array [ i ].a_trans_array [ k ];

                this_trans_array_item->m_cash_amount = that_trans_array_item->m_cash_amount;
                strcpy ( this_trans_array_item->m_src_acc_id, that_trans_array_item->m_src_acc_id );
                strcpy ( this_trans_array_item->m_dest_acc_id, that_trans_array_item->m_dest_acc_id );
                strcpy ( this_trans_array_item->m_sign, that_trans_array_item->m_sign );
            }
        }
        return *this; // CBank instance
    }
  
    bool NewAccount ( const char * accID,
        int initialBalance )
    {
        T_Register account;

        for ( unsigned int i = 0; i < m_item_cnt; i++ )
        {
            if ( accID == a_reg_array [ i ].m_acc_id )
                return false;
        }

        account.m_acc_id = new char [ strlen ( accID + 1 ) ];
        strcpy ( account.m_acc_id, accID );
        account.m_init_balance = initialBalance;
        account.m_actual_balance = initialBalance;

        if ( m_item_cnt >= m_reg_size )
            array_realloc ( a_reg_array, m_reg_size );
        
        account.a_trans_cnt = 0;
        account.a_trans_size = 10;
        account.a_trans_array = new T_Transaction [ account.a_trans_size ];

        a_reg_array [ m_item_cnt ] = account;
        m_item_cnt++;
        return true;
    }

    bool Transaction ( const char * debAccID, // src
        const char * credAccID, // dest
        int amount,
        const char * signature )
    {
        T_Register * src_account, *dest_account;
        T_Transaction trans_item;
        bool srcAcc_found = false, destAcc_found = false;
        unsigned int srcAcc_position, destAcc_position;

        transaction_check (
            srcAcc_found, destAcc_found, srcAcc_position, destAcc_position, debAccID, credAccID );

        if ( !srcAcc_found 
            || !destAcc_found 
            || srcAcc_position == destAcc_position ) // transfering money itself 
            return false;

        *src_account = ( a_reg_array [ srcAcc_position ] );
        *dest_account = ( a_reg_array [ destAcc_position ] );
        
        if ( src_account->a_trans_size <= src_account->a_trans_cnt )
            array_realloc ( src_account->a_trans_array, src_account->a_trans_size ); // realloc
        if ( dest_account->a_trans_size <= dest_account->a_trans_cnt )
            array_realloc ( dest_account->a_trans_array, dest_account->a_trans_size );

        transaction_join ( 
            trans_item, src_account, dest_account, debAccID, credAccID, amount, signature );
        
        return true;
    }
    
    bool TrimAccount ( const char * accID ) // transactions delete, actual_balance = initial_balance
    {
        for ( unsigned int i = 0; i < m_item_cnt; i++ )
        {
            if ( this->a_reg_array [ i ].m_acc_id == accID )
            {
                delete [] this->a_reg_array [ i ].a_trans_array;
                this->a_reg_array [ i ].a_trans_cnt = 0;
                this->a_reg_array [ i ].a_trans_size = 10;
                this->a_reg_array [ i ].a_trans_array = new T_Transaction [ 10 ];
                this->a_reg_array [ i ].m_actual_balance = this->a_reg_array [ i ].m_init_balance;
                return true;
            }
        }
        return false;
    }

  // Account ( accID )
    T_Register * Account ( const char * accID )
    {

    }

    friend ostream & operator << ( ostream &, T_Register * account )
    {

    }
private:
    struct T_Transaction
    {
        char * m_src_acc_id;
        char * m_dest_acc_id;
        int m_cash_amount;
        char * m_sign;
    };

    struct T_Register
    {
        char * m_acc_id;
        int m_init_balance;
        int m_actual_balance;
        T_Transaction * a_trans_array;
        unsigned int a_trans_size;
        unsigned int a_trans_cnt;
    };

    T_Register * a_reg_array;
    unsigned int m_item_cnt;
    unsigned int m_reg_size;

    // realloc
    void array_realloc ( T_Register *& arr, unsigned int & arr_size ) // universal for T_Register && T_Transaction
    {
        T_Register * sup_array;
        unsigned int sup_size = 2 * m_reg_size;
        sup_array = new T_Register [ sup_size ];

        for ( size_t i = 0; i < arr_size; i++ )
        {
            sup_array [ i ] = arr [ i ];
        }
        delete [] arr;
        arr = sup_array;
        return;
    }

    void array_realloc ( T_Transaction *& arr, unsigned int & arr_size ) // universal for T_Register && T_Transaction
    {
        T_Transaction * sup_array;
        unsigned int sup_size = 2 * m_reg_size;
        sup_array = new T_Transaction [ sup_size ];

        for ( size_t i = 0; i < arr_size; i++ )
        {
            sup_array [ i ] = arr [ i ];
        }
        delete [] arr;
        arr = sup_array;
        return;
    }

    // transaction check if already inserted
    void transaction_check ( bool & srcAcc_found,
        bool & destAcc_found,
        unsigned int & srcAcc_position, 
        unsigned int & destAcc_position, 
        const char *& debAccID,
        const char *& credAccID )
    {
        for ( unsigned int i = 0; i < m_item_cnt; i++ )
        {
            if ( srcAcc_found && destAcc_found ) break;
            if ( ( a_reg_array [ i ] ).m_acc_id == debAccID )
            {
                srcAcc_found = true;
                srcAcc_position = i;
            }
            if ( ( a_reg_array [ i ] ).m_acc_id == credAccID )
            {
                destAcc_found = true;
                destAcc_position = i;
            }
        }
    }

    // values join
    void transaction_join ( T_Transaction & trans_item,
        T_Register *& src_account,
        T_Register *& dest_account,
        const char * debAccID,
        const char * credAccID,
        int amount,
        const char * signature )
    {
        strcpy ( trans_item.m_src_acc_id, debAccID );
        strcpy ( trans_item.m_dest_acc_id, credAccID );
        trans_item.m_cash_amount = amount;
        strcpy ( trans_item.m_sign, signature );

        src_account->a_trans_array [ src_account->a_trans_cnt ] = trans_item;
        dest_account->a_trans_array [ dest_account->a_trans_cnt ] = trans_item;

        src_account->m_actual_balance -= amount;
        dest_account->m_actual_balance += amount;

        src_account->a_trans_cnt++;
        dest_account->a_trans_cnt++;
    }

    void account_sort ( const char * added_account )
    {
        //todo
        return;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
  ostringstream os;
  char accCpy[100], debCpy[100], credCpy[100], signCpy[100];
  CBank x0;

  x0.Account("12345689")
  assert ( x0 . NewAccount ( "123456", 1000 ) );
  assert ( x0 . NewAccount ( "987654", -500 ) );
  assert ( x0 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x0 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x0 . NewAccount ( "111111", 5000 ) );
  assert ( x0 . Transaction ( "111111", "987654", 290, "Okh6e+8rAiuT5=" ) );
  /*assert ( x0 . Account ( "123456" ). Balance ( ) ==  -2190 );
  assert ( x0 . Account ( "987654" ). Balance ( ) ==  2980 );
  assert ( x0 . Account ( "111111" ). Balance ( ) ==  4710 );
  os . str ( "" );
  os << x0 . Account ( "123456" );
  assert ( ! strcmp ( os . str () . c_str (), "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );
  os . str ( "" );
  os << x0 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 290, from: 111111, sign: Okh6e+8rAiuT5=\n = 2980\n" ) );
  os . str ( "" );
  os << x0 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 290, to: 987654, sign: Okh6e+8rAiuT5=\n = 4710\n" ) );
  assert ( x0 . TrimAccount ( "987654" ) );
  assert ( x0 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  os . str ( "" );
  os << x0 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   2980\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 3103\n" ) );

  CBank x2;
  strncpy ( accCpy, "123456", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, 1000 ));
  strncpy ( accCpy, "987654", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, -500 ));
  strncpy ( debCpy, "123456", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "XAbG5uKz6E=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 300, signCpy ) );
  strncpy ( debCpy, "123456", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "AbG5uKz6E=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 2890, signCpy ) );
  strncpy ( accCpy, "111111", sizeof ( accCpy ) );
  assert ( x2 . NewAccount ( accCpy, 5000 ));
  strncpy ( debCpy, "111111", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "Okh6e+8rAiuT5=", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 2890, signCpy ) );
  assert ( x2 . Account ( "123456" ). Balance ( ) ==  -2190 );
  assert ( x2 . Account ( "987654" ). Balance ( ) ==  5580 );
  assert ( x2 . Account ( "111111" ). Balance ( ) ==  2110 );
  os . str ( "" );
  os << x2 . Account ( "123456" );
  assert ( ! strcmp ( os . str () . c_str (), "123456:\n   1000\n - 300, to: 987654, sign: XAbG5uKz6E=\n - 2890, to: 987654, sign: AbG5uKz6E=\n = -2190\n" ) );
  os . str ( "" );
  os << x2 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n = 5580\n" ) );
  os . str ( "" );
  os << x2 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n = 2110\n" ) );
  assert ( x2 . TrimAccount ( "987654" ) );
  strncpy ( debCpy, "111111", sizeof ( debCpy ) );
  strncpy ( credCpy, "987654", sizeof ( credCpy ) );
  strncpy ( signCpy, "asdf78wrnASDT3W", sizeof ( signCpy ) );
  assert ( x2 . Transaction ( debCpy, credCpy, 123, signCpy ) );
  os . str ( "" );
  os << x2 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   5580\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );

  CBank x4;
  assert ( x4 . NewAccount ( "123456", 1000 ) );
  assert ( x4 . NewAccount ( "987654", -500 ) );
  assert ( !x4 . NewAccount ( "123456", 3000 ) );
  assert ( !x4 . Transaction ( "123456", "666", 100, "123nr6dfqkwbv5" ) );
  assert ( !x4 . Transaction ( "666", "123456", 100, "34dGD74JsdfKGH" ) );
  assert ( !x4 . Transaction ( "123456", "123456", 100, "Juaw7Jasdkjb5" ) );
  try
  {
    x4 . Account ( "666" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  try
  {
    os << x4 . Account ( "666" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  assert ( !x4 . TrimAccount ( "666" ) );

  CBank x6;
  assert ( x6 . NewAccount ( "123456", 1000 ) );
  assert ( x6 . NewAccount ( "987654", -500 ) );
  assert ( x6 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x6 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x6 . NewAccount ( "111111", 5000 ) );
  assert ( x6 . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
  CBank x7 ( x6 );
  assert ( x6 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  assert ( x7 . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
  assert ( x6 . NewAccount ( "99999999", 7000 ) );
  assert ( x6 . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" ) );
  assert ( x6 . TrimAccount ( "111111" ) );
  assert ( x6 . Transaction ( "123456", "111111", 221, "Q23wr234ER==" ) );
  os . str ( "" );
  os << x6 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   -1802\n + 221, from: 123456, sign: Q23wr234ER==\n = -1581\n" ) );
  os . str ( "" );
  os << x6 . Account ( "99999999" );
  assert ( ! strcmp ( os . str () . c_str (), "99999999:\n   7000\n + 3789, from: 111111, sign: aher5asdVsAD\n = 10789\n" ) );
  os . str ( "" );
  os << x6 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 123, from: 111111, sign: asdf78wrnASDT3W\n = 5703\n" ) );
  os . str ( "" );
  os << x7 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );
  try
  {
    os << x7 . Account ( "99999999" ). Balance ( );
    assert ( "Missing exception !!" == NULL );
  }
  catch ( ... )
  {
  }
  os . str ( "" );
  os << x7 . Account ( "987654" );
  assert ( ! strcmp ( os . str () . c_str (), "987654:\n   -500\n + 300, from: 123456, sign: XAbG5uKz6E=\n + 2890, from: 123456, sign: AbG5uKz6E=\n + 2890, from: 111111, sign: Okh6e+8rAiuT5=\n + 789, from: 111111, sign: SGDFTYE3sdfsd3W\n = 6369\n" ) );

  CBank x8;
  CBank x9;
  assert ( x8 . NewAccount ( "123456", 1000 ) );
  assert ( x8 . NewAccount ( "987654", -500 ) );
  assert ( x8 . Transaction ( "123456", "987654", 300, "XAbG5uKz6E=" ) );
  assert ( x8 . Transaction ( "123456", "987654", 2890, "AbG5uKz6E=" ) );
  assert ( x8 . NewAccount ( "111111", 5000 ) );
  assert ( x8 . Transaction ( "111111", "987654", 2890, "Okh6e+8rAiuT5=" ) );
  x9 = x8;
  assert ( x8 . Transaction ( "111111", "987654", 123, "asdf78wrnASDT3W" ) );
  assert ( x9 . Transaction ( "111111", "987654", 789, "SGDFTYE3sdfsd3W" ) );
  assert ( x8 . NewAccount ( "99999999", 7000 ) );
  assert ( x8 . Transaction ( "111111", "99999999", 3789, "aher5asdVsAD" ) );
  assert ( x8 . TrimAccount ( "111111" ) );
  os . str ( "" );
  os << x8 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   -1802\n = -1802\n" ) );
  os . str ( "" );
  os << x9 . Account ( "111111" );
  assert ( ! strcmp ( os . str () . c_str (), "111111:\n   5000\n - 2890, to: 987654, sign: Okh6e+8rAiuT5=\n - 789, to: 987654, sign: SGDFTYE3sdfsd3W\n = 1321\n" ) );*/

  return 0;
}
#endif /* __PROGTEST__ */
