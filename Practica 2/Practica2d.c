#include "bibliotecas.h"

void nivelUno(int numero, pid_t pid, FILE *archivo);

int main()
{
    pid_t pid;
    int status;
    char mensaje[] = "Hola esta es mi practica uno";
    FILE *archivo = fopen("archivo.txt", "w");
    if (pid == 0)
    {
        printf("Nivel 0 -> Proceso hijo ID: %d - Padre ID: %d\n", getpid(), getppid());
    }
    else if (pid > 0)
    {
        nivelUno(5, pid, archivo);
    }
    else
        perror("Hubo un error al crear el proceso\n");

    return 0;
}
void nivelUno(int numero, pid_t pid, FILE *archivo)
{
    int i = 0;
    if (numero > 0)
    {
        pid = fork();
        if (pid == 0)
        {
            printf("Nivel 1 -> Proceso hijo ID: %d - Padre ID: %d\n", getpid(), getppid());
            switch (numero)
            {
            case 5:
                fprintf(archivo, "Hola ");
                break;
            case 4:
                fprintf(archivo, "esta ");
                break;
            case 3:
                fprintf(archivo, "es ");
                break;
            case 2:
                fprintf(archivo, "mi ");
                break;
            case 1:
                fprintf(archivo, "practica ");
                break;
            default:
                break;
            }
        }
        else if (pid > 0)
        {
            waitpid(pid, NULL, 0);
            nivelUno(numero - 1, pid, archivo);
        }
        else
            perror("Hubo un error al crear el proceso\n");
    }
    else
    {
        printf("Nivel 0 -> Proceso padre ID: %d - Padre ID: %d\n", getpid(), getppid());
        fprintf(archivo, "uno.");
    }
}