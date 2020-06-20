#include "bibliotecas.h"

key_t crearKey(char *dir, int tam);
int crearMemoria(int **num, key_t key, int tam);
int crearSemaforo(key_t key, int num_sem);
void down(int semid, int num_sem);
void up(int semid, int num_sem);
void pensar(int num_fil, int *estado);
void comer(int num_fil, int *estado);
void tomarTenedores(int i, int mutex, int *estado, int sem);
void probar(int i, int *estado, int sem);
void ponerTenedores(int i, int mutex, int *estado, int sem);

#define N 5
#define IZQUIERDO (i + N - 1) % N
#define DERECHO (i + 1) % N
#define PENSANDO 0
#define HAMBRIENTO 1
#define COMIENDO 2

int main()
{
    int *estado = NULL;
    key_t key1 = crearKey("/tmp", 'f');
    key_t key2 = crearKey("/tmp", 'g');
    key_t key3 = crearKey("/tmp", 'h');
    int mutex = crearSemaforo(key1, 1);
    int sem = crearSemaforo(key2, 5);
    int shmid = crearMemoria(&estado, key3, 5);

    while (1)
    {
        pensar(4, estado);
        tomarTenedores(4, mutex, estado, sem);
        comer(4, estado);
        ponerTenedores(4, mutex, estado, sem);
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
        if (num_sem == 1)
            semctl(semid, 0, SETVAL, 1);
        else
        {
            semctl(semid, 0, SETVAL, 0);
            semctl(semid, 1, SETVAL, 0);
            semctl(semid, 2, SETVAL, 0);
            semctl(semid, 3, SETVAL, 0);
            semctl(semid, 4, SETVAL, 0);
        }
        return (semid);
    }
}
void eliminarMemoria(int *ptr, int shmid)
{
    shmdt(ptr);
    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        printf("Error al eliminar la memoria.\n");
        exit(-1);
    }
    printf("Se elimino de manera correcta la memoria.\n");
}
void down(int semid, int num_sem)
{
    struct sembuf op_p[] = {num_sem, -1, 0};
    semop(semid, op_p, N);
}
void up(int semid, int num_sem)
{
    struct sembuf op_p[] = {num_sem, +1, 0};
    semop(semid, op_p, N);
}
void pensar(int num_fil, int *estado)
{
    for (int i = 0; i < 5; i++)
    {
        if (estado[i] == PENSANDO)
        {
            printf("Filosofo %d pensando...\n", num_fil + 1);
            sleep(1);
        }
        if (estado[i] == HAMBRIENTO)
        {
            printf("Filosofo %d hambriento...\n", num_fil + 1);
            sleep(1);
        }
    }
}
void comer(int num_fil, int *estado)
{
    for (int i = 0; i < 5; i++)
    {
        if (estado[i] == HAMBRIENTO)
        {
            printf("Filosofo %d hambriento...\n", num_fil + 1);
            sleep(1);
        }
        if (estado[i] == COMIENDO)
        {
            printf("Filosofo %d comiendo...\n", num_fil + 1);
            sleep(1);
        }
    }
}
void tomarTenedores(int i, int mutex, int *estado, int sem)
{
    down(mutex, 0);
    estado[i] = HAMBRIENTO;
    probar(i, estado, sem);
    up(mutex, 0);
    down(sem, i);
}
void probar(int i, int *estado, int sem)
{
    if (estado[i] == HAMBRIENTO && estado[IZQUIERDO] != COMIENDO && estado[DERECHO] != COMIENDO)
    {
        estado[i] = COMIENDO;
        up(sem, i);
    }
}
void ponerTenedores(int i, int mutex, int *estado, int sem)
{
    down(mutex, 0);
    estado[i] = PENSANDO;
    probar(IZQUIERDO, estado, sem);
    probar(DERECHO, estado, sem);
    up(mutex, 0);
}
