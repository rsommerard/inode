#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hardware.h"
#include "drive.h"
#include "bloc.h"
#include "mbr.h"
#include "inode.h"


static void empty_it()
{
    return;
}

int main (int argc, char **argv)
{
    unsigned int i;
    int current_volume_int;
    char* hw_config;
    char* current_volume;
    struct inode_s inode;
    int inumber;
    
    
    hw_config = getenv("HW_CONFIG");
    current_volume = getenv("CURRENT_VOLUME");
    current_volume_int = atoi(current_volume);
    
    /* init hardware */
    if(init_hardware(hw_config) == 0)
    {
	    fprintf(stderr, "Error in hardware initialization\n");
	    exit(EXIT_FAILURE);
    }

    /* Interreupt handlers */
    for(i=0; i<16; i++)
	    IRQVECTOR[i] = empty_it;

    /* Allows all IT */
    _mask(1);

    read_mbr();
    load_super(current_volume_int);

    inumber = create_inode(FILE_FILE);
    read_inode(inumber, &inode);
    
    vbloc_of_fbloc(inumber, 115, 1);
    
    vbloc_of_fbloc(inumber, 7, 1);
    
    vbloc_of_fbloc(inumber, 125, 1);
    
    vbloc_of_fbloc(inumber, 320, 1);
    
    vbloc_of_fbloc(inumber, 396, 1);
    
    read_inode(inumber, &inode);
    
    printf("7: %i\n", vbloc_of_fbloc(inumber, 7, 0));
    printf("115: %i\n", vbloc_of_fbloc(inumber, 115, 0));
    printf("125: %i\n", vbloc_of_fbloc(inumber, 125, 0));
    printf("320: %i\n", vbloc_of_fbloc(inumber, 320, 0));
    printf("396: %i\n", vbloc_of_fbloc(inumber, 396, 0));

    /* and exit! */
    exit(EXIT_SUCCESS);
}
















