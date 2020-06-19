#include "bibliotecas.h"

int main(){
    pid_t pid;
    pid = fork(); //Llamada al sistema que crea un proceso hijo.
    if(pid == -1)
        perror("Error al crear proceso");
    else if (pid == 0)
        printf("Soy el proceso hijo y mi PID es: %d y mi padre es: %d \n",getpid(),getppid());
    else
        printf("Soy el proceso padre y mi PID es: %d y mi padre es: %d \n",getpid(),getppid());
    return 0;
}