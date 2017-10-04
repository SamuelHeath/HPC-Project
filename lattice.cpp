#include "lattice.h"

LATTICE lat;


/**
 * Return true if site is open, false otherwise. 
 */
bool is_site_open(int r, int c)
{
	if(lat.lattice_array[r][c] == 0)
		return false;
	return true;
}


/**
 * Prints the existing lattice, takes the length of the 2D array 
 * as argument. If 'v' is passed in as print type a visual representation is 
 * provided, any other character defaults to 1s and 0s. 
 */
void print_lattice(int len, char viewType)
{
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < len; j++)
		{
			if(viewType == 'v' || viewType == 'V')
			{
				if(lat.lattice_array[i][j] == 1)
					printf("\u2588"); //dark shade
				else if(lat.lattice_array[i][j] == 2)
					printf("\u2592"); //medium shade
				else
					printf(" ");
			}
			else
				printf("%i", lat.lattice_array[i][j]);
		}
		printf("\n");
	}
}

/**
 * A function that seeds the lattice sites, provided a site probability.
 */
void seed_lattice_sites(double prob)
{
	//seeding pseudo-random number generator.
	srand(time(NULL));
	for(int i = 0; i < lat.len; i++)
	{
		for(int j = 0; j < lat.len; j++)
		{
			double site_prob = (double)rand()/(double)RAND_MAX;
			if(site_prob < prob )
				lat.lattice_array[i][j] = 1;
			else
				lat.lattice_array[i][j] = 0;
		}
	}
}


/**
 * A function that seeds the lattice bonds, provided a bond probability.
 */
void seed_lattice_bonds(double prob)
{
	srand(time(NULL));
	for(int i = 0; i < lat.len; i++)
	{
		for(int j = 0; j < lat.len; j++)
		{
			double bond_prob = (double)rand()/(double)RAND_MAX;
			if(bond_prob < prob)
			{
				lat.lattice_array[i][j] = 1;
				lat.lattice_array[(i+1)%lat.len][j] = 1; //down neighbour
			}
			bond_prob = (double)rand()/(double)RAND_MAX;
			if(bond_prob < prob)
			{
				lat.lattice_array[i][j] = 1;
				lat.lattice_array[i][(j+1)%lat.len] = 1; //right neighbour
			}
		}

	}
}


/**
 * Initialize the lattice struct to specified length.
 * Dynamically allocates the array representing the lattice to a specified size.
 */
void init_lattice(int arrlen)
{
	lat.len = arrlen;
	if(arrlen <= 1){
		fprintf(stderr, "%d is an invalid lattice size. Must be greater than 1", arrlen);
		return;
	}
	//dynamically allocate memory for an arrlen*arrlen 2D array.
	lat.lattice_array = (char**) malloc(arrlen*sizeof(int*));
	for(int i = 0; i < arrlen; i++){
		lat.lattice_array[i] = (char*) malloc(arrlen*sizeof(int));
	}
}


/**
 * Frees dynamically allocated memory for storing the lattice.
 */
void destroy_lattice()
{
	for(int i = 0; i < lat.len; i++)
	{
		free(lat.lattice_array[i]);
	}
	free(lat.lattice_array);
}


/**
 * Return the lattice array.
 */
char** get_lattice_array()
{
	return lat.lattice_array;
}
