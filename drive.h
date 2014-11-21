#ifndef _DRIVE_H_
#define _DRIVE_H_

#define HDA_CMDREG 0x3F6
#define HDA_DATAREGS 0x110
#define HDA_IRQ 14 
#define HDA_MAXCYLINDER 16
#define HDA_MAXSECTOR 16
#define HDA_SECTORSIZE 512

#define CMD_SEEK 0x02
#define CMD_READ 0x04
#define CMD_WRITE 0x06
#define CMD_FORMAT 0x08
#define CMD_DSKNFO 0x16


void read_sector(unsigned int cylinder, unsigned int sector, unsigned char *buffer);

void write_sector(unsigned int cylinder, unsigned int sector, const unsigned char *buffer);

void format_sector(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value);

void init_master();
                   
#endif
