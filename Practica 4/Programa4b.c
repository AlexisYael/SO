#include "bibliotecas.h"

#define FALSE 0
#define TRUE 1
#define N 2

void seccionCritica(int pid);
void seccionNoCriticaHijo(int pid);
void seccionNoCriticaPadre(int pid);
int crearMemoria(int **ptr, key_t key,int tam);
void eliminarMemoria(int *ptr, int shmid);
key_t crearKey(char *dir, int tam);

int main()
{
    pid_t pid;
    int shmid1, shmid2;
    int *turno = NULL;
    int *interesado = NULL;

    key_t key1 = crearKey("/tmp",'a');
    key_t key2= crearKey("/tmp",'b');

    shmid1 = crearMemoria(&turno, key1, 1);
    shmid2 = crearMemoria(&interesado, key2, N);

    interesado[0] = FALSE;
    interesado[1] = FALSE;

    pid = fork();
    if (pid == 0)
    {
        while (1){
                interesado[0] == TRUE;
                *turno = 1;
                while (interesado[1] && *turno == 1);
                seccionCritica(getpid());
                interesado[0] = FALSE;
                seccionNoCriticaHijo(getpid());
            }
    }
    else if (pid > 0)
    {
        while (1){
                interesado[1] == TRUE;
                *turno = 0;
                while (interesado[0] && *turno == 0);
                seccionCritica(getpid());
                interesado[1] = FALSE;
                seccionNoCriticaPadre(getpid());
            }
    }
    else
        perror("Hubo un error al crear el proceso\n");
    return 0;
}

void seccionCritica(int pid)
{
    printf("El proceso %d entro en la zona critica.\n", pid);
    for (int i = 0; i < 3; i++)
    {
        printf("Cuenta critica %d.\n", i);
        sleep(1);
    }
}

void seccionNoCriticaPadre(int pid)
{
    printf("El proceso padre (%d) entro en la zona NO critica.\n", pid);
    for (int i = 0; i < 15; i++)
    {
        printf("Cuenta NO critica padre %d.\n", i);
        sleep(1);
    }
}

void seccionNoCriticaHijo(int pid)
{
    printf("El proceso hijo (%d) entro en la zona NO critica.\n", pid);
    for (int i = 0; i < 5; i++)
    {
        printf("Cuenta NO critica hijo %d.\n", i);
        sleep(1);
    }
}
int crearMemoria(int **num, key_t key,int tam)
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