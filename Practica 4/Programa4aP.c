#include "bibliotecas.h"

int crearMemoria(int **ptr, key_t key);
void eliminarMemoria(int *ptr, int shmid);
void regionCritica(int pid);
void regionCriticaPadre(int pid);
void regionCriticaHijo(int pid);
key_t crearKey(char *dir, int tam);

int main()
{
    int shmid1;
    int shmid2;
    int shmid3;
    int *hijo_desea_entrar = NULL;
    int *padre_desea_entrar = NULL;
    int *proceso_favorecido = NULL;
    key_t key1 = crearKey("/tmp",'o');
    key_t key2= crearKey("/tmp",'p');
    key_t key3 = crearKey("/tmp",'q');
    shmid1 = crearMemoria(&hijo_desea_entrar,key1);
    shmid2 = crearMemoria(&padre_desea_entrar,key2);
    shmid3 = crearMemoria(&proceso_favorecido,key3);

    *hijo_desea_entrar = 1;
    *padre_desea_entrar = 0;
    *proceso_favorecido = 1;

    while (1)
    {
        *padre_desea_entrar = 1;
        while(*hijo_desea_entrar)
            if(*proceso_favorecido == 1)
            {
                *padre_desea_entrar = 0;
                while(*proceso_favorecido == 1);
                *padre_desea_entrar = 1;
            }
        regionCritica(getpid());
        *proceso_favorecido = 1;
        *padre_desea_entrar = 0;
        regionCriticaPadre(getpid());  
    }
    return 0;
}

int crearMemoria(int **num, key_t key)
{
    int shmid;
    if ((shmid = shmget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0600)) == -1)
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
void regionCritica(int pid)
{
    printf("El proceso %d entro en la region crtitica.\n", pid);
    for (int i = 0; i < 3; i++)
    {
        printf("Region critica %d\n", i);
        sleep(1);
    }
}
void regionCriticaHijo(int pid)
{
    printf("El proceso Hijo (%d) entro en la region NO crtitica.\n", pid);
    for (int i = 0; i < 5; i++)
    {
        printf("Hijo en region no critica contando: %d\n", i);
        sleep(1);
    }
}
void regionCriticaPadre(int pid)
{
    printf("El proceso Padre (%d) entro en la region NO crtitica.\n", pid);
    for (int i = 0; i < 10; i++)
    {
        printf("Padre en region no critica contando: %d\n", i);
        sleep(1);
    }
}