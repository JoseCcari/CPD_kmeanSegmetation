#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include "kmeans.h"
//#include "image_io.h"

#define DEFAULT_N_CLUSTS 4
#define DEFAULT_IMAGES 20
#define DEFAULT_MAX_ITERS 150
#define DEFAULT_N_THREADS 2
#define DEFAULT_OUT_PATH "result.jpg"

double get_time();
void print_exec(int width, int height, int n_ch, int n_clus, int n_threads, int n_iters, double sse, double exec_time);
int loadImages(int n, int id, int p);
int kmeans(int n_image);

int main ( int argc, char *argv[] )
{
  int id;
  int ierr;
  int n = DEFAULT_IMAGES;
  int p;
  int result;

/*
  Inicializamos MPI.
*/
  ierr = MPI_Init ( &argc, &argv );
/*
  Obtener el numero de procesos.
*/
  ierr = MPI_Comm_size ( MPI_COMM_WORLD, &p );
/*
  Obtener el id del proceso.
*/
  ierr = MPI_Comm_rank ( MPI_COMM_WORLD, &id );

    // ierr = MPI_Bcast ( &n, 1, MPI_INT, 0, MPI_COMM_WORLD );

    result = loadImages ( n, id, p );

    // ierr = MPI_Reduce ( &result, &primes, 1, MPI_INT, MPI_SUM, 0, 
    //   MPI_COMM_WORLD );

  
/*
  Terminate MPI.
*/
  ierr = MPI_Finalize ( );
/*
  Terminate.
*/

  return 0;
}

int loadImages(int n, int id, int p){
    std::cout<<"loading "<<n<<" "<<id<<" "<<p<<" "<<std::endl;
    int images = n/p;
    int first = 1 + (images * id);
    std::cout<<first<<std::endl;
    for(int i = 0; i<images ; i++){
        kmeans(first);
        first++;
    }

    return 1;
}

int kmeans(int n_image)
{
    char *in_path = (char *)malloc( 20 * sizeof(char));
    char *out_path = (char *)malloc( 20 * sizeof(char));
    byte_t *data;
    int width, height, n_ch;
    int n_clus = DEFAULT_N_CLUSTS;
    int n_iters = DEFAULT_MAX_ITERS;
    int n_threads = DEFAULT_N_THREADS;
    int seed = time(NULL);
    double sse, start_time, exec_time;

    // Analizamos argumentos y parámetros opcionales

    strcpy (in_path,std::to_string(n_image).c_str());
    strcat (in_path,"_image.jpg");


    srand(seed);

    // Scanning input image

    data = img_load(in_path, &width, &height, &n_ch);
    std::cout<<in_path<<std::endl;

    // Executing k-means segmentation

    start_time = get_time();
    //Kmeans Kseg_parallel (data, width, height, n_ch, n_clus, &n_iters, &sse, n_threads);
    Kmeans Kseg_parallel (data, width, height, n_ch, n_clus, &n_iters, &sse, n_threads);

    Kseg_parallel.init();
    exec_time = get_time() - start_time;

    // Saving and printing results

    strcpy (out_path,std::to_string(n_image).c_str());
    strcat (out_path,"_result.jpg");

    std::cout<<out_path<<std::endl;

    img_save(out_path, data, width, height, n_ch);
    // print_exec(width, height, n_ch, n_clus, n_threads, n_iters, sse, exec_time);

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
