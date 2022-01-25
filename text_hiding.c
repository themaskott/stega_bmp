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
#include "text_hiding.h"


/*--------------------------------------------------------------------*/
/*	 cipher														      */
/* Fonction :	 cache un texte saisi par l utilisateur dans une img  */
/* Entree :	 nom de fichier ouvert	         						  */
/* Sortie :														      */
/*--------------------------------------------------------------------*/


void cipher( bitmap * fichier_image ){

	unsigned char buff, lsb ;
	char secret_message[ MESSAGE_MAX_LENGTH ] ;

	// preparation du fichier destination
    printf("[+] Creation du fichier de destination\n") ;

	size_t full_size ;
	char * prefix = "new_" ;
	full_size = strlen( prefix ) + strlen( fichier_image->file_name ) + 1 ;

	char * dest_file_name = (char *) malloc( full_size ) ;

	strcpy( dest_file_name, prefix ) ;
	strcat( dest_file_name, fichier_image->file_name ) ;

	// ouverture des fichiers
    FILE * source_file = fopen( fichier_image->file_name, "rb" ) ;
	FILE * dest_file = fopen( dest_file_name, "wb") ;

	if ( source_file == NULL || dest_file == NULL ){
         printf("\tProbleme a l ouverture du fichier\n") ;
         exit(-1) ;
    }
	printf( "\tFichier de sortie : %s\n", dest_file_name ) ;


	// analyse bitmap pour recuperer offset
	printf("[+] Offset d ecriture\n") ;
	printf("\tOffset : 0x%lx\n", fichier_image->data_offset ) ;

	int i = 0 ;
	unsigned char red_byte = 0 ;
	unsigned char green_byte = 0 ;
	unsigned char blue_byte = 0 ;


	// recuperation du message a cacher
	printf( "[+] Quel est votre message :\n") ;

	do{
		buff = getchar() ;
		secret_message[ i ] = buff ;
		i++ ;
	}while( buff != '\n' && i < MESSAGE_MAX_LENGTH - 1);

    // i-1 pour ecraser le \n par \0
	secret_message[ i - 1 ] = '\0' ;

	unsigned char msg_size = strlen( secret_message ) ;

	// le debut du fichier est recopie tel quel (entetes)
	for( i = 0 ; i < fichier_image->data_offset ; i++ ){
		fputc( fgetc( source_file ), dest_file ) ;
	}

	// 1er octet insere = taille du message
	// msg_size est insere de son LSB vers son MSB
	// on ecrit que dans les octets R ( 1 pixel = RGB )
	for( int j = 0 ; j < 8 ; j++ ){

		red_byte = fgetc( source_file ) ;
		green_byte = fgetc( source_file ) ;
		blue_byte = fgetc( source_file ) ;

		lsb = ( msg_size >> j ) & 0x1 ;

		// on elenve le LSB de l octet a ecrire
		red_byte = ( red_byte >> 1 ) << 1 ;
		fputc( ( red_byte | lsb ), dest_file ) ;
		fputc( green_byte, dest_file ) ;
		fputc( blue_byte, dest_file ) ;

	}

	// insersion du message dans les bits de poids faibles
	// chaque octet du message est insere de son LSB vers son MSB

	for( i = 0 ; i < msg_size ; i++ ){

		//printf("Lettre : %c\n", secret_message[i] ) ;
		for( int j = 0 ; j < 8 ; j++ ){

			red_byte = fgetc( source_file ) ;
			green_byte = fgetc( source_file ) ;
			blue_byte = fgetc( source_file ) ;

			lsb = ( secret_message[i] >> j ) & 0x1 ;

			red_byte = ( red_byte >> 1 ) << 1 ;
			fputc( ( red_byte | lsb ), dest_file ) ;
			fputc( green_byte, dest_file ) ;
			fputc( blue_byte, dest_file ) ;

		}
	}

	// fin du fichier telle quelle

	while( !feof( source_file ) ){
        buff = fgetc( source_file ) ;
		fputc( buff, dest_file ) ;
	}

	printf( "[+] Informations :\n") ;
	printf( "\tMessage cache : %s\n", secret_message ) ;
	printf( "\tTaille du message : %d\n", msg_size ) ;

	fclose( source_file ) ;
	fclose( dest_file ) ;
	free( dest_file_name ) ;
	dest_file_name = NULL ;
}


/*-----------------------------------------------------------------*/
/*	 decipher													   */
/* Fonction :	 recupere un texte dans une img					   */
/* Entree :	 nom de fichier                						   */
/* Sortie :														   */
/*-----------------------------------------------------------------*/


void decipher( bitmap * fichier_image ){


	unsigned char lsb = 0, msg_size = 0 ;
	unsigned char secret_message[ MESSAGE_MAX_LENGTH ] = {0} ;
	int i = 0 ;


    // analyse bitmap pour recuperer offset
    printf("[+] Offset de lecture\n") ;
    unsigned long int offset = fichier_image->data_offset ;
    printf("\tOffset : 0x%lx\n", offset) ;


    printf("[+] Ouverture du fichier source\n") ;
	FILE * source_file ;
	source_file = fopen( fichier_image->file_name, "rb" ) ;
    if ( source_file == NULL ){
         printf("\nProbleme a l ouverture du fichier\n") ;
         exit(-1) ;
    }

	unsigned char buff ;
	unsigned char red_byte = 0 ;
	unsigned char green_byte = 0  ;
	unsigned char blue_byte = 0 ;

	// les entetes sont ignorees
	for( i = 0 ; i < offset ; i++ ){
		buff = fgetc( source_file ) ;
	}

	// recuperation de la taille du message
	// dans l ordre LSB vers MSB
	for( int j = 0 ; j < 8 ; j++ ){

		red_byte = fgetc( source_file ) ;
		green_byte = fgetc( source_file ) ;
		blue_byte = fgetc( source_file ) ;

		lsb = red_byte & 0x1 ;
		msg_size = msg_size | ( lsb << j ) ;
	}

	// extraction du message
	for( i = 0 ; i < msg_size ; i++ ){

		for( int j = 0 ; j < 8 ; j++ ){

			red_byte = fgetc( source_file ) ;
			green_byte = fgetc( source_file ) ;
			blue_byte = fgetc( source_file ) ;

			lsb = red_byte & 0x1 ;

			secret_message[i] = secret_message[i] | ( lsb << j ) ;
		}
	}
	secret_message[ msg_size ] = '\0' ;
    printf("[+] Le message a ete recupere\n") ;
	printf("\tTaille : %d\n", msg_size) ;
	printf("\tMessage recupere :\n%s\n", secret_message );

    fclose( source_file ) ;
}
