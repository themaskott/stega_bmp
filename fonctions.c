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

/*----------------------------------------------------------------*/
/*	 help								   				          */
/* Fonction :	 affiche l aide                                   */
/* Entree :														  */
/* Sortie :														  */
/*----------------------------------------------------------------*/

void print_menu(){

	static const char menu[] = 	"usage : ./stega -c|d fichier_bitmap.bmp\n"
								"\t-c : chiffrer - cacher un texte dans une image\n"
								"\t-d : dechiffrer - recuperer un texte cache dans une image\n"
								"\t-h : help - affiche cette aide\n" ;

	printf(menu) ;
}


/*----------------------------------------------------------------*/
/*	 show_binary								   				  */
/* Fonction :	 affiche un octet sous sa representation binaire  */
/* Entree :														  */
/* Sortie :														  */
/* Fonction utilisee pour le debuggage							  */
/*----------------------------------------------------------------*/

void show_binary( unsigned char c ){

	unsigned char lsb ;
	char binary[8] ;

	for( int i = 0 ; i < 8 ; i++ ){
		lsb = ( c >> i ) & 0x1;
		if( lsb == 0 ){ binary[ 7-i ] = '0' ; }
		else{ binary[ 7-i ] = '1' ; }
	}
	printf("Char : %c | dec : %d | binaire : %s\n", c, c, binary) ;
}
