//
//	fonctions.c
//	Steaga
//
//	Created by Maskott on 15/11/2021.
//

/*---------------------------------------------*/
/*				 INCLUDE LANGAGE C			   */
/*---------------------------------------------*/
#include "librairies.h"

/*---------------------------------------------*/
/*		 INCLUDE PERSONNELS	 - prototypes	   */
/*---------------------------------------------*/

#include "public.h"
#include "fonctions.h"
#include "bitmap.h"



/*--------------------------------------------------------------------*/
/*	 check_img_bmp		    										  */
/* Fonction : verifie que l image est bien un bitmap                  */
/* Entree :	 descripteur de fichier ouvert							  */
/* Sortie :	l offset sous forme d entier						      */
/*--------------------------------------------------------------------*/

unsigned int check_img_bmp( FILE * fichier ){

    unsigned char buff = 0 ;
    unsigned int magic_number = 0 ;

	fseek( fichier, 0, SEEK_SET ) ;

	// recupere les deux premiers octets
	buff = fgetc( fichier ) ;
	magic_number = magic_number | buff ;
	magic_number = magic_number << 8 ;
	buff = fgetc( fichier ) ;
	magic_number = magic_number | buff ;

	if( magic_number != 0x424d ){
		printf("Format de fichier incorrect, veuillez choisir une imgae bitmap\n" ) ;
		fclose( fichier ) ;
		exit(-1) ;
	}

	return magic_number ;
}

/*--------------------------------------------------------------------*/
/*	 get_bmp_offset		    										  */
/* Fonction :	 recherche l offset de debut d image dans un bitmap   */
/* Entree :	 descripteur de fichier ouvert							  */
/* Sortie :	l offset sous forme d entier						      */
/*--------------------------------------------------------------------*/

unsigned long int get_bmp_offset( FILE * fichier ){

    unsigned char buff = 0 ;
    unsigned long int offset = 0 ;

	fseek( fichier, 0, SEEK_SET) ;

    // passe les 10 premiers octets
    for(int i = 0 ; i < 10 ; i++){
        buff = fgetc( fichier ) ;
    }

    // recupere 4 octets en little endian = offset
    for(int i = 0 ; i < 4 ; i++ ){

        buff = fgetc( fichier ) ;
        offset = offset | ( buff << 8 * i ) ;
    }

    return offset ;
}

/*--------------------------------------------------------------------*/
/*	 parse_bitmap		    										  */
/* Fonction :	 instancie un bitmap, parse les infos du fichier      */
/* Entree :	 nom du fichier bitmap       							  */
/* Sortie :	pointeur vers la structure instancie				      */
/*--------------------------------------------------------------------*/

bitmap * parse_bitmap( char * file_name ){

	FILE * fichier_image = fopen( file_name, "rb" ) ;
	if( fichier_image == NULL ){
		printf("[-] Erreur a l ouverture du fichier\n" ) ;
		exit(-1) ;
	}

	bitmap * pImage = (bitmap *) malloc( sizeof( bitmap ) ) ;

	pImage->magic_cookie = check_img_bmp( fichier_image ) ;

	pImage->file_name = (char *) malloc( sizeof(char) * strlen( file_name ) + 1 ) ;
	strcpy( pImage->file_name, file_name ) ;

	pImage->data_offset = get_bmp_offset( fichier_image ) ;

	fclose( fichier_image ) ;
	return pImage ;
}

/*--------------------------------------------------------------------*/
/*	 release         	    										  */
/* Fonction : desaloue les zones memoire et relache les pointeurs     */
/* Entree :	 pointeur vers la structure a relacher					  */
/* Sortie :	                            						      */
/*--------------------------------------------------------------------*/

void release( bitmap * fichier_image ){
	free( fichier_image->file_name ) ;
	free (fichier_image ) ;
	fichier_image = NULL ;
}
