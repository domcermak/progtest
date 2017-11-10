#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <cstdlib>
#include <fstream>
#include <cerrno>
#include <thread>
#include <pthread.h>

#include "language.h"
#include "exeptions.h"
#include "moves.h"
#include "bishop.h"
#include "king.h"
#include "pawn_black.h"
#include "pawn_white.h"
#include "queen.h"
#include "rook.h"
#include "knight.h"
#include "language.h"

#define isnum(c) ( c >= '1' && c <= '8' )
#define issalpha(c) ( c >= 'a' && c <= 'h' )
#define NEWLINE cout << endl

//Standart chess 8x8 table
class CTable
{
public:
    //Constructor
    CTable ( const CLanguage * language_pack = 0 );

    //Destructor
    ~CTable ();

    //Load data from user
    std::string load ();

    /**
    *Start game with turns in file
    *@param position_list 1 file with full gameplay turns
    */
    void preplay ( char ** position_list );

    /**
    *Set figure positions from file
    *@param position_list 2 files with figure positions
    */
    void preset ( char ** position_list );

    /**
    *Return figure on _x _y croft, NULL if there is none 
    *@param _x X position of figure on table
    *@param _y Y position of figure on table
    */
    CFigures * get_figure ( size_t _x, size_t _y ) const;

    /**
    *Return true if there is a figure on a croft
    *@param _x X position of croft on table
    *@param _y Y position of croft on table
    */
    bool is_occupied ( size_t _x, size_t _y ) const;

    //Return true if white is on move
    bool on_move () const;

    /**
    *Does turn with figure on the table
    *@param move Instruction data in string
    *@param x_included Boolean value, if string was shorted and 'x' was included
    */
    bool do_move ( const std::string move, const bool x_included = false );

    //check if king is in checkmate
    void checkmate () const;

    //Throw exeption if king on move is in check
    void check () const;

    /**
    *Throw exeption if king on move is in check
    *@param king Position of the king that might be in check
    */
    template <typename T>
    void check ( const T * king ) const;

    /**
    *Throw exeption if king on move is in check
    *@param king Position of the king that might be in check
    *@param figure_pos A variable which gets value of position of figure attacking king
    */
    template <typename T>
    void check ( const T * king, std::string & figure_pos ) const;

    //Return number of moves played by single player
    size_t player_moves_count () const;

    
    //Return reference to stack of played moves
    CMoves & get_moves_list ();

    //Find possible moves for all figures
    void find_possible_moves ();

    /**
    *Return true if figure is covered by ally figure
    *@param _x Value of position
    *@param _y Value of position
    *@param colour A colour of figure, which should cover the croft
    */
    bool is_covered ( const size_t _x, const size_t _y, const bool colour ) const;

    /**
     *Function in cooperation with display() method
     *@param statement Boolean value that sets m_display variable
     */
    void switch_display ( const bool statement );

    //Display table depending on m_display value
    void display () const;

    //Load array with language pack names from database folder
    void fill_language_pack ();

    //Clear table
    void clear ();
private:
    //Player on turn/move
    bool m_on_move;

    //Table memory for figures
    std::vector <std::vector <CFigures*>> m_table;

    //Names of all figures
    std::vector <char> m_figure_name_list;

    std::vector <std::string> m_preplay_data;

    //Moves by white player loaded from external file
    std::vector <std::string> m_preset_data_white;

    //Moves by black player loaded from externa file
    std::vector <std::string> m_preset_data_black;

    //Storage of played turns
    CMoves m_stack;

    //Statement if table should be graphicaly displayed
    bool m_display;

    //Pointer to const language pack
    const CLanguage * m_language;

    //Set default layout of chess table
    void set_default ();

    /**
    *Save move into stack
    *@param move A string-type move
    */
    void save_move ( const std::string move );

    //Delete spaces in string
    void space_delete ( std::string & str );

    /**
    *Return 1 if it found any figure to go between king and enemy bishop
    *Return 0 if it reaches bishop
    *Return -1 if it needs next search 
    *@param move Position of a croft between king and bishop
    */
    int find_figure_to_go_between ( const std::string & move, const std::string figure_position ) const;

    /**
    *Throw exeption if there is any multiple turn chance
    *@param str Turn given by user
    */
    void multiple_turn_possibility ( const std::string & str ) const;
    
    // Return reference to language pack with default (en) strings
    CLanguage & getDefaultLanguagePack ();
};
