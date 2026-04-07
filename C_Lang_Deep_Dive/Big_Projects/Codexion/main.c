#include <stdio.h>
#include <stdlib.h>
#include "codexion.h"

int main(int argc, char **argv)
{
    t_args      data;
    t_shared    shared;
    long long   coders;
    long long   i;

    data = convert_args(argc, argv);
    if (is_empty_args(data))
    {
        printf("error\n");
        return (1);
    }
    coders = data.number_of_coders;
    shared.coders = NULL;
    shared.dongles = NULL;
    if (!init_simulation(&shared, &data))
    {
        printf("error\n");
        cleanup_simulation(&shared);
        return (1);
    }
    i = 0;
    while (i < coders)
    {
        pthread_create(&shared.coders[i].thread, NULL, run_simulation, &shared.coders[i]);
        i++;
    }
    i = 0;
    while (i < coders)
    {
        pthread_join(shared.coders[i].thread, NULL);
        i++;
    }
    cleanup_simulation(&shared);
    return (0);
}