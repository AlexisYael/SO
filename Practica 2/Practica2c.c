#include "bibliotecas.h"

void nivelDos(int numero, pid_t pid);
void nivelTres(int numero, pid_t pid);

int main()
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        printf("Nivel 1 -> Proceso hijo ID: %d - Padre ID: %d\n", getpid(), getppid());
        nivelDos(3, pid);
    }
    else if (pid > 0)
    {
        waitpid(pid, NULL, 0);
        printf("Nivel 0 -> Proceso padre ID: %d - Padre ID: %d\n", getpid(), getppid());
    }
    else
        perror("Hubo un error al crear el proceso\n");

    return 0;
}
/**
 * Función que permite crear un nivel 2 de procesos
 * @param int numero -> Numero de interaciones en recursividad.
 * @param pid_t pid -> ID del proceso.
 **/
void nivelDos(int numero, pid_t pid)
{
    if (numero > 0)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("Nivel 2 -> Proceso hijo ID: %d - Padre ID: %d\n", getpid(), getppid());
            nivelTres(2, pid);
        }
        else if (pid > 0)
        {
            waitpid(pid, NULL, 0);
            nivelDos(numero - 1, pid);
        }
        else
            perror("Hubo un error al crear el proceso\n");
    }
}
/**
 * Función que permite crear un nivel 3 de procesos
 * @param int numero -> Numero de interaciones en recursividad.
 * @param pid_t pid -> ID del proceso.
 **/
void nivelTres(int numero, pid_t pid)
{
    if (numero > 0)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("Nivel 3 -> Proceso hijo ID: %d - Padre ID: %d\n", getpid(), getppid());
        }
        else if (pid > 0)
        {
            waitpid(pid, NULL, 0);
            nivelTres(numero - 1, pid);
        }
        else
            perror("Hubo un error al crear el proceso\n");
    }
}