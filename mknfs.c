#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware.h"
#include "mbr.h"
#include "drive.h"
#include "bloc.h"

static void empty_it()
{
    return;
}

int main (int argc, char **argv)
{
    int i;
    int current_volume_int;
    char* hw_config;
    char* current_volume;
    
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
    
    init_super(current_volume_int, 666, "TOTORO");

    /* and exit! */
    exit(EXIT_SUCCESS);
}
