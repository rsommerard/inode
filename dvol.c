#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "hardware.h"
#include "mbr.h"

extern struct mbr_s mbr;

static void empty_it()
{
    return;
}

void dvol(int vol)
{
    int i;

    read_mbr();

    if(vol != -1 && (vol < 0 || vol >= NB_VOLS))
    {
        printf("Error volume invalid.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n--------------------------------------------------------------------------------\n");
    printf("\tMBR\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("Volumes on disk: %i\n", mbr.nb_vols);

    if(vol == -1)
    {
        for(i=0; i < NB_VOLS; i++)
        {
            printf("--------------------------------------------------------------------------------\n");
            printf("\tVolume %i\n", i);
            printf("--------------------------------------------------------------------------------\n");
            printf("Type: %i\n", mbr.vols[i].type);
            printf("NbSec: %i\n", mbr.vols[i].nb_sec);
            printf("PremCyl: %i\n", mbr.vols[i].prem_cyl);
            printf("PremSec: %i\n", mbr.vols[i].prem_sec);
            printf("Valide: %i\n", mbr.vols[i].valide);
        }
    }
    else
    {
        printf("--------------------------------------------------------------------------------\n");
        printf("\tVolume %i\n", vol);
        printf("--------------------------------------------------------------------------------\n");
        printf("Type: %i\n", mbr.vols[vol].type);
        printf("NbSec: %i\n", mbr.vols[vol].nb_sec);
        printf("PremCyl: %i\n", mbr.vols[vol].prem_cyl);
        printf("PremSec: %i\n", mbr.vols[vol].prem_sec);
        printf("Valide: %i\n", mbr.vols[vol].valide);
    }

    printf("--------------------------------------------------------------------------------\n\n");
}

int main (int argc, char **argv)
{
    int i;
    int vol;

    int opt;

    opterr = 0;

    vol = -1;

    while((opt = getopt(argc, argv, "v:")) != -1)
    {
        switch(opt)
        {
            case 'v':
                vol = atoi(optarg);
                break;
            case '?':
                if(optopt == 'v')
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

    dvol(vol);

    /* and exit! */
    exit(EXIT_SUCCESS);
}
