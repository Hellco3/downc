#include <stdio.h>

#include "ClientTool.h"

int main(int argc, char *argv[])
{
    printf("-------------------------------------\n");
    Client_Tool ptool(argc, argv);
    ptool.start();
    return 0;
}