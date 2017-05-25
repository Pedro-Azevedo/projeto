/*************************************************************
Projeto Final de Programação 2º Semestre 2016/2017
AUTORES:    Diogo Costa, nº 86971
            Pedro Azevedo, nº 84396
DATA: 25/05/2017
Implementação do programa Bikestation, de tratamento de dados
sobre viagens/estações de bicicleta de Boston
***************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//We include our header files
#include "trips.h"
#include "argtest.h"
#include "graphicmode.h"

int main(int argc, char*argv[])
{
    //Test the arguments entered
    arguments_test(argc, argv);

    //Define the pointers to our lists
    tripnode* triplist=NULL;
    stationnode* stationlist=NULL;

    load_fromfiles(argv, &triplist, &stationlist);

    //Start the text mode
    if(strcmp(argv[1], "-t")==0)
        textmode(argv, &triplist, &stationlist);

    //Start the graphic mode
    if(strcmp(argv[1], "-g")==0)
        graphicmode(argv, &triplist, &stationlist);

    //Free the lists memory
    ClearData(argv, &triplist, &stationlist, 1);

    return EXIT_SUCCESS;
}
