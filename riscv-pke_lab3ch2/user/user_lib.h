/*
 * header file to be used by applications.
 */

int printu(const char *s, ...);
int exit(int code);
void *naive_malloc();
void naive_free(void *va);
int fork();
void yield();

// lab3_ch2
int sem_new(int n);
int sem_P(int n);
int sem_V(int n);
