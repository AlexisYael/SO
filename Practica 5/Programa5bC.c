#include "bibliotecas.h"
#define SILLAS 5

key_t crearKey(char *dir, int tam);
int crearMemoria(int **num, key_t key, int tam);
int crearSemaforo(key_t key, int num_sem);
void down(int semid);
void up(int semid);

int main()
{
    int *espera = NULL;
    int *actual = NULL;
    int shmid, shmid2;
    key_t key1 = crearKey("/tmp", 'm');
    key_t key2 = crearKey("/tmp", 'o');
    key_t key3 = crearKey("/tmp", 'p');
    key_t key4 = crearKey("/tmp", 'q');
    key_t key5 = crearKey("/tmp", 'r');
    int clientes = crearSemaforo(key1, 1);
    int barbero = crearSemaforo(key2, 1);
    int mutex = crearSemaforo(key3, 1);
    shmid = crearMemoria(&espera, key4, 1);
    shmid2 = crearMemoria(&actual, key5, 1);
    while (1)
    {
        down(mutex);
        if (*espera > 0 && *espera <= SILLAS)
        {
            *espera = *espera + 1;
            printf("Entro cliente.\n");
            up(clientes);
            up(mutex);
            if (*actual == 0)
            {
                *actual = 1;
                down(barbero);
                for (int i = 0; i < 5; i++)
                {
                    printf("Me estan cortando el cabello.\n");
                    sleep(1);
                }
                *actual = 0;
            }
            else
            {
                printf("Espero mi turno.\n");
                sleep(1);
            }
        }
        else
            up(mutex);
    }
    return 0;
}

key_t crearKey(char *dir, int tam)
{
    key_t key;
    if ((key = ftok(dir, tam)) == (key_t)-1)
    {
        printf("Error al crear la llave.\n");
        exit(-1);
    }
    return key;
}
int crearMemoria(int **num, key_t key, int tam)
{
    int shmid;
    if ((shmid = shmget(key, tam * sizeof(int), IPC_CREAT | IPC_EXCL | 0600)) == -1)
    {
        if (errno == EEXIST)
        {
            printf("La memoria existe.\n");
            if ((shmid = shmget(key, sizeof(int), IPC_EXCL)) == -1)
            {
                printf("Error al atar la memoria.\n");
                exit(-1);
            }
            printf("Se conecto a la memoria: %d.\n", shmid);
        }
        else
        {
            printf("Error al crear la memoria.\n");
            exit(-1);
        }
    }
    else
    {
        printf("Se creo la memoria: %d.\n", shmid);
    }
    // Atamos la memoria creada
    if ((*num = shmat(shmid, 0, 0)) == NULL)
    {
        printf("Error al atar la memoria.\n");
        exit(-1);
    }
    return shmid;
}
int crearSemaforo(key_t key, int num_sem)
{
    int semid;
    if ((semid = semget(key, num_sem, IPC_CREAT | IPC_EXCL | 0600)) == -1)
    {
        if (errno == EEXIST)
        {
            printf("El semaforo existe.\n");
            if ((semid = semget(key, num_sem, IPC_EXCL)) == -1)
            {
                printf("Error al atar semaforo.\n");
                exit(-1);
            }
            printf("Se conecto el semaforo: %d.\n", semid);
        }
        else
        {
            printf("Error al crear semaforo.\n");
            exit(-1);
        }
    }
    else
    {
        printf("Se creo el semaforo: %d.\n", semid);
        semctl(semid, 0, SETVAL, 1);
        return (semid);
    }
}
void down(int semid)
{
    struct sembuf op_p[] = {0, -1, 0};
    semop(semid, op_p, 1);
}
void up(int semid)
{
    struct sembuf op_p[] = {0, +1, 0};
    semop(semid, op_p, 1);
}