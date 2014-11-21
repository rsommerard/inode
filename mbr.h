#ifndef _MBR_H_
#define _MBR_H_

#define NB_VOLS 8
#define MAGIC 0xDEADBEEF

enum type_vol_e
{
    BASE,
    ANNEXE,
    AUTRE
};

struct vol_s
{
    enum type_vol_e type;
    unsigned int nb_sec;
    unsigned int prem_cyl;
    unsigned int prem_sec;
    int valide;
};

struct mbr_s
{
    struct vol_s vols[NB_VOLS];
    unsigned int nb_vols;
    int magic;
};

void read_mbr();

void write_mbr();

#endif
