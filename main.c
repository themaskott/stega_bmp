//
//  main.c
//  Stega
//
//  @Maskott
//  15/11/2021.
//

/*---------------------------------------------*/
/*             INCLUDE LANGAGE C               */
/*---------------------------------------------*/

#include "librairies.h"


/*---------------------------------------------*/
/*     INCLUDE PERSONNELS   - prototypes       */
/*---------------------------------------------*/

#include "public.h"
#include "fonctions.h"
#include "bitmap.h"
#include "text_hiding.h"


/*---------------------------------------------*/
/* PROGRAMME PRINCIPAL                         */
/*---------------------------------------------*/


int main( int argc, char * argv[] ){

    int option_index = 0 ;
    bitmap * fichier_bitmap = NULL ;

    if( argc == 1){
        print_menu() ;
        exit(0) ;
    }

    while( (option_index = getopt( argc, argv, "c:d:h")) != -1 ){
      switch ( option_index ) {
        case 'c':
            fichier_bitmap = parse_bitmap( optarg ) ;
            cipher( fichier_bitmap ) ;
            release( fichier_bitmap ) ;
            break ;
        case 'd':
            fichier_bitmap = parse_bitmap( optarg ) ;
            decipher( fichier_bitmap ) ;
            release( fichier_bitmap ) ;
            break ;
        case 'h':
            print_menu() ;
            break ;
        default:
            print_menu() ;
            break ;
        }
    }
    return 0 ;
}// main
