#include <stdio.h>
#include <time.h>  // For clock_gettime

#define INTERVALS 10000000

double a[INTERVALS], b[INTERVALS];

// Function to get time in seconds
double get_time_in_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // Use CLOCK_MONOTONIC for steady time
    return ts.tv_sec + ts.tv_nsec * 1e-9; // Convert to seconds
}

int main(int argc, char **argv)
{
  double total_time;
  double start_time = get_time_in_seconds(); // Start time using clock_gettime
  double *to = b;
  double *from = a;
  int time_steps = 100;

  /* Set up initial and boundary conditions. */
  from[0] = 1.0;
  from[INTERVALS - 1] = 0.0;
  to[0] = from[0];
  to[INTERVALS - 1] = from[INTERVALS - 1];
  
  for(long i = 1; i < INTERVALS; i++)
    from[i] = 0.0;

  printf("Number of intervals: %ld. Number of time steps: %d\n", INTERVALS, time_steps);

  /* Apply stencil iteratively. */
  while(time_steps-- > 0)
  {
    for(long i = 1; i < (INTERVALS - 1); i++)
      to[i] = from[i] + 0.1*(from[i - 1] - 2*from[i] + from[i + 1]);

    {
      double* tmp;
      tmp = from;
      from = to;
      to = tmp;
    }
  }

  total_time = get_time_in_seconds() - start_time; // Total elapsed time

  printf("Total elapsed time (s) = %f\n", total_time);

  for(long i = 2; i < 30; i += 2)
    printf("Interval %ld: %f\n", i, to[i]);

  return 0;
}
