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
    int shmid;
    key_t key1 = crearKey("/tmp", 'a');
    key_t key2 = crearKey("/tmp", 'b');
    key_t key3 = crearKey("/tmp", 'c');
    key_t key4 = crearKey("/tmp", 'd');
    int cliente = crearSemaforo(key1, 1);
    int barbero = crearSemaforo(key2, 1);
    int mutex = crearSemaforo(key3, 1);
    shmid = crearMemoria(&espera, key4, 1);
    *espera = 0;
    while (1)
    {
        printf("LLego un nuevo cliente.\n");
        down(mutex);
        if (*espera < SILLAS)
        {
            if (*espera == 0)
                printf("El cliente desperto al barbero.\n");
            *espera = *espera + 1;
            up(cliente);
            up(mutex);
            down(barbero);
            printf("Estoy recibiendo mi corte.\n");
            sleep(2);
        }
        else
        {
            printf("El cliente se retira al noi haber espacio disponible.\n");
            up(mutex);
        }
        
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