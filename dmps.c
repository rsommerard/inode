#include <stdlib.h>
#include <stdio.h>
#include "hardware.h"
#include "drive.h"

static void empty_it()
{
    return;
}

int main(int argc, char **argv)
{
    unsigned int i;
    unsigned char buffer[HDA_SECTORSIZE];
    unsigned int cylinder;
    unsigned int sector;

    if(argc < 3) {
	    fprintf(stderr, "Error arguments missing\n");
	    exit(EXIT_FAILURE);
    }

    cylinder = atoi(argv[1]);
    sector = atoi(argv[2]);

    if(cylinder < 0 || cylinder >= HDA_MAXCYLINDER) {
	    fprintf(stderr, "Error cylinder\n");
	    exit(EXIT_FAILURE);
    }

    if(sector < 0 || sector >= HDA_MAXSECTOR) {
	    fprintf(stderr, "Error sector\n");
	    exit(EXIT_FAILURE);
    }
    
    /* init hardware */
    if(init_hardware("hardware.ini") == 0) {
	    fprintf(stderr, "Error in hardware initialization\n");
	    exit(EXIT_FAILURE);
    }

    /* Interreupt handlers */
    for(i=0; i<16; i++)
	    IRQVECTOR[i] = empty_it;

    /* Allows all IT */
    _mask(1);

    read_sector(cylinder, sector, buffer);
    
    printf("--------------------------------------------------------------------------------\n");
    printf("Cylindre %i | Secteur %i\n", cylinder, sector);
    printf("--------------------------------------------------------------------------------");
    for(i = 0; i < HDA_SECTORSIZE; i++)
    {
        if(i%8 == 0) {
            printf("\n");
        }
        printf("\t0x%x ",buffer[i]);
    }
    printf("\n");
    printf("--------------------------------------------------------------------------------\n");

    /* and exit! */
    exit(EXIT_SUCCESS);
}
