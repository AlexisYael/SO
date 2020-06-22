#include "bibliotecas.h"

key_t crearKey(char *dir, int tam);
int crearMemoria(int **num, key_t key, int tam);
int crearSemaforo(key_t key, int num_sem);
void down(int semid);
void up(int semid);
void switchOn(int mutex, int rec, int *cuenta);
void switchOff(int mutex, int rec, int *cuenta);
void babuino(int bab, int mut, int *cuenta, int rec);

int main()
{
    key_t key1 = crearKey("/tmp",'j');
    key_t key2 = crearKey("/tmp",'k');
    key_t key3 = crearKey("/tmp",'l');
    key_t key4 = crearKey("/tmp",'m');
    key_t key5 = crearKey("/tmp",'n');
    key_t key6 = crearKey("/tmp",'o');
    key_t key7 = crearKey("/tmp",'p');
    int shmid1, shmid2;
    int *cVa = NULL;
    int *cVi = NULL;
    int mutVa = crearSemaforo(key1, 1);
    int mutVi = crearSemaforo(key2, 1);
    int babVa = crearSemaforo(key3, 1);
    int babVi = crearSemaforo(key4, 1);
    int rec = crearSemaforo(key5, 1);
    shmid1 = crearMemoria(&cVa, key6, 1);
    shmid2 = crearMemoria(&cVi, key7, 1);

    while(1)
    {
        babuino(babVi, mutVi, cVi, rec);
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

void switchOn(int mutex, int rec, int *cuenta)
{
    down(mutex);
    *cuenta = *cuenta + 1;
    if(*cuenta == 1)
        down(rec);
    up(mutex);

}
void switchOff(int mutex, int rec, int *cuenta)
{
    down(mutex);
    *cuenta = *cuenta - 1;
    if(*cuenta == 0)
        up(rec);
    up(mutex);
}
void babuino(int bab, int mut, int *cuenta, int rec)
{   
    down(bab);
    switchOn(mut, rec, cuenta);
    printf("Babuino esta cruzando.\n");
    sleep(1);
    switchOff(mut, rec, cuenta);
    up(bab);
}