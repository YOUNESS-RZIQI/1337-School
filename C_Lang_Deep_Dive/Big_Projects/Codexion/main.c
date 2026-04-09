#include <stdio.h>
#include <stdlib.h>
#include "codexion.h"

int main(int argc, char **argv)
{
    t_args data = convert_args(argc, argv);
    if (is_empty_args(data))
    {
        printf("Error\n");
        return 1;
    }
    printf("OK\n");
    return (0);
}