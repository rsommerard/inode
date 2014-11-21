#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "mbr.h"
#include "drive.h"

struct mbr_s mbr;

void read_mbr()
{
    unsigned char buffer[HDA_SECTORSIZE];
    int i;
    
    read_sector(0, 0, buffer);
    
    memcpy(&mbr, buffer, sizeof(struct mbr_s));
    
    if(mbr.magic != MAGIC)
    {
        assert(mbr.magic != MAGIC);
        mbr.nb_vols = 0;
        for(i=0; i<NB_VOLS; i++)
        {
            mbr.vols[i].type = BASE;
            mbr.vols[i].nb_sec = 0;
            mbr.vols[i].prem_cyl = 0;
            mbr.vols[i].prem_sec = 0;
            mbr.vols[i].valide = 0;
        }
        mbr.magic = MAGIC;
    }
}

void write_mbr()
{
    unsigned char buffer[HDA_SECTORSIZE];
    
    assert(mbr.magic == MAGIC);
    
    memset(buffer, HDA_SECTORSIZE, 0);
    
    memcpy(buffer, &mbr, sizeof(struct mbr_s));
    
    write_sector(0, 0, buffer);
}
