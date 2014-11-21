#ifndef _BLOC_H_
#define _BLOC_H_

#define MAGIC_SB 0xDEAD

struct superbloc_s
{
    int magic;
    int serial;
    char name[32];
    unsigned premier_libre;
    unsigned nb_blocs_libres;
    unsigned inoeud_racine;
};

struct superbloc_s sb;

struct bloc_libre_s
{
    unsigned size;
    unsigned next;
};

unsigned vol_courant;

void init_super(unsigned vol, int serie, char* nom);

void save_super();

int load_super(unsigned vol);

unsigned new_bloc();

void free_bloc(unsigned bloc);

#endif
