#include <stdlib.h>
#include <string.h>
#include "ifile.h"
#include <assert.h>


unsigned int create_ifile(enum file_type_e type)
{
    return create_inode(type);
}

int delete_ifile(unsigned int inumber)
{
    delete_inode(inumber);
    return 0;
}

int open_ifile(file_desc_t *fd, unsigned int inumber)
{
    struct inode_s inode;
    
    read_inode(inumber, &inode);
    
    fd->inumber = inumber;
    fd->cursor_pos = 0;
    fd->size = inode.size;
    fd->buffer = inode;
    fd->buffer_modified = 0;
}

void close_ifile(file_desc_t *fd)
{
    flush_ifile(fd);
    free(fd);
}
void flush_ifile(file_desc_t *fd)
{
    if(fd->buffer_modified)
        write_bloc(vbloc_of_fbloc(fd->inumber, fd->cursor_pos, 1);fd->cursor_pos, fd->buffer);
}

void seek_ifile(file_desc_t *fd, int r_offset)
{
    ;
}  /* relatif */

void seek2_ifile(file_desc_t *fd, int a_offset)
{
    int bloc;
    
    bloc = vbloc_of_fbloc(fd->inumber, a_offset, 0);
    read_bloc(current_vol, bloc, fd->buffer);
} /* absolu */

int readc_ifile(file_desc_t *fd)
{
    ;
}
int writec_ifile(file_desc_t *fd, char c)
{
    ;
}
int read_ifile(file_desc_t *fd, void *buf, unsigned int nbyte)
{
    ;
}
int write_ifile(file_desc_t *fd, const void *buf, unsigned int nbyte)
{
    ;
}
