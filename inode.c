#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "mbr.h"
#include "drive.h"
#include "vol.h"
#include "bloc.h"
#include "inode.h"



void read_inode(unsigned inumber, struct inode_s *inode)
{
    assert(HDA_SECTORSIZE == sizeof(struct inode_s));
    read_bloc(vol_courant, inumber, (void*)inode);
}

void write_inode(unsigned inumber, struct inode_s *inode)
{
    assert(HDA_SECTORSIZE == sizeof(struct inode_s));
    write_bloc(vol_courant, inumber, (void*)inode);
}


unsigned create_inode(enum file_type_e type)
{
    struct inode_s inode;
    unsigned inumber;
    
    memset(&inode, 0, sizeof(struct inode_s));
    inode.type = type;
    inumber = new_bloc();
    write_inode(inumber, &inode);
    
    return inumber;
}

void free_blocs(unsigned *blocs, unsigned size)
{
    unsigned i;
    for(i=0; i<size; i++)
    {
        if(blocs[i])
            free_bloc(blocs[i]);
    }
}


void delete_inode(unsigned inumber)
{
    struct inode_s inode;
    struct indirect_s bloc_blocs;
    struct indirect_s bloc_blocs2;
    int i;
    
    read_inode(inumber, &inode);
    free_blocs(inode.blocs_directs, (HDA_SECTORSIZE-sizeof(enum file_type_e) - 3*sizeof(unsigned)) / sizeof(unsigned));

    if(inode.bloc_indirect)
    {
        read_bloc(vol_courant, inode.bloc_indirect, (void*)(&bloc_blocs));
        free_blocs(bloc_blocs.directs, HDA_SECTORSIZE/sizeof(unsigned));
        free_bloc(inode.bloc_indirect);
    }
    
    if(inode.bloc_double_indirect)
    {
        read_bloc(vol_courant, inode.bloc_double_indirect, (void*)(&bloc_blocs2));
        for(i=0; i < HDA_SECTORSIZE/sizeof(unsigned); i++)
        {
            read_bloc(vol_courant, bloc_blocs2.directs[i], (void*)(&bloc_blocs));
            free_blocs(bloc_blocs.directs, HDA_SECTORSIZE/sizeof(unsigned));
            free_bloc(bloc_blocs2.directs[i]);
        }
        free_bloc(inode.bloc_double_indirect);
    }
    free_bloc(inumber);
}

unsigned int vbloc_of_fbloc(unsigned int inumber, unsigned int fbloc, bool_t do_allocate)
{
    struct inode_s inode;
    struct indirect_s bloc;
    struct indirect_s bloc2;
    struct indirect_s bloc_blocs;
    struct indirect_s bloc_blocs2;
    int i, index;
    int j;
    
    if(fbloc >= MAX_ENTRIES)
        return 0;
    
    read_inode(inumber, &inode);
    
    if(fbloc < MAX_DIRECT_ENTRIES)
    {
        if(do_allocate && !inode.blocs_directs[fbloc]) {
            inode.blocs_directs[fbloc] = new_bloc();
            write_inode(inumber, &inode);
        }
        
        return inode.blocs_directs[fbloc];
    }
    
    if(fbloc < (MAX_DIRECT_ENTRIES + NB_BLOCS_PAR_BLOC))
    {
        
        if(!do_allocate && !inode.bloc_indirect) {
            return 0;
        }
        
        if(do_allocate && !inode.bloc_indirect) {
            inode.bloc_indirect = new_bloc();
            read_bloc(vol_courant, inode.bloc_indirect, (void*)(&bloc));
            for(j = 0; j < NB_BLOCS_PAR_BLOC; j++)
                bloc.directs[j] = 0;
            write_bloc(vol_courant, inode.bloc_indirect, (void*)(&bloc));
            write_inode(inumber, &inode);
        }
        
        read_bloc(vol_courant, inode.bloc_indirect, (void*)(&bloc_blocs));
        
        if(do_allocate && !bloc_blocs.directs[fbloc - MAX_DIRECT_ENTRIES]) {
            bloc_blocs.directs[fbloc - MAX_DIRECT_ENTRIES] = new_bloc();
            write_bloc(vol_courant, inode.bloc_indirect, (void*)(&bloc_blocs));
            write_inode(inumber, &inode);
        }
        
        return bloc_blocs.directs[fbloc - MAX_DIRECT_ENTRIES];
    }
    
    index = MAX_DIRECT_ENTRIES + NB_BLOCS_PAR_BLOC + NB_BLOCS_PAR_BLOC;
    i = 0;
    
    if(!do_allocate && !inode.bloc_double_indirect) {
        return 0;
    }
    
    if(do_allocate && !inode.bloc_double_indirect) {
        inode.bloc_double_indirect = new_bloc();
        read_bloc(vol_courant, inode.bloc_double_indirect, (void*)(&bloc));
        for(j = 0; j < NB_BLOCS_PAR_BLOC; j++)
            bloc.directs[j] = 0;
        write_bloc(vol_courant, inode.bloc_double_indirect, (void*)(&bloc));
        write_inode(inumber, &inode);
    }
    
    read_bloc(vol_courant, inode.bloc_double_indirect, (void*)(&bloc_blocs));
    
    do {
        if(fbloc < index) {
            if(do_allocate && !bloc_blocs.directs[i]) {
                bloc_blocs.directs[i] = new_bloc();
                read_bloc(vol_courant, bloc_blocs.directs[i], (void*)(&bloc2));
                for(j = 0; j < NB_BLOCS_PAR_BLOC; j++)
                    bloc2.directs[j] = 0;
                write_bloc(vol_courant, bloc_blocs.directs[i], (void*)(&bloc2));
                write_bloc(vol_courant, inode.bloc_double_indirect, (void*)(&bloc_blocs));
                write_inode(inumber, &inode);
            }
            
            read_bloc(vol_courant, bloc_blocs.directs[i], (void*)(&bloc_blocs2));
            
            if(do_allocate && !bloc_blocs2.directs[fbloc - index + NB_BLOCS_PAR_BLOC]) {
                bloc_blocs2.directs[fbloc - index + NB_BLOCS_PAR_BLOC] = new_bloc();
                write_bloc(vol_courant, bloc_blocs.directs[i], (void*)(&bloc_blocs2));
                write_inode(inumber, &inode);
            }

            return bloc_blocs2.directs[fbloc - index + NB_BLOCS_PAR_BLOC];
        }
        i++;
        index += NB_BLOCS_PAR_BLOC;
    } while(index <= MAX_ENTRIES);
    
    return 0;
}
                            





































