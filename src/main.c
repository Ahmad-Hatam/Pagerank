
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "utils.h"
#include <ctype.h>

#include <string.h>



int main(int argc, char *const *argv) {

if (argc < 2) {
        exit(1);
    }
    int option;
    int r_steps = 0;
    double p = 0.10;
    int show_stats = 0;
    int r_surfer = 0;
    int m_markov = 0;
    int m_steps = 0;
    int h_flag = 0;


    while ((option = getopt(argc, argv, "hr:m:sp:")) != -1) {
        switch (option) {
            case 'h':
                // Handle -h option
                h_flag = 1;
                printf("Usage: ./pagerank [OPTION].....[FILENAME]\n");
                printf("Perform pagerank computations for a given file in the DOT format\n");
                printf("\n");
                printf("-h    Print a brief overview of the available command line parameters\n");
                printf("-r N  Simulate N steps of the random surfer and output the result\n");
                printf("-m N  Simulate N steps of the Markov chain and output the result\n");
                printf("-s    Compute and print the statistics of the graph as defined\n");
                printf("        in section 3.4\n");
                printf("-p P  Set the parameter p to P%%. (Default: P = 10)\n");
                exit(0);
                
            
            case 'r':
                // initialize the random number generator
                r_steps = atoi(optarg);
                if(r_steps <= 0){
                    exit(1);
                }
                r_surfer = 1;
                break;

            case 's':
                // if (optind < argc) {
                
                show_stats= 1;

                // }
                break;
            

            case 'm':
                m_markov = 1;
                m_steps = atoi(optarg);
                if(m_steps <= 0){
                    exit(1);
                }
                break;

            case 'p':
                if (optarg[0] == '-') {
                //     // If there's no space after -p, the value is attached to it
                //     // Skip the '-p' character and convert the rest to a double
                    p = atof(optarg + 2); // Skip '-p' and convert the rest
                } else {
                p = atof(optarg); // Use atof to parse floating point number
                }
                if (p<1.0 || p > 100.0){
                    exit(1);
                }
                p /= 100.0;
                break;
            // default:
            //     exit(1); // Unknown option
        }
    }
    

    if (!h_flag && !r_surfer && !m_markov && !show_stats) {
        exit(1);
    }

    if (optind >= argc) {
        exit(1);
    }


    char *filename = argv[optind];
    if(filename == NULL && !h_flag){
        exit(1);
    }
    char *dot = strrchr(filename, '.');
    if (dot == NULL || strcmp(dot, ".dot") != 0) {
        exit(1); 
    }


    FILE *file = fopen(filename, "r");
    if (file == NULL){
        exit(1);
    }

    char format[500], file_identifier[500], state[500], arrow[500], target[500];
    char bracket;
    char line[520];


    if (fgets(line, sizeof(line), file)){
        if(sscanf(line, "%s %s %c", format, file_identifier, &bracket) != 3 ||
            strcmp(format, "digraph") != 0 || bracket != '{'|| strlen(file_identifier)>256) {
            fclose(file);
            exit(1);

        }
        if(strlen(line)> 519){
            fclose(file);
            exit(1);
        }
        }
    int file_size = 0;
    while (fgets(line, sizeof(line), file)){
        if(sscanf(line, "%s %s %s", state, arrow, target) == 3){
        file_size++;
        if (!isalpha(state[0]) || !isalpha(target[0])|| strlen(state)> 256 || strlen(target)> 257|| strlen(arrow)> 2) {
            fclose(file);
            exit(1); // Terminate with return code 1
        }
        }
        if (strlen(line)>519){
            fclose(file);
            exit(1);
        }
        }  
    fclose(file);
    
    if(file_size == 0 && show_stats == 1){
        printf("%s: \n", file_identifier);
        printf("- num nodes: 0\n");
        printf("- num edges: 0\n");
        printf("- indegree: 0-0\n");
        printf("- outdegree: 0-0\n");
        exit(0);
        }
    else if (file_size == 0){
        exit(0);
    }
    
    int max_nodes = 2 * file_size;
    
    
    nod *nodes = (nod *)calloc(max_nodes, sizeof(nod));
    if(nodes==NULL){
        exit(1);
    }
    
    for (int i = 0; i < max_nodes; i++) {
        nodes[i].edges = (int *)calloc(file_size, sizeof(int));
        nodes[i].target = (int*)calloc(file_size, sizeof(int));
        for (int j = 0; j < file_size; j++) {
            nodes[i].target[j] = -1; 
        }
    }

    
    
    int *stat = collection(nodes, filename, file_size);
    if(stat==NULL){
        exit(1);
    }
    if (show_stats == 1){
    printf("%s: \n", file_identifier);
    printf("- num nodes: %d\n", stat[4]);
    printf("- num edges: %d\n", stat[5]);
    printf("- indegree: %d-%d\n", stat[2],stat[0]);
    printf("- outdegree: %d-%d\n", stat[3],stat[1]);

    }

    if (r_surfer == 1){
        random_surfer(nodes, stat[4],r_steps, p);
    }

    if (m_markov ==1)
    {
        markov(nodes, stat[4],m_steps,p);
    }
    

    free(stat);
    for (int i = 0; i < max_nodes; i++) {
        free(nodes[i].target);
        free(nodes[i].edges);
    }
    free(nodes);


    dot = NULL;
    filename = NULL;
    file= NULL;


    exit(0);
}
