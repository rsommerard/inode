#include <stdlib.h>
#include <string.h>
#include "drive.h"
#include "hardware.h"
#include "mbr.h"
#include <assert.h>

extern struct mbr_s mbr;

void seek(int cylinder, int sector)
{
    _out(HDA_DATAREGS, (cylinder >> 8) & 0xFF);
    _out(HDA_DATAREGS + 1, cylinder & 0xFF);
    _out(HDA_DATAREGS + 2, (sector >> 8) & 0xFF);
    _out(HDA_DATAREGS + 3, sector & 0xFF);
    _out(HDA_CMDREG, CMD_SEEK);
    _sleep(HDA_IRQ);
}

void read_sector(unsigned int cylinder, unsigned int sector, unsigned char *buffer)
{
    seek(cylinder, sector);
    
    _out(HDA_DATAREGS, 0);
    _out(HDA_DATAREGS + 1, 1);
    _out(HDA_CMDREG, CMD_READ);
    _sleep(HDA_IRQ);
    
    memcpy(buffer, MASTERBUFFER, HDA_SECTORSIZE);
}

void write_sector(unsigned int cylinder, unsigned int sector, const unsigned char *buffer)
{
    seek(cylinder, sector);
    
    memcpy(MASTERBUFFER, buffer, HDA_SECTORSIZE);
    
    _out(HDA_DATAREGS, 0);
    _out(HDA_DATAREGS + 1, 1);
    _out(HDA_CMDREG, CMD_WRITE);
    _sleep(HDA_IRQ);
}

void format_sector(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value)
{
    int i, j;
    int k = 0;
    
    for(i = cylinder; i < HDA_MAXCYLINDER; i++)
    {
        for(j = sector; j < HDA_MAXSECTOR; j++)
        {
            if(k++ == nsector)
                return;
            
            seek(i, j);
            _out(HDA_DATAREGS, 0);
            _out(HDA_DATAREGS + 1, 1);
            _out(HDA_DATAREGS + 2, (value >> 24) & 0xFF);
            _out(HDA_DATAREGS + 3, (value >> 16) & 0xFF);
            _out(HDA_DATAREGS + 4, (value >> 8) & 0xFF);
            _out(HDA_DATAREGS + 5, value & 0xFF);
            _out(HDA_CMDREG, CMD_FORMAT);
            _sleep(HDA_IRQ);
        }
    }
}

void init_master()
{
    int i;

    read_mbr();

    if(mbr.magic != MAGIC)
    {
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

        write_mbr();
    }
}
