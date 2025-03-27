#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
  u32 remaining_time;
  u32 total_run_time;
  u32 waiting_time;
  bool ran;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;
/* Your code here */
u32 time = 0;
u32 completed = 0;
struct process *temp;

  // Initialize processes arriving at time 0
  for (u32 i = 0; i < size; i++) {
      if (data[i].arrival_time == 0) {
          struct process *new_process = malloc(sizeof(struct process));
          if (new_process == NULL) {
              perror("malloc");
              exit(EXIT_FAILURE);
          }
          *new_process = data[i];
          new_process->remaining_time = data[i].burst_time;
          new_process->total_run_time = 0;
          new_process->ran = false;
          new_process->waiting_time = 0;
          TAILQ_INSERT_TAIL(&list, new_process, pointers);
      }
  }

  while (completed < size) {
      if (!TAILQ_EMPTY(&list)) {
          temp = TAILQ_FIRST(&list);

          // Calculate response time only when process first runs
          if (!temp->ran) {
              temp->ran = true;
              total_response_time += (time - temp->arrival_time);
          }

          temp->remaining_time--;
          temp->total_run_time++;

          // Update waiting time only for processes that have arrived
          struct process *iter;
          TAILQ_FOREACH(iter, &list, pointers) {
              if (iter != temp) {
                  iter->waiting_time++;
              }
          }

          time++;

          // Check for new arrivals after incrementing time
          for (u32 i = 0; i < size; i++) {
              if (data[i].arrival_time == time) {
                  struct process *new_process = malloc(sizeof(struct process));
                  if (new_process == NULL) {
                      perror("malloc");
                      exit(EXIT_FAILURE);
                  }
                  *new_process = data[i];
                  new_process->remaining_time = data[i].burst_time;
                  new_process->total_run_time = 0;
                  new_process->ran = false;
                  new_process->waiting_time = 0;
                  TAILQ_INSERT_TAIL(&list, new_process, pointers);
              }
          }

          if (temp->remaining_time == 0) {
              total_waiting_time += temp->waiting_time;
              TAILQ_REMOVE(&list, temp, pointers);
              completed++;
              free(temp);
          }
          else if (temp->total_run_time == quantum_length) {
              temp->total_run_time = 0;
              TAILQ_REMOVE(&list, temp, pointers);
              TAILQ_INSERT_TAIL(&list, temp, pointers);
          }
      }
      else {
          time++;
          // Check for new arrivals in idle time
          for (u32 i = 0; i < size; i++) {
              if (data[i].arrival_time == time) {
                  struct process *new_process = malloc(sizeof(struct process));
                  if (new_process == NULL) {
                      perror("malloc");
                      exit(EXIT_FAILURE);
                  }
                  *new_process = data[i];
                  new_process->remaining_time = data[i].burst_time;
                  new_process->total_run_time = 0;
                  new_process->ran = false;
                  new_process->waiting_time = 0;
                  TAILQ_INSERT_TAIL(&list, new_process, pointers);
              }
          }
      }
  }
  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
