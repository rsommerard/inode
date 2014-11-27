#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include "hardware.h"
#include "drive.h"

static void empty_it()
{
    return;
}

void dmps(int cylinder, int sector)
{
    unsigned char buffer[HDA_SECTORSIZE];
    unsigned int i;

    read_sector(cylinder, sector, buffer);

    printf("\n--------------------------------------------------------------------------------\n");
    printf("\tCylindre %i | Secteur %i\n", cylinder, sector);
    printf("--------------------------------------------------------------------------------");
    for(i = 0; i < HDA_SECTORSIZE; i++)
    {		
        if(i%8 == 0) {
            printf("\n");
        }
        printf("\t0x%x ",buffer[i]);
    }
    printf("\n");
    printf("--------------------------------------------------------------------------------\n\n");
}

int main(int argc, char **argv)
{
    unsigned int i;	
    unsigned int cylinder;
    unsigned int sector;
    int opt;

    opterr = 0;

    cylinder = 0;
    sector = 0;

    while((opt = getopt(argc, argv, "c:s:")) != -1)
    {
        switch(opt)
        {
            case 'c':
                cylinder = atoi(optarg);
                break;
            case 's':
                sector = atoi(optarg);
                break;
            case '?':
                if(optopt == 'c' || optopt == 's')
                {
                    printf("Option '-%c' requires an argument.\n", optopt);
                }
                else if(isprint(optopt))
                {
                    printf("Unknown option '-%c'.\n", optopt);
                }
                else
                {
                    printf("Unknown option character '\\x%x'.\n", optopt);
                }
                exit(EXIT_FAILURE);
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }
    }

    for(i = optind; i < argc; i++)
    {
        printf("Non-option argument %s\n", argv[i]);
        opterr++;
    }

    if(opterr)
    {
        exit(EXIT_FAILURE);
    }

    if(cylinder < 0 || cylinder >= HDA_MAXCYLINDER) {
	    fprintf(stderr, "Error cylinder number (HDA_MAXCYLINDER = %d).\n", (HDA_MAXCYLINDER-1));
	    exit(EXIT_FAILURE);
    }

    if(sector < 0 || sector >= HDA_MAXSECTOR) {
	    fprintf(stderr, "Error sector number (HDA_MAXSECTOR = %d).\n", (HDA_MAXSECTOR-1));
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
    
    dmps(cylinder, sector);

    /* and exit! */
    exit(EXIT_SUCCESS);
}
