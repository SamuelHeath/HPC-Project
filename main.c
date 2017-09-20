#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <omp.h>
#include <getopt.h>

#define SIZE 96
#define OPTLIST "p:sb"

bool pflag = false;
bool sflag = false;
bool bflag = false;

bool percolates = false;
double prob;

/**
 *	Stack
 */
typedef struct node {
	struct node* parent;
	int position[2];
} NODE;

NODE *stack;
int size_stack = 0;

bool isEmpty() { return !(size_stack > 0); }

NODE* peek() {
	if (!isEmpty()) return &stack[size_stack];
	else return NULL;
}

//Remember to release memory or save elsewhere after you pop.
NODE* pop() {
	if (!isEmpty()) {
		return &stack[--size_stack];
	} else return NULL;
}

void push(NODE n) {
	stack = (NODE *) realloc(stack,sizeof(NODE)*(++size_stack));
	stack[size_stack-1] = n;
}
/**
 * End Stack
 */

/**
 * 0 = unoccupied, 1 = occupied, 2 = visited.
 */
typedef struct lattice {
	int sites[SIZE][SIZE]; //Change to int??
} LATTICE;

LATTICE lattice;

void printNodes() {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			printf("%d ", lattice.sites[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void seed_sites() {
	#pragma omp parallel for num_threads(8) collapse(2)
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if ((double) (rand()/(RAND_MAX+1.0)) < prob) {
					lattice.sites[i][j] = 1;
				}
			}
		}
}

void seed_bonds () {
	#pragma omp parallel for num_threads(8) collapse(2)
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				double r = (rand()/(RAND_MAX+1.0)),d = (rand()/(RAND_MAX+1.0));
				if (r < prob) {
					//Creates bonds wit surrounding sites by setting them to occupied
					//NOT SURE IF THIS IS CORRECT!!!
					lattice.sites[i][j] = 1;
					lattice.sites[i][j+(1%SIZE)] = 1;
				}
				if (d < prob) {
					lattice.sites[i][j] = 1;
					lattice.sites[i+(1%SIZE)][j] = 1;
				}
			}
		}
}

//This could skip over some values needs checking.
int find_start(int last_tried) {
	bool found = false;
	for (int i = last_tried; i < SIZE; i++) {
		if (lattice.sites[0][i] == 1) {
			lattice.sites[0][i] = 2; //visited.
			NODE n = {NULL,{0,i}};
			push(n);
		} else if (lattice.sites[i][0] == 1) {
			lattice.sites[i][0] = 2; //visited.
			NODE n = {NULL,{i,0}};
			push(n);
		}
		if (found) return i;
	}
	return SIZE;
}

void addNode(NODE* n, int i, int j) {
	NODE node = {n,{i,j}};
	lattice.sites[i][j] = 2;
	push(node);
}

bool check_percolates() {
	while (!isEmpty()) {
		NODE* n = pop();
		int i = n->position[0],j = n->position[1];
		if (lattice.sites[(i+1)%SIZE][j] == 1) addNode(n,i+1%SIZE,j);
		if (lattice.sites[i][(j+1)%SIZE] == 1) addNode(n,i,(j+1)%SIZE);
		if (lattice.sites[(i+SIZE-1)%SIZE][j] == 1) addNode(n,(i+SIZE-1)%SIZE,j);
		if (lattice.sites[i][(j+SIZE-1)%SIZE] == 1) addNode(n, i, (j+SIZE-1)%SIZE);
	}
	return false;
}

int main(int argc, char *argv[]) {
	int opt;
	while ((opt = getopt(argc,argv,OPTLIST)) != -1) {
		switch (opt) {
			case 'p': 
				if (isdigit(optarg[0])) {
					prob = strtod(optarg,NULL);
					pflag = true;
				} else {
					fprintf(stderr, "Invalid probability argument.\n");
					exit(EXIT_FAILURE);
				}
				break;
			case 's':
				sflag = true;
				break;
			case 'b':
				bflag = true;
				break;
			case '?': fprintf(stderr, "Invalid command.\n"); exit(EXIT_FAILURE);
		}
	}
	if (sflag == bflag) {
		fprintf(stderr, "You need only one -s or -b flag for either site or bond percolation.\n");
		exit(EXIT_FAILURE);
	} else if (!pflag) {
		fprintf(stderr, "No percolation probability given.\n");
		exit(EXIT_FAILURE);
	}

	//Check if we are looking for the site percolation or bond.
	if (sflag) {
		seed_sites();
	} else {
		seed_bonds();
	}

	for (int i = 0; i < SIZE; i++) {
		i = find_start(i);

		if (check_percolates()) {
			percolates = true;
			break;
		}
		printNodes();
	}

	if (percolates) {
		printf("Percolates at %.8f\n", prob);
	} else {
		printf("Does not percolate\n");
	}

	exit(EXIT_SUCCESS);
}