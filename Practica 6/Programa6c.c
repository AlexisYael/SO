#include "bibliotecas.h"

void *creoTresHilos(void *params);
void *creoDosHilos(void *params);

int main()
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        //PROCESO HIJO
        pthread_t hilo_0[3];
        printf("Soy el proceso hijo con PID: %d.\n", getpid());
        for (int i = 0; i < 3; i++)
        {

            pthread_create(&hilo_0[i], NULL, creoTresHilos, NULL);
            pthread_t hilo_1[2];
            for (int j = 0; j < 2; j++)
            {
                pthread_create(&hilo_1[j], NULL, creoDosHilos , NULL);
            }
        }
    }
    else if (pid > 0)
    {
        //PROCESOS PADRE
        waitpid(pid, NULL, 0);
        printf("Soy el proceso padre con PID: %d.\n", getpid());
    }
    else
    {
        perror("Hubo un error al crear el proceso.\n");
        exit(-1);
    }
    return 0;
}

void *creoTresHilos(void *params)
{
    printf("Soy el hilo Padre: %d.\n", (int)pthread_self());
    pthread_exit(NULL);
}
void *creoDosHilos(void *params)
{
    printf("Soy el hilo Hijo: %d.\n", (int)pthread_self());
    pthread_exit(NULL);
}