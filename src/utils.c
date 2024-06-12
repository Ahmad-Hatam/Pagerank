
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <string.h>
#include <limits.h>

void rand_init() {
  /* get three integers for seeding the RNG */
  unsigned long a = clock();
  unsigned long b = time(NULL);
  unsigned long c = getpid();

  /* now mix them
   * (this is Robert Jenkins' 96 bit Mix Function) */
  a=a-b;  a=a-c;  a=a^(c >> 13);
  b=b-c;  b=b-a;  b=b^(a << 8);
  c=c-a;  c=c-b;  c=c^(b >> 13);
  a=a-b;  a=a-c;  a=a^(c >> 12);
  b=b-c;  b=b-a;  b=b^(a << 16);
  c=c-a;  c=c-b;  c=c^(b >> 5);
  a=a-b;  a=a-c;  a=a^(c >> 3);
  b=b-c;  b=b-a;  b=b^(a << 10);
  c=c-a;  c=c-b;  c=c^(b >> 15);

  /* use this mix to initialize RNG */
  srand(c);
}

unsigned randu(unsigned max) {
  const unsigned int buckets = RAND_MAX / max;
  const unsigned int limit = buckets * max;

  unsigned int r;
  do {
    r = rand();
  } while (r >= limit);

  return r / buckets;
}





void remove_semicolon(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == ';') {
        str[len - 1] = '\0';
    }
}


int *collection( nod *nodes, char *filename, int size){


    if (filename ==NULL){
        exit(1);
    }
    FILE *file = fopen(filename, "r");
    if(file ==NULL || size==0|| nodes == NULL){
        exit(1);
    }



    char format[258], file_identifier[258], state[258], arrow[258], target[258];
    char bracket;

   
    fscanf(file,"%s %s %c", format, file_identifier,&bracket);

    int num_nodes = 0;
    
    while (fscanf(file, "%s %s %s", state, arrow, target) ==3) {

        remove_semicolon(target);

        nod *state_exists = NULL;
        nod *target_exists = NULL;

        for (int j = 0; j <=num_nodes; j++) {
            
            if (strcmp(nodes[j].name, state) == 0) {
                state_exists = &nodes[j];
                
            }
            if (strcmp(nodes[j].name, target) == 0) {
                target_exists = &nodes[j];
                
            }
            
        }

        if (!state_exists)
        {
            
            state_exists = &nodes[num_nodes];
            strncpy(state_exists->name, state, sizeof(state_exists->name) - 1);
            state_exists->name[sizeof(state_exists->name) - 1] = '\0';
            state_exists->id = num_nodes;
            state_exists->in_deg = 0;
            state_exists->out_deg = 0;
            num_nodes++;
        }
            
        if (!target_exists && strcmp(target, state) != 0) {
            target_exists = &nodes[num_nodes];
            strncpy(target_exists->name, target, sizeof(target_exists->name) - 1);
            target_exists->name[sizeof(target_exists->name) - 1] = '\0';

            target_exists->id = num_nodes;
            target_exists->in_deg = 0;
            target_exists->out_deg = 0;
            num_nodes++;

        }else if(!target_exists && state_exists && strcmp(target, state)==0){
            target_exists = state_exists;

        }

        state_exists->out_deg++;
        target_exists->in_deg++;
        state_exists->edges[target_exists->id]++;

        
        for (int l = 0; l <=size; l++) {
            if (state_exists->target[l] == -1) {
                state_exists->target[l] = target_exists->id;
                break;
            }
        }
        state_exists = NULL;
        target_exists = NULL;
    }
    
    fclose(file);
    
    

    int max_indeg = INT_MIN;
    int min_indeg = INT_MAX;

    int max_outdeg = INT_MIN;
    int min_outdeg = INT_MAX;

    for (int i = 0; i < num_nodes; i++) {
        
        if (nodes[i].in_deg < min_indeg){
            min_indeg = nodes[i].in_deg;
        }

        if (nodes[i].out_deg < min_outdeg){
            min_outdeg = nodes[i].out_deg;
        }

        if (nodes[i].in_deg > max_indeg){
            max_indeg = nodes[i].in_deg;
        }

        if (nodes[i].out_deg > max_outdeg){
            max_outdeg = nodes[i].out_deg;
        }
        
            
        }

    int *stat = (int *) calloc(6, sizeof(int));
    stat[0] = max_indeg;
    stat[1] = max_outdeg;
    stat[2] = min_indeg;
    stat[3] = min_outdeg;
    stat[4] = num_nodes;
    stat[5] = size;

     
    return stat;


}


void random_surfer(nod *nodes, int size, int r_steps, double p) {
    if(nodes==NULL || r_steps==0|| p<=0.0||p>100.0 || size ==0){
        return;
    }
    rand_init();
    int random_node = randu(size);
    int next_node = 0;

    for (int i = 0; i < size; i++) {
        nodes[i].visits = 0;
    }

    for (int i = 0; i < r_steps; i++) {
        double random_val = (double) rand() /RAND_MAX;

        if (p == 1.0 || random_val < p || nodes[random_node].out_deg == 0) {
            random_node = randu(size);
        } else {
            next_node = randu(nodes[random_node].out_deg);
            random_node = nodes[random_node].target[next_node];
        }

        nodes[random_node].visits++;
    }

    for (int i = 0; i < size; i++) {
        double visit_fraction = (double) nodes[i].visits / r_steps;
        printf("%-11s %.10f\n", nodes[i].name, visit_fraction);
    }

    return;
}
void markov(nod *nodes, int size, int m_steps, double p){
    if(nodes==NULL || m_steps==0|| p<=0.0||p>100.0 || size == 0){
        return;
    }
    double matrix[size][size];
    
    double pi[size];

    for (int i = 0; i < size; i++)
    {
        pi[i] = 1.0/size;

        for (int j = 0; j < size; j++)
        {
            if (nodes[i].out_deg ==0)
            {
                matrix[i][j] = 1.0/size;
            }
            else{
                matrix[i][j] = (p/size) + ((1.0-p) * (nodes[i].edges[j]/(double)nodes[i].out_deg));
            }
            
        }
        
    }
    double new_pi[size];// Temporary array to store updated pi values


    for (int i = 0; i < size; i++)
    {
        pi[i] = 1.0/size;}
    for (int step = 0; step < m_steps; step++) {

        for (int j = 0; j < size; j++) {

            new_pi[j] = 0.0;

            for (int k = 0; k < size; k++) {
                new_pi[j] += pi[k] * matrix[k][j];
            }
        }

        // Normalize pi after each step
        double total_sum = 0.0;
        for (int j = 0; j < size; j++) {
            total_sum += new_pi[j];
        }
        for (int j = 0; j < size; j++) {
            pi[j] = new_pi[j] / total_sum;
        }

    }

    for (int i = 0; i < size; i++) {
        printf("%-11s %.10f\n", nodes[i].name, pi[i]);
    }
    
    return;   
}

