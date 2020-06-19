#include "bibliotecas.h"

void regionCriticaPadre(int PID, float *array);
void regionCriticaHijo(int PID, float *array);
void regionNoCritica(int PID);
int crearMemoria(key_t key);
float *atarMemoria(int shmid, float **ptr);
void eliminarMemoria(float *ptr ,int shmid);

int main()
{
    pid_t pid;
    key_t key;
    int shmid, i , PID;
    float *array = NULL;
    key = ftok("mem", 'M');
    shmid = crearMemoria(key);
    atarMemoria(shmid, &array);
    for (i = 0; i < 10; i++)
        array[i] = 0;
    pid = fork();
    if (pid == 0)
    {
        //PROCESO HIJO
        while(1){
            while(array[9] != 0);
            PID = getpid();
            regionCriticaHijo(PID,array);
            printf("El proceso hijo (%d) entro en zona no critica.\n",PID);
        }
    }
    else if (pid > 0)
    {
        //PROCESOS PADRE
        while(1){
            while(array[9] == 0);
            PID = getpid();
            regionCriticaPadre(PID,array);
            for(int i = 0; i < 10; i++)
                array[i] = 0;
            printf("El proceso padre (%d) entro en zona no critica.\n",PID);
        }
    }
    else
    {
        perror("Hubo un error al crear el proceso.\n");
        exit(-1);
    } 
    eliminarMemoria(array, shmid);
    return 0;
}

void regionCriticaPadre(int PID, float *array){
    printf("Comienza a leer datos el proceso padre (%d)...\n",PID);
    for(int i = 0; i < 10; i++){
        printf("%f.\n", array[i]);
        sleep(1);
    }
}
void regionCriticaHijo(int PID, float *array){
    printf("Comienza a grabar datos el proceso hijo (%d)...\n", PID);
    for(int i = 0; i < 10; i++){
        array[i] = ((float)rand()/(float)(RAND_MAX)) * 10;
        sleep(1);
    }
}
int crearMemoria(key_t key)
{
    int shmid;
    if ((shmid = shmget(key, 10 * sizeof(float), IPC_CREAT | 0600)) == -1)
    {
        printf("Error al crear la memoria.\n");
        exit(-1);
    }
    printf("Se creo la memoria: %d.\n", shmid);
    return (shmid);
}
float *atarMemoria(int shmid, float **ptr)
{
    if ((*ptr = shmat(shmid, 0, 0)) == NULL){
        printf("Error al atar la memoria.\n");
        exit(-1);
    }
    printf("Se ato de manera correcta la memoria.\n");
}
void eliminarMemoria(float *ptr ,int shmid){
    shmdt(ptr);
    if(shmctl(shmid, IPC_RMID, 0) == -1){
        printf("Error al eliminar la memoria.\n");
        exit(-1);
    }
    printf("Se elimino de manera correcta la memoria.\n");
}