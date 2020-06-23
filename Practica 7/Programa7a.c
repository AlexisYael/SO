#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    NO,
    SI
} boolean_t;

typedef struct
{
    unsigned id, T, C, D, P, R;
    boolean_t planificable;
} tarea_t;

void dms(tarea_t *tareas, int N)
{
#define redondeo_al_alza(n, d) (((n) % (d) == 0) ? ((n) / (d)) : ((n) / (d) + 1))
    int i, j;
    tarea_t aux;
    int w, w_ant, I;

    for (i = 0; i < N - 1; i++)
        for (j = i + 1; j < N; j++)
            if (tareas[i].D < tareas[j].D)
            {
                aux = tareas[i];
                tareas[i] = tareas[j];
                tareas[j] = aux;
            }
    for (i = 0; i < N; i++)
        tareas[i].P = i + 1;

    for (i = 0; i < N; i++)
    {
        w = w_ant = 0;
        for (j = i; j < N; j++)
            w += tareas[j].C;
        while (w != w_ant && w < tareas[i].D)
        {
            w_ant = w;
            I = 0;
            for (j = i + 1; j < N; j++)
                I += redondeo_al_alza(w_ant, tareas[j].T) * tareas[j].C;
            w = tareas[i].C + I;
        }
        tareas[i].R = w;
    }
    for (i = 0; i < N; i++)
        tareas[i].planificable = tareas[i].R <= tareas[i].D ? SI : NO;
}

int main(int argc, char *argv[])
{
    tarea_t *tareas;
    int N;
    int nitems;
    int i;

    nitems = scanf("%d", &N);
    if (nitems != 1)
    {
        fprintf(stderr, "%s: lectura incorrecta del número de tareas\n", argv[0]);
        exit(-1);
    }
    tareas = (tarea_t *)malloc(N * sizeof(tarea_t));
    if (tareas == NULL)
    {
        perror(argv[0]);
        exit(-1);
    }
    for (i = 0; i < N; i++)
    {
        tareas[i].id = i + 1;
        nitems = scanf("%d%d%d", &tareas[i].T, &tareas[i].C, &tareas[i].D);
        if (nitems != 3)
        {
            fprintf(stderr, "%s: lectura incorrecta de la tarea %d\n", argv[0], i + 1);
            exit(-1);
        }
    }
    dms(tareas, N);
    printf("+---------------------------------------------------+\n");
    printf("| Tarea-----T-----C-----D-----P-----R--Planificable |\n");
    printf("+---------------------------------------------------+\n");
    for (i = 0; i < N; i++)
        printf("| %5d %5d %5d %5d %5d %5d %12s |\n",
               tareas[i].id,
               tareas[i].T,
               tareas[i].C,
               tareas[i].D,
               tareas[i].P,
               tareas[i].R,
               tareas[i].planificable ? "SI" : "NO");
    printf("+---------------------------------------------------+\n");
    return 0;
}
