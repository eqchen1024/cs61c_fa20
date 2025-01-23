#include "coordinator.h"
#include <mpi.h>
#define TERMINATE -1
#define READY 0


int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Error: not enough arguments\n");
    printf("Usage: %s [path_to_task_list]\n", argv[0]);
    return -1;
  }

  // Read and parse task list file
  int num_tasks;
  task_t **tasks;
  if (read_tasks(argv[1], &num_tasks, &tasks)) {
    printf("Error reading task list from %s\n", argv[1]);
    return -1;
  }

  MPI_Init(&argc, &argv);
  int procID, totalProcs;
  MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &procID);
  if (procID == 0) {
    // manager node
    int nextTask = 0; // next task to do、
    MPI_Status status;
    int32_t message;
    while (nextTask < num_tasks) {
      //printf("plan next task: %d\n",nextTask);
      MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      int sourceProc = status.MPI_SOURCE;
      message = nextTask;
      MPI_Send(&message, 1, MPI_INT32_T, sourceProc, 0, MPI_COMM_WORLD);
      nextTask++;
    }
    for (int i = 0; i < totalProcs - 1; i++) {
      //printf("terminate worker: %d\n",i);
      MPI_Recv(&message, 1, MPI_INT32_T, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      int sourceProc = status.MPI_SOURCE;
      message = TERMINATE;
      MPI_Send(&message, 1, MPI_INT32_T, sourceProc, 0, MPI_COMM_WORLD);
    }
    free(tasks);
  } else {
    // worker node
    int32_t message;
    while (true) {
      message = READY;
      MPI_Send(&message, 1, MPI_INT32_T, 0, 0, MPI_COMM_WORLD);
      MPI_Recv(&message, 1, MPI_INT32_T, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      if (message == TERMINATE) break; // all done!
      //printf("worker do task: %d",message);
      execute_task(tasks[message]);
      free(tasks[message]->path);
      free(tasks[message]);
    }
  }
  MPI_Finalize(); // clean up
}

		  
	
  // Execute tasks
  // need to be modified to be PLP
  // for (int i = 0; i < num_tasks; i++) {
    // if (execute_task(tasks[i])) {
      // printf("Task %d failed\n", i);
      // return -1;
    // }
    // free(tasks[i]->path);
    // free(tasks[i]);
  // }
  // free(tasks);

