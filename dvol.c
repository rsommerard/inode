#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "mbr.h"

extern struct mbr_s mbr;

static void empty_it()
{
    return;
}

int main (int argc, char **argv)
{
    int i;

    /* init hardware */
    if(init_hardware("hardware.ini") == 0)
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
    
    printf("--------------------------------------------------------------------------------\n");
    printf("MBR\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("Nb volumes: %i\n", mbr.nb_vols);
    for(i=0; i<NB_VOLS; i++)
    {
        printf("----------------------------------------\n");
        printf("Volume %i:\n", i+1);
        printf("\tType: %i\n", mbr.vols[i].type);
        printf("\tNbSec: %i\n", mbr.vols[i].nb_sec);
        printf("\tPremCyl: %i\n", mbr.vols[i].prem_cyl);
        printf("\tPremSec: %i\n", mbr.vols[i].prem_sec);
        printf("\tValide: %i\n", mbr.vols[i].valide);
    }

    /* and exit! */
    exit(EXIT_SUCCESS);
}
