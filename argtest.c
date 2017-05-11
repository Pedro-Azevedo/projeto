#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "argtest.h"

void arguments_test(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Erro! Número de argumentos inválido.\n");
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1],"-g")!=0 && strcmp(argv[1],"-t")!=0)
    {
        printf("Erro! Modo não existente.\n");
        exit(EXIT_FAILURE);
    }

    if((strcmp(argv[2],"hubway_stations.csv")!=0 || strcmp(argv[3],"hubway_trips_v2.csv")!=0 ) && (strcmp(argv[2],"hubway_trips_v2.csv")!=0 || strcmp(argv[3],"hubway_stations.csv")!=0))
    {
        printf("Erro! Ficheiros introduzidos incompatíveis.\n");
        exit(EXIT_FAILURE);
    }

    for(int i=2; i<4; i++)
    {
        if(access(argv[i],F_OK)==-1)
        {
            printf("Erro! Ficheiros introduzidos não existem ou não estão localizados na pasta.\n");
            exit(EXIT_FAILURE);
        }
    }
    return;
}
