#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>

void sleep_ms(int ms){
    struct timespec sleep_time =  {.tv_sec = 0, .tv_nsec = ms * 1000000};
    nanosleep(&sleep_time, NULL);
}

void *create_shared_memory(size_t size) {
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_ANONYMOUS | MAP_SHARED;
    return mmap(NULL, size, prot, flags, 0, 0);
}

sem_t *init_shared_sem(int init_value) {
    sem_t *sem = (sem_t *)create_shared_memory(sizeof(sem_t));
    sem_init(sem, 1, init_value);
    return sem;
}
