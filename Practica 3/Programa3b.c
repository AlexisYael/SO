#include "bibliotecas.h"

void nivelUno(int numero, pid_t pid, float *num, int m);
int crearMemoria(key_t key);
void eliminarMemoria(float *ptr, int shmid);
float *atarMemoria(int shmid, float **ptr);

int main()
{
    pid_t pid;
    key_t key;
    float *num = NULL;
    int shmid, m;
    key = ftok("mem", 'M');
    shmid = crearMemoria(key);
    atarMemoria(shmid, &num);
    printf("Ingrese el numero de interaciones para los procesos hijos: ");
    scanf("%d", &m); //Número de interaciones que hara los incrementos
    *num = rand() % 11; //Iniciación de la variable compartida por el padre de menra aleatoria
    if (pid == 0)
    {
        printf("Nivel 0 -> Proceso hijo ID: %d - Padre ID: %d\n", getpid(), getppid());
    }
    else if (pid > 0)
    {
        nivelUno(5, pid, num, m);
    }
    else
        perror("Hubo un error al crear el proceso\n");

    return 0;
}

/**
 * Función que permite crear un nivel 1 de procesos
 * @param int numero -> Numero de interaciones en recursividad.
 * @param pid_t pid -> ID del proceso.
 **/
void nivelUno(int numero, pid_t pid, float *num, int m)
{
    int i = 0;
    if (numero > 0)
    {
        pid = fork();
        if (pid == 0)
        {
            switch (numero)
            {
            case 5:
                while (numero != 5);
                printf("Proceso (%d) entro en zona critica a incrementar\n", getpid());
                *num = *num + (numero * m); //numero sera el incremento que hara el proceso
                sleep(1);
                printf("Proceso (%d) entro en zona no critica a esperar\n", getpid());
                break;
            case 4:
                while (numero != 4);
                printf("Proceso (%d) entro en zona critica a incrementar\n", getpid());
                *num = *num + (numero * m); //numero sera el incremento que hara el proceso
                sleep(1);
                printf("Proceso (%d) entro en zona no critica a esperar\n", getpid());
                break;
            case 3:
                while (numero != 3);
                printf("Proceso (%d) entro en zona critica a incrementar\n", getpid());
                *num = *num + (numero * m); //numero sera el incremento que hara el proceso
                sleep(1);
                printf("Proceso (%d) entro en zona no critica a esperar\n", getpid());
                break;
            case 2:
                while (numero != 2);
                printf("Proceso (%d) entro en zona critica a incrementar\n", getpid());
                *num = *num + (numero * m); //numero sera el incremento que hara el proceso
                sleep(1);
                printf("Proceso (%d) entro en zona no critica a esperar\n", getpid());
                break;
            case 1:
                while (numero != 1);
                printf("Proceso (%d) entro en zona critica a incrementar\n", getpid());
                *num = *num + (numero * m); //numero sera el incremento que hara el proceso
                sleep(1);
                printf("Proceso (%d) entro en zona no critica a esperar\n", getpid());
                break;
            default:
                break;
            }
        }
        else if (pid > 0)
        {
            waitpid(pid, NULL, 0);
            nivelUno(numero - 1, pid, num, m);
        }
        else
            perror("Hubo un error al crear el proceso\n");
    }
    else
    {
        printf("El numero final es: %f.\n", *num);
    }
}
int crearMemoria(key_t key)
{
    int shmid;
    if ((shmid = shmget(key, sizeof(float), IPC_CREAT | 0600)) == -1)
    {
        printf("Error al crear la memoria.\n");
        exit(-1);
    }
    printf("Se creo la memoria: %d.\n", shmid);
    return (shmid);
}
float *atarMemoria(int shmid, float **ptr)
{
    if ((*ptr = shmat(shmid, 0, 0)) == NULL)
    {
        printf("Error al atar la memoria.\n");
        exit(-1);
    }
    printf("Se ato de manera correcta la memoria.\n");
}
void eliminarMemoria(float *ptr, int shmid)
{
    shmdt(ptr);
    if (shmctl(shmid, IPC_RMID, 0) == -1)
    {
        printf("Error al eliminar la memoria.\n");
        exit(-1);
    }
    printf("Se elimino de manera correcta la memoria.\n");
}