#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "mbr.h"
#include "drive.h"
#include "vol.h"
#include "bloc.h"

extern struct mbr_s mbr;

void init_super(unsigned vol, int serie, char* nom)
{
    struct vol_s volume;
    struct bloc_libre_s bl;
    unsigned char buf[HDA_SECTORSIZE];
    
    read_mbr();
    volume = mbr.vols[vol];
    
    sb.magic = MAGIC_SB;
    sb.serial = serie;
    sb.premier_libre = 1;
    sb.nb_blocs_libres = volume.nb_sec - 1;
    sb.inoeud_racine = 0;
    strncpy(sb.name, nom, 32);
    
    bl.size = volume.nb_sec - 1;
    bl.next = 0;
    
    save_super();
    
    memset(buf, 0, HDA_SECTORSIZE);
    memcpy(buf, &bl, sizeof(struct bloc_libre_s));
    write_bloc(vol, 1, buf);
    
    vol_courant = vol;
}

void save_super()
{
    unsigned char buf[HDA_SECTORSIZE];
    memset(buf, 0, HDA_SECTORSIZE);
    memcpy(buf, &sb, sizeof(struct superbloc_s));
    write_bloc(vol_courant, 0, buf);
}

int load_super(unsigned vol)
{
    unsigned char buf[HDA_SECTORSIZE];
    
    memset(buf, 0, HDA_SECTORSIZE);
    read_bloc(vol, 0, buf);
    memcpy(&sb, buf, sizeof(struct superbloc_s));
    
    if(sb.magic == MAGIC_SB)
    {
        vol_courant = vol;
        return 0;
    }

    return -1;
}

unsigned new_bloc()
{
    struct bloc_libre_s* bl;
    unsigned char buf[HDA_SECTORSIZE];
    unsigned res;

    if(sb.nb_blocs_libres == 0)
        return 0;
        
    assert(sb.premier_libre);
    
    read_bloc(vol_courant, sb.premier_libre, buf);
    bl = (struct bloc_libre_s*) buf;
    
    res = sb.premier_libre;
    
    if(bl->size > 1)
    {
        sb.premier_libre++;
        bl->size--;
        write_bloc(vol_courant, sb.premier_libre, buf);
    }
    else
    {
        sb.premier_libre = bl->next;
    }
    
    sb.nb_blocs_libres--;
    save_super();
    
    return res;
}

void free_bloc(unsigned bloc)
{
    struct bloc_libre_s bl;
    unsigned char buf[HDA_SECTORSIZE];
    
    bl.size = 1;
    bl.next = sb.premier_libre;
    sb.premier_libre = bloc;
    sb.nb_blocs_libres++;
    
    memset(buf, 0, HDA_SECTORSIZE);
    memcpy(buf, &bl, sizeof(struct bloc_libre_s));
    write_bloc(vol_courant, bloc, buf);
    save_super();
}
