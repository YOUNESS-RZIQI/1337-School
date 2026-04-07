#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * do_refactor - Execute refactor phase
 * Why: After debugging, coder refactors
 * Problem solved: Logs action and waits for duration
 */
void do_refactor(t_coder *coder, t_shared *shared, long long now)
{
    long long end;

    coder->state = STATE_REFACTOR;
    log_message(shared, coder->number, "is refactoring");
    end = now + shared->args.time_to_refactor;
    while (get_timestamp() < end)
        usleep(1000);
}