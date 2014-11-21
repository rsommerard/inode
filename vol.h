#ifndef _VOL_H_
#define _VOL_H_

extern struct mbr_s mbr;

void read_bloc(unsigned int vol, unsigned int nbloc, unsigned char *buffer);

void write_bloc(unsigned int vol, unsigned int nbloc, const unsigned char *buffer);

void format_vol(unsigned int vol);

#endif
