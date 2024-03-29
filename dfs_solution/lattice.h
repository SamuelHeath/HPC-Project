#ifndef LATTICE_H_INCLUDED
#define LATTICE_H_INCLUDED

#include <stdbool.h>
#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

extern bool bflag;
extern bool sflag;

typedef struct bond {
	unsigned char left:1,up:1,right:1,down:1,visited:2; //Uses 6 bits out of the 8
} BOND;

typedef struct lattice {
	int len;
	BOND** bond_array;
} LATTICE;

extern LATTICE lat;
extern int lat_size;
extern double prob;

extern void init_lattice();
extern void destroy_lattice();
extern void print_lattice(int l, char c);
extern void seed_lattice_sites(double sp);
extern void seed_lattice_bonds(double bp);

#endif
