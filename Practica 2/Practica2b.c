#include "bibliotecas.h"

int main()
{
    pid_t pid;
    //Interación para crear los 10 procesos hijos 
    for (int i = 0; i < 11; i++)
    {
        if (pid == -1)
            perror("Error al crear proceso");
        else if (pid == 0)
        {
            printf("Soy el proceso hijo, mi PID es: %d, mi padre es: %d y mi conteo es: %d\n", getpid(), getppid(), i);
            break;
        }
        else
        {
            //El if permite que no muestre el padre por cada hijo creado.
            if (i == 0)
                printf("Soy el proceso padre y mi PID es: %d y mi padre es: %d \n", getpid(), getppid());
            pid = fork();
            sleep(1);
        }
    }
}