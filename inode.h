#ifndef _INODE_H_
#define _INODE_H_

#include "drive.h"

#define MAX_DIRECT_ENTRIES  (HDA_SECTORSIZE-sizeof(enum file_type_e) - 3*sizeof(unsigned)) / sizeof(unsigned)
#define NB_BLOCS_PAR_BLOC   HDA_SECTORSIZE/sizeof(unsigned)
#define MAX_ENTRIES         (MAX_DIRECT_ENTRIES + NB_BLOCS_PAR_BLOC + NB_BLOCS_PAR_BLOC*NB_BLOCS_PAR_BLOC)
#define TAILLE_MAX_INODE    MAX_ENTRIES*HDA_SECTORSIZE


extern unsigned vol_courant;
typedef int bool_t;



enum file_type_e
{
    FILE_FILE,
    FILE_DIRECTORY
};


struct indirect_s
{
    unsigned directs[HDA_SECTORSIZE/sizeof(unsigned)];
};

struct inode_s
{
    enum file_type_e type;
    unsigned size;
    unsigned blocs_directs[MAX_DIRECT_ENTRIES];
    unsigned bloc_indirect;
    unsigned bloc_double_indirect;
};

void read_inode(unsigned inumber, struct inode_s *inode);

void write_inode(unsigned inumber, struct inode_s *inode);

unsigned create_inode(enum file_type_e type);

void delete_inode(unsigned inumber);

unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc, bool_t do_allocate); 



#endif
