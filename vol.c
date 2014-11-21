#include "mbr.h"
#include "vol.h"
#include "drive.h"

unsigned int ncyl_of_nbloc(unsigned int nvol, unsigned int nbloc)
{
    return (mbr.vols[nvol].prem_cyl + ((mbr.vols[nvol].prem_sec + nbloc) / HDA_MAXSECTOR));
}

unsigned int nsec_of_nbloc(unsigned int nvol, unsigned int nbloc)
{
    return ((mbr.vols[nvol].prem_sec + nbloc) % HDA_MAXSECTOR);
}

void read_bloc(unsigned int vol, unsigned int nbloc, unsigned char *buffer)
{
    read_sector(ncyl_of_nbloc(vol,nbloc), nsec_of_nbloc(vol, nbloc), buffer);
}

void write_bloc(unsigned int vol, unsigned int nbloc, const unsigned char *buffer)
{
    write_sector(ncyl_of_nbloc(vol,nbloc), nsec_of_nbloc(vol, nbloc), buffer);
}

void format_vol(unsigned int vol)
{
    format_sector(mbr.vols[vol].prem_cyl, mbr.vols[vol].prem_sec, mbr.vols[vol].nb_sec, 0);
}
