#include "table.h"
#include "file_save.h"
#include "language.h"

#define DATABASE_PATH "database/"

int main ( int argc, char ** argv )
{
    CTable table;
    CFile _file ( DATABASE_PATH );
    
    switch ( argc )
    {
    case 1: // no file in argument 
        break;
            
    case 2: // one file with argument with gameplay from the begining
        try { table.preplay ( argv ); }
        catch ( CException & e ) { e.print (); NEWLINE; exit ( EXIT_FAILURE ); }
        table.display ();
        break;
    
    case 3: // two files only with positions of figures separately
        try { table.preset ( argv ); }
        catch ( CException & e ) { e.print (); NEWLINE; exit ( EXIT_FAILURE ); }
        table.display ();
        break;
    
    default: // unsupported count of files in argument
        CArgument_overflow ().print ();
        NEWLINE;
        exit ( EXIT_FAILURE );
    }

    // user gameplay begin
    system ( "clear" );
    table.switch_display ( true ); // for testing is now off
    table.display ();
    table.find_possible_moves ();

    while ( true )
    {
//        const TText txt = lang.get_pack ().m_text_list;

        ( table.on_move () ) 
            ? CNext_turn ( &table).print ()
            : CNext_turn ( &table).print ();

        try
        { 
            table.checkmate ();
            table.check (); 
            table.do_move ( table.load () );
        }
        catch ( CCheckmate & e ) { e.print (); CQuit ().print (&lang); break; }
        catch ( CCheck & e ) { e.print (); }
        catch ( CQuit & e ) { e.print (); break; }
//        catch ( CLanguage & e ) { lang.choose_language ( e.get_name () ); continue; }
        catch ( bool & e ) { table.switch_display ( e ); CDisplay ( e ).print (); continue; }
        catch ( CPlayed_turns & e ) { e.print (); continue; } // ERROR
        catch ( CException & e ) { e.print (); continue; }
        
        system ( "clear" );
        table.display ();
        table.find_possible_moves ();
    }

    if ( table.get_moves_list ().size () /*&& !_file.is_duplicit_file ( table.get_moves_list () )*/ )
        _file.save ( table.get_moves_list () );

    exit ( EXIT_SUCCESS );
}
