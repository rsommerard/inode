#ifndef _IFILE_H_
#define _IFILE_H_

#include "drive.h"

struct file_desc_t
{
    unsigned inumber;
    unsigned cursor_pos; /* à l'octet */
    unsigned size;
    char[HDA_SECTORSIZE] buffer;
    int buffer_modified;
}

unsigned int create_ifile(enum file_type_e type); 
int delete_ifile(unsigned int inumber);

int open_ifile(file_desc_t *fd, unsigned int inumber);
void close_ifile(file_desc_t *fd);
void flush_ifile(file_desc_t *fd);
void seek_ifile(file_desc_t *fd, int r_offset);  /* relatif */
void seek2_ifile(file_desc_t *fd, int a_offset); /* absolu */

int readc_ifile(file_desc_t *fd);
int writec_ifile(file_desc_t *fd, char c);
int read_ifile(file_desc_t *fd, void *buf, unsigned int nbyte);
int write_ifile(file_desc_t *fd, const void *buf, unsigned int nbyte);


#endif


