#include "bibliotecas.h"

void *hilo_hijo(void *params);
void *hilo_padre(void *params);

int main()
{
    pthread_t hilo_p;
    pthread_create(&hilo_p, NULL, hilo_padre, NULL);
    pthread_join(hilo_p, NULL);
    return 0;
}

void *hilo_padre(void *params)
{
    printf("Soy el Hilo padre %d.\n ", pthread_self());
    printf("Creare dos Hilos...\n ");
    pthread_t id_hilo_p;
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&id_hilo_p, NULL, hilo_hijo, NULL);
        pthread_join(id_hilo_p, NULL);
    }
    printf("Hilo padre termian su ejecución.\n");
    pthread_exit(NULL);
}

void *hilo_hijo(void *params)
{
    printf("Soy un Hilo hijo %d\n", pthread_self());
}