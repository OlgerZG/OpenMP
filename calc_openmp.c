#include <stdio.h>
#include <omp.h>  // Include OpenMP for parallelization and timing

#define INTERVALS 10000000

double a[INTERVALS], b[INTERVALS];

int main(int argc, char **argv)
{
  // Limit the number of threads to 8
  omp_set_num_threads(16);

  double total_time, loop_time;
  double start_time = omp_get_wtime(); // Start time using wall clock
  double *to = b;
  double *from = a;
  int time_steps = 100;

  /* Set up initial and boundary conditions. */
  from[0] = 1.0;
  from[INTERVALS - 1] = 0.0;
  to[0] = from[0];
  to[INTERVALS - 1] = from[INTERVALS - 1];
  
  // #pragma omp parallel for
  for(long i = 1; i < INTERVALS; i++)
    from[i] = 0.0;

  printf("Number of intervals: %ld. Number of time steps: %d\n", INTERVALS, time_steps);

  /* Apply stencil iteratively. */
  double loop_start_time = omp_get_wtime(); // Start time for the iteration
  while(time_steps-- > 0)
  { 
    // Chunk Size ≈ INTERVALS / Number of Threads
    //#pragma omp parallel for
    //#pragma omp parallel for schedule(static)  # this is also valid 
    #pragma omp parallel for schedule(static, 625000 ) 
    for(long i = 1; i < (INTERVALS - 1); i++)
      to[i] = from[i] + 0.1*(from[i - 1] - 2*from[i] + from[i + 1]);

    {
      #pragma omp single // implicit barrrier
      {
        double* tmp;
        {
          tmp = from;
          from = to;
          to = tmp;
        }
      }
    }
  }
  loop_time = omp_get_wtime() - loop_start_time; // Elapsed time for this iteration
  printf("Time for step %d: %f seconds\n", 100 - time_steps, loop_time);


  total_time = omp_get_wtime() - start_time; // Total elapsed wall clock time

  printf("Total elapsed time (s) = %f\n", total_time);

  for(long i = 2; i < 30; i += 2)
    printf("Interval %ld: %f\n", i, to[i]);

  return 0;
}
