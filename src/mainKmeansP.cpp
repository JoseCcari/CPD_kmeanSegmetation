#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include "kmeans.h"
//#include "image_io.h"

#define DEFAULT_N_CLUSTS 4
#define DEFAULT_MAX_ITERS 150
#define DEFAULT_N_THREADS 2
#define DEFAULT_OUT_PATH "result.jpg"

double get_time();
void print_exec(int width, int height, int n_ch, int n_clus, int n_threads, int n_iters, double sse, double exec_time);

int main(int argc, char **argv)
{
    char *in_path = NULL;
    char *out_path = DEFAULT_OUT_PATH;
    byte_t *data;
    int width, height, n_ch;
    int n_clus = DEFAULT_N_CLUSTS;
    int n_iters = DEFAULT_MAX_ITERS;
    int n_threads = DEFAULT_N_THREADS;
    int seed = time(NULL);
    double sse, start_time, exec_time;

    // Analizamos argumentos y parámetros opcionales

    char optchar;
    while ((optchar = getopt(argc, argv, "k:m:o:t:h")) != -1) {
        switch (optchar) {
            case 'k': // num clusters k 
                n_clus = strtol(optarg, NULL, 10);
                break;
            case 'm': // max iterations
                n_iters = strtol(optarg, NULL, 10);
                break;
            case 'o': //image output
                out_path = optarg;
                break;
            case 't': //num threads
                n_threads = strtol(optarg, NULL, 10);
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }
    }

    in_path = argv[optind];

    // Validating input parameters

    if (in_path == NULL) {
        fprintf(stderr, "INPUT ERROR: << Path is NULL >> \n");
        exit(EXIT_FAILURE);
    }

    if (n_clus < 2) {
        fprintf(stderr, "INPUT ERROR: << Invalid number of clusters >> \n");
        exit(EXIT_FAILURE);
    }

    if (n_iters < 1) {
        fprintf(stderr, "INPUT ERROR: << Invalid maximum number of iterations >> \n");
        exit(EXIT_FAILURE);
    }

    if (n_threads < 2) {
        fprintf(stderr, "INPUT ERROR: << Invalid number of threads >> \n");
        exit(EXIT_FAILURE);
    }

    srand(seed);

    // Scanning input image

    data = img_load(in_path, &width, &height, &n_ch);

    // Executing k-means segmentation

    start_time = get_time();
    //Kmeans Kseg_parallel (data, width, height, n_ch, n_clus, &n_iters, &sse, n_threads);
    Kmeans Kseg_parallel (data, width, height, n_ch, n_clus, &n_iters, &sse, n_threads);

    Kseg_parallel.init();
    exec_time = get_time() - start_time;

    // Saving and printing results

    img_save(out_path, data, width, height, n_ch);
    print_exec(width, height, n_ch, n_clus, n_threads, n_iters, sse, exec_time);

    free(data);

    return EXIT_SUCCESS;
}

double get_time()
{
    struct timeval timecheck;

    gettimeofday(&timecheck, NULL);

    return timecheck.tv_sec + timecheck.tv_usec / 1000000.0;
}

void print_exec(int width, int height, int n_ch, int n_clus, int n_threads, int n_iters, double sse, double exec_time)
{
    const char *details = "\nDETALLES DE EJECUCION\n\n"
        "  Image size             : %d x %d\n"
        "  Color channels         : %d\n"
        "  Number of clusters     : %d\n"
        "  Number of threads      : %d\n"
        "  Number of iterations   : %d\n"
        "  Sum of squared errors  : %f\n"
        "  Execution time         : %f\n\n";

    fprintf(stdout, details, width, height, n_ch, n_clus, n_threads, n_iters, sse, exec_time);
}
