#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>

using namespace std;
#endif /* __PROGTEST_ */

class CWeight
{
public:
    CWeight ( const int weight ) : m_weight ( weight ) { }
    virtual ~CWeight ( void )
    {
    }
    virtual void Print ( ostream & os ) const = 0;
    virtual CWeight * Copy ( void ) const = 0;
    
    int Weight ( void ) const
    {
        return m_weight;
    }

    virtual bool Danger ( void ) const
    {
        return false;
    }
protected:
    int m_weight;
};

class CKnife : public CWeight
{
public:
    CKnife ( const int bladeLength ) : CWeight ( 100 ), m_bladeLength ( bladeLength ) { }

    bool operator == ( const CKnife & item ) const
    {
        return ( this->m_bladeLength == item.m_bladeLength ) ? true : false;
    }

    void Print ( ostream & os ) const
    {
        os << "Knife, blade: " << m_bladeLength << " cm";
    }
    
    CWeight * Copy ( void ) const
    {
        CKnife * c;
        c = new CKnife ( m_bladeLength );
        return c;
    }

    bool Danger ( void ) const
    {
        return ( m_bladeLength > 7 ) ? true : false;
    }
protected:
    int m_bladeLength;
};

class CClothes : public CWeight
{
public:
    CClothes ( const string description ) : CWeight ( 500 ), m_description ( description ) { }
    ~CClothes ( void )
    {
        m_description.clear ();
    }
    
    bool operator == ( const CClothes & item ) const
    {
        return ( this->m_description == item.m_description ) ? true : false;
    }

    void Print ( ostream & os ) const
    {
        os << "Clothes (" << m_description << ")";
    }

    CWeight * Copy ( void ) const
    {
        CClothes * c;
        c = new CClothes ( m_description );
        return c;
    }
protected:
    string m_description;
};

class CShoes : public CWeight
{
public:
    CShoes ( void ) : CWeight ( 750 ) { }
    void Print ( ostream & os ) const
    {
        os << "Shoes";
    }

    bool operator == ( const CShoes & item ) const
    {
        if ( this->m_weight == item.m_weight ) return true;
        else return true;
    }

    CWeight * Copy ( void ) const
    {
        CShoes * c;
        c = new CShoes ();
        return c;
    }
};

class CMobile : public CWeight
{
public:
    CMobile ( const string manufacturer, const string model )
        : CWeight ( 150 ), m_manufacturer ( manufacturer ), m_model ( model ) { }
    ~CMobile ( void )
    {
        m_manufacturer.clear ();
        m_model.clear ();
    }

    bool operator == ( const CMobile & item ) const
    {
        return ( this->m_manufacturer == item.m_manufacturer && this->m_model == item.m_model ) ? true : false;
    }

    void Print ( ostream & os ) const
    {
        os << "Mobile " << m_model << " by: " << m_manufacturer;
    }

    CWeight * Copy ( void ) const
    {
        CMobile * c;
        c = new CMobile ( m_manufacturer, m_model );
        return c;
    }

    bool Danger ( void ) const
    {
        return ( "Samsung" == m_manufacturer && "Galaxy Note S7" == m_model ) ? true : false;
    }
protected:
    string m_manufacturer;
    string m_model;
};

class CLuggage
{
public:
    CLuggage ( const int weight ) : m_weight ( weight )
    {
    }

    CLuggage ( const CLuggage & item )
    {
        for ( unsigned int i = 0; i < item.v_list.size (); i++ )
        {
            v_list.push_back ( item.v_list [ i ]->Copy () );
        }
        m_weight = item.m_weight;
    }

    virtual ~CLuggage ( void )
    {
        for ( unsigned int i = 0; i < v_list.size (); i++ )
            delete v_list [ i ];
        v_list.clear ();
    }

    CLuggage & operator = ( const CLuggage & item )
    {
        if ( this == &item ) return *this;
        for ( unsigned int i = 0; i < v_list.size (); i++ )
            delete v_list [ i ];
        v_list.clear ();
        for ( unsigned int i = 0; i < item.v_list.size (); i++ )
        {
            v_list.push_back ( item.v_list [ i ]->Copy () );
        }
        m_weight = item.m_weight;
        return *this;
    }
 
    CLuggage & Add ( const CWeight & item )
    {
        v_list.push_back ( item.Copy () );
        return *this;
    }
    
    int Weight ( void ) const
    {
        int total = m_weight;

        for ( unsigned int i = 0; i < v_list.size (); i++ )
        {
            total += v_list [ i ]->Weight ();
        }
        return total;
    }

    int Count ( void ) const
    {
        return v_list.size ();
    }
    
    bool Danger ( void ) const  
    {
        for ( unsigned int i = 0; i < v_list.size (); i++ )
        {
            if ( v_list [ i ]->Danger () )
                return true;
        }
        return false;
    }

    bool IdenticalContents ( const CLuggage & lugg ) const // todo
    {
        if ( this->v_list.size () != lugg.v_list.size () )
            return false;
     
        for ( unsigned int i = 0; i < v_list.size (); i++ ) 
        {
            bool found = false;

            for ( unsigned int k = 0; k < v_list.size (); k++ )
            {
                if ( dynamic_cast < CKnife* > ( v_list [ i ] ) != NULL )
                    if ( dynamic_cast < CKnife* > ( lugg.v_list [ k ] ) != NULL )
                        if ( *( CKnife* ) v_list [ i ] == *( CKnife* ) lugg.v_list [ k ] )
                        {
                            found = true;
                            break;
                        }
                if ( dynamic_cast < CClothes* > ( v_list [ i ] ) != NULL )
                    if ( dynamic_cast < CClothes* > ( lugg.v_list [ k ] ) != NULL )
                        if ( *( CClothes* ) v_list [ i ] == *( CClothes* ) lugg.v_list [ k ] )
                        {
                            found = true;
                            break;
                        }
                if ( dynamic_cast < CShoes* > ( v_list [ i ] ) != NULL )
                    if ( dynamic_cast < CShoes* > ( lugg.v_list [ k ] ) != NULL )
                        if ( *( CShoes* ) v_list [ i ] == *( CShoes* ) lugg.v_list [ k ] )
                        {
                            found = true;
                            break;
                        }
                if ( dynamic_cast < CMobile* > ( v_list [ i ] ) != NULL )
                    if ( dynamic_cast < CMobile* > ( lugg.v_list [ k ] ) != NULL )
                        if ( *( CMobile* ) v_list [ i ] == *( CMobile* ) lugg.v_list [ k ] )
                        {
                            found = true;
                            break;
                        }
            }
            if ( !found )
                return false;
        }
        return true;
    }

    virtual void Head ( ostream & os ) const = 0;
    
    friend ostream & operator << ( ostream & os, const CLuggage & item )
    {
        item.Head ( os );
        for ( unsigned int i = 0; i < item.v_list.size (); i++ )
        {
            os << ( ( i == item.v_list.size () - 1 ) ? "\\-" : "+-" );
            item.v_list [ i ]->Print ( os );
            os << endl;
        }
        return os;
    }
protected:
    int m_weight;
    vector <CWeight*> v_list;
};
  
class CSuitcase : public CLuggage
{
public:
    CSuitcase ( const int width, const int height, const int depth ) : CLuggage ( 2000 ), m_width ( width ), m_height ( height ), m_depth ( depth )
    {
    }
    ~CSuitcase ( void ) { }
    void Head ( ostream & os ) const
    {
        os << "Suitcase " << m_width << "x" << m_height << "x" << m_depth << endl;
    }
protected:
    int m_width;
    int m_height;
    int m_depth;
};

class CBackpack : public CLuggage
{
public:
    CBackpack ( void ) : CLuggage ( 1000 ) { }
    ~CBackpack ( void )
    {
    }
    
    void Head ( ostream & os ) const
    {
        os << "Backpack" << endl;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
    CSuitcase x ( 1, 2, 3 );
    CBackpack y;
    ostringstream os;
    x . Add ( CKnife ( 7 ) );
    x . Add ( CClothes ( "red T-shirt" ) );
    x . Add ( CClothes ( "black hat" ) );
    x . Add ( CShoes () );
    x . Add ( CClothes ( "green pants" ) );
    x . Add ( CClothes ( "blue jeans" ) );
    x . Add ( CMobile ( "Samsung", "J3" ) );
    x . Add ( CMobile ( "Tamtung", "Galaxy Note S7" ) );
    os . str ( "" );
    os << x;
    assert ( os . str () == "Suitcase 1x2x3\n"
             "+-Knife, blade: 7 cm\n"
             "+-Clothes (red T-shirt)\n"
             "+-Clothes (black hat)\n"
             "+-Shoes\n"
             "+-Clothes (green pants)\n"
             "+-Clothes (blue jeans)\n"
             "+-Mobile J3 by: Samsung\n"
             "\\-Mobile Galaxy Note S7 by: Tamtung\n" );
    assert ( x . Count () == 8 );
    assert ( x . Weight () == 5150 );
    assert ( !x . Danger () );
    x . Add ( CKnife ( 8 ) );
    os . str ( "" );
    os << x;
    assert ( os . str () == "Suitcase 1x2x3\n"
             "+-Knife, blade: 7 cm\n"
             "+-Clothes (red T-shirt)\n"
             "+-Clothes (black hat)\n"
             "+-Shoes\n"
             "+-Clothes (green pants)\n"
             "+-Clothes (blue jeans)\n"
             "+-Mobile J3 by: Samsung\n"
             "+-Mobile Galaxy Note S7 by: Tamtung\n"
             "\\-Knife, blade: 8 cm\n" );
    assert ( x . Count () == 9 );
    assert ( x . Weight () == 5250 );
    assert ( x . Danger () );
    y . Add ( CKnife ( 7 ) )
      . Add ( CClothes ( "red T-shirt" ) )
      . Add ( CShoes () );
    y . Add ( CMobile ( "Samsung", "Galaxy Note S7" ) );
    y . Add ( CShoes () );
    y . Add ( CClothes ( "blue jeans" ) );
    y . Add ( CClothes ( "black hat" ) );
    y . Add ( CClothes ( "green pants" ) );
    os . str ( "" );
    os << y;
    assert ( os . str () == "Backpack\n"
             "+-Knife, blade: 7 cm\n"
             "+-Clothes (red T-shirt)\n"
             "+-Shoes\n"
             "+-Mobile Galaxy Note S7 by: Samsung\n"
             "+-Shoes\n"
             "+-Clothes (blue jeans)\n"
             "+-Clothes (black hat)\n"
             "\\-Clothes (green pants)\n" );
    assert ( y . Count () == 8 );
    assert ( y . Weight () == 4750 );
    assert ( y . Danger () );
    y . Add ( CMobile ( "Samsung", "J3" ) );
    y . Add ( CMobile ( "Tamtung", "Galaxy Note S7" ) );
    y . Add ( CKnife ( 8 ) );
    os . str ( "" );
    os << y;
    assert ( os . str () == "Backpack\n"
             "+-Knife, blade: 7 cm\n"
             "+-Clothes (red T-shirt)\n"
             "+-Shoes\n"
             "+-Mobile Galaxy Note S7 by: Samsung\n"
             "+-Shoes\n"
             "+-Clothes (blue jeans)\n"
             "+-Clothes (black hat)\n"
             "+-Clothes (green pants)\n"
             "+-Mobile J3 by: Samsung\n"
             "+-Mobile Galaxy Note S7 by: Tamtung\n"
             "\\-Knife, blade: 8 cm\n" );
    assert ( y . Count () == 11 );
    assert ( y . Weight () == 5150 );
    assert ( y . Danger () );
    assert ( !x . IdenticalContents ( y ) );
    assert ( !y . IdenticalContents ( x ) );
    x . Add ( CMobile ( "Samsung", "Galaxy Note S7" ) );
    assert ( !x . IdenticalContents ( y ) );
    assert ( !y . IdenticalContents ( x ) );
    x . Add ( CShoes () );
    assert ( x . IdenticalContents ( y ) );
    assert ( y . IdenticalContents ( x ) );
    assert ( y . IdenticalContents ( y ) );
    assert ( x . IdenticalContents ( x ) );
    return 0;
}
#endif /* __PROGTEST__ */
