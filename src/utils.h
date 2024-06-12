/*
 * utils.h
 *
 * Created on: 2015-04-01
 *     Author: Clemens Hammacher <hammacher@cs.uni-saarland.de>
 */

#ifndef _INC_UTILS_H
#define _INC_UTILS_H

void rand_init();

// compute a value between 0 and max (exclusively)
unsigned randu(unsigned max);

typedef struct{
        char name[258];
        int id;
        int visits;
        int *target;
        int in_deg;
        int out_deg;
        int *edges;

}nod;


int *open_file(char *filename);

int *collection(nod *nodes , char *filename, int size);

// extern struct node *nodes;
void remove_semicolon(char *str);
void random_surfer(nod *nodes, int size, int r_steps, double p);

void markov(nod *nodes, int size, int m_steps, double p);

// Function to duplicate a string
char *my_strdup(const char *s);

#endif /* !_INC_UTILS_H */
