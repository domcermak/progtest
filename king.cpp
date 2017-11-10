#include "king.h"

CKing::CKing () : CFigures ()
{
    m_value = INT_MAX;
}

CKing::CKing (const CLanguage * language) : CFigures (language)
{
    m_value = INT_MAX;
}

CKing::~CKing ()
{
}

char CKing::get_name () const
{
    return m_language->get_pack().m_figure_name_list.m_king;
}

void CKing::seek ()
{
    size_t pos_x = this->get_position () [ 0 ] - 'a',
           pos_y = this->get_position () [ 1 ] - '1';
    CFigures * rook = m_table->get_figure ( 7, pos_y );
    
    // 0-0
    if ( !this->is_moved ()
        && this->get_colour () == rook->get_colour ()
        && !rook->is_moved ()
        && !m_table->get_figure ( 5, pos_y )
        && !m_table->get_figure ( 6, pos_y ) )
    {
        bool issue = false;

        for ( size_t i = 0; i < 4; i++ )
            if ( m_table->is_covered ( pos_x + i, pos_y, !this->get_colour () ) ) issue = true;
        if ( !issue ) this->add_possible_move ( "0-0" );
    }

    // 0-0-0
    rook = m_table->get_figure ( 0, pos_y ); // rook changed
    if ( !this->is_moved ()
        && this->get_colour () == rook->get_colour ()
        && !rook->is_moved ()
        && !m_table->get_figure ( 1, pos_y )
        && !m_table->get_figure ( 2, pos_y )
        && !m_table->get_figure ( 3, pos_y ) )
    {
        bool issue = false;

        for ( size_t i = 0; i < 5; i++ )
            if ( m_table->is_covered ( pos_x - i, pos_y, !this->get_colour () ) ) issue = true;
        if ( !issue ) this->add_possible_move ( "0-0-0" );
    }

    // normal moves ( !allowed attacked possitions & covered figures )
    this->step ( pos_x, pos_y + 1 );
    this->step ( pos_x, ( int ) pos_y - 1 );
    this->step ( pos_x + 1, pos_y );
    this->step ( ( int ) pos_x - 1, pos_y );
    this->step ( pos_x + 1, pos_y + 1 );
    this->step ( ( int ) pos_x - 1, pos_y + 1 );
    this->step ( ( int ) pos_x - 1, ( int ) pos_y - 1 );
    this->step ( pos_x + 1, ( int ) pos_y - 1 );
}

void CKing::step ( int _x, int _y )
{
    CFigures * figure;
    string possible_move;

    if ( _x < 0 || _x > 7 || _y < 0 || _y > 7 ) return;

    figure = m_table->get_figure ( _x, _y );
    possible_move.push_back ( _x + 'a' );
    possible_move.push_back ( _y + '1' );

    if ( m_table->is_occupied ( _x, _y ) 
        && figure->get_colour () != this->get_colour () 
        && !m_table->is_covered ( _x, _y, !this->get_colour () ) ) this->add_possible_move ( possible_move );
    
    if ( !m_table->is_occupied ( _x, _y ) 
        && !m_table->is_covered ( _x, _y, !this->get_colour () ) ) this->add_possible_move ( possible_move );
}
