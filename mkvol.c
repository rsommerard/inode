#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hardware.h"
#include "mbr.h"
#include "drive.h"
#include "vol.h"

/* default values */
#define NSECTORS_DFLT           100
#define FIRST_CYLINDER_DFLT     0
#define FIRST_SECTOR_DFLT       1
#define VOL_DFLT                0

extern struct mbr_s mbr;

static void empty_it()
{
    return;
}

int main (int argc, char **argv)
{
    unsigned int i;
    unsigned int index_valide;
    unsigned int size;
    unsigned int cylinder;
    unsigned int sector;
    unsigned int vol;

    if(argc != 1 && argc != 7)
    {
	    fprintf(stderr, "Error arguments missing\n");
	    exit(EXIT_FAILURE);
    }

    if(argc == 1)
    {
        size = NSECTORS_DFLT;
        cylinder = FIRST_CYLINDER_DFLT;
        sector = FIRST_SECTOR_DFLT;
    }
    else
    {
        size = atoi(argv[2]);
        if(size < 0 || size > (HDA_SECTORSIZE * ((HDA_MAXSECTOR * HDA_MAXCYLINDER) - 1)))
        {
            fprintf(stderr, "Error size\n");
	    exit(EXIT_FAILURE);
        }

        cylinder = atoi(argv[4]);
        if(cylinder < 0 || cylinder > HDA_MAXCYLINDER)
        {
            fprintf(stderr, "Error cylinder\n");
	    exit(EXIT_FAILURE);
        }

	sector = atoi(argv[6]);
        if(sector < 0 || sector > HDA_MAXCYLINDER)
        {
            fprintf(stderr, "Error sector\n");
	    exit(EXIT_FAILURE);
        }
    }

    vol = VOL_DFLT;
    
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

    init_master();
    
    index_valide = -1;
    for(i=0; i<NB_VOLS; i++)
    {
        if(mbr.vols[i].valide == 0)
        {
            index_valide = i;
            vol = i;
            break;
        }
    }

    if(index_valide == -1)
    {
        fprintf(stderr, "Error no free volume\n");
	    exit(EXIT_FAILURE);
    }
            
    mbr.vols[vol].type = BASE;
    mbr.vols[vol].nb_sec = size;
    mbr.vols[vol].prem_cyl = cylinder;
    mbr.vols[vol].prem_sec = sector;
    mbr.vols[vol].valide = 1;

    mbr.nb_vols++;

    write_mbr();
    
    format_vol(vol);

    /* and exit! */
    exit(EXIT_SUCCESS);
}
