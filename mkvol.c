#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "hardware.h"
#include "mbr.h"
#include "drive.h"
#include "vol.h"

extern struct mbr_s mbr;

static void empty_it()
{
    return;
}

void mkvol(int nb_sector, int prem_cyl, int prem_sec)
{
    int i;
    int vol;

    if(prem_cyl == 0 && prem_sec == 0)
    {
        printf("Error, can't use sector 0 and cylinder 0.\n");
        exit(EXIT_FAILURE);
    }

    if(nb_sector >= (HDA_MAXCYLINDER*HDA_MAXSECTOR))
    {
        printf("Error sector size.\n");
        exit(EXIT_FAILURE);
    }

    init_master();

    vol = -1;

    for(i = 0; i < NB_VOLS; i++)
    {
        if(mbr.vols[i].valide == 0)
        {
            vol = i;
            break;
        }
    }

    if(vol == -1)
    {
        printf("Error, volumes full.\n");
        exit(EXIT_FAILURE);
    }

    mbr.vols[vol].type = BASE;
    mbr.vols[vol].nb_sec = nb_sector;
    mbr.vols[vol].prem_cyl = prem_cyl;
    mbr.vols[vol].prem_sec = prem_sec;
    mbr.vols[vol].valide = 1;

    mbr.nb_vols++;

    write_mbr();
    
    format_vol(vol);

    printf("Volume %i created.\n", vol);
}

int main (int argc, char **argv)
{
    int i;

    int nb_sector;
    int prem_cyl;
    int prem_sec;

    int opt;

    opterr = 0;

    nb_sector = 1;
    prem_cyl = 0;
    prem_sec = 1;

    while((opt = getopt(argc, argv, "l:s:c:")) != -1)
    {
        switch(opt)
        {
            case 'l':
                nb_sector = atoi(optarg);
                break;
            case 'c':
                prem_cyl = atoi(optarg);
                break;
            case 's':
                prem_sec = atoi(optarg);
                break;
            case '?':
                if(optopt == 's' || optopt == 'l' || optopt == 'c')
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

    printf("nb_sector = %d, prem_cyl = %d, prem_sec = %d\n", nb_sector, prem_cyl, prem_sec);
    
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

    mkvol(nb_sector, prem_cyl, prem_sec);

    /* and exit! */
    exit(EXIT_SUCCESS);
}
