//
//  fonctions.h
//  Stega
//
//  Created by Maskott on 15/11/2021.
//

#ifndef bitmap_h
#define bitmap_h

#include "public.h"
#include "librairies.h"

typedef struct {
	char * file_name ;
    int height ;
    int width ;
    unsigned int magic_cookie ;
    unsigned long int data_offset ;
} bitmap;

unsigned int check_img_bmp( FILE * ) ;
unsigned long int get_bmp_offset( FILE * ) ;
bitmap * parse_bitmap( char * ) ;
void release( bitmap * ) ;

#endif /* bitmap_h */
