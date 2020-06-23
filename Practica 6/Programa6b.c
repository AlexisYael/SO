#include "bibliotecas.h"

void *hilo_incrementa(void *params);
void *hilo_padre(void *params);
void *hilo_disminuye(void *params);

int variable = 0;

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
    pthread_t id_hilo_p[2];
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            pthread_create(&id_hilo_p[i], NULL, hilo_incrementa, NULL);
            pthread_join(id_hilo_p[i], NULL);
        }
        if (i == 1)
        {
            pthread_create(&id_hilo_p[i], NULL, hilo_disminuye, NULL);
            pthread_join(id_hilo_p[i], NULL);
        }
    }
    printf("Hilo padre termian su ejecución y el numero final es: %d.\n", variable);
    pthread_exit(NULL);
}

void *hilo_incrementa(void *params)
{
    int numero = rand() % 21;
    printf("El hilo (%d) incremento en: %d.\n", pthread_self(), numero);
    variable = variable + numero;
}
void *hilo_disminuye(void *params)
{
    int numero = rand() % 21;
    printf("El hilo (%d) decremento en: %d.\n", pthread_self(), numero);
    variable = variable - numero;
}