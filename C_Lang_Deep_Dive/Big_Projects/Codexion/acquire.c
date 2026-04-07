#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * acquire_both_dongles - Try to acquire both dongles
 * Why: Compilation requires both dongles
 * Problem solved: Tries both, releases first if second fails
 */
int acquire_both_dongles(t_coder *coder, t_shared *shared, long long now)
{
    int left_id;
    int right_id;
    int left_ok;
    int right_ok;

    left_id = get_left_dongle_id(coder);
    right_id = get_right_dongle_id(coder, shared->args.number_of_coders);
    left_ok = try_acquire_dongle(&shared->dongles[left_id], now);
    if (left_ok)
    {
        right_ok = try_acquire_dongle(&shared->dongles[right_id], now);
        if (right_ok)
        {
            coder->left_dongle = left_id;
            coder->right_dongle = right_id;
            return (1);
        }
        release_dongle(&shared->dongles[left_id], now, shared->args.dongle_cooldown);
    }
    return (0);
}

/*
 * release_both_dongles - Release both dongles
 * Why: After compilation, return dongles to table
 * Problem solved: Properly releases both with cooldown
 */
void release_both_dongles(t_coder *coder, t_shared *shared, long long now)
{
    release_dongle(&shared->dongles[coder->left_dongle], now, shared->args.dongle_cooldown);
    release_dongle(&shared->dongles[coder->right_dongle], now, shared->args.dongle_cooldown);
}

/*
 * check_burnout - Check if coder exceeded deadline
 * Why: Coders burn out if they don't compile in time
 * Problem solved: Returns 1 if burnout, 0 otherwise
 */
int check_burnout(t_coder *coder, long long now)
{
    if (coder->state == STATE_COMPILE)
        return (0);
    if (now >= coder->deadline)
        return (1);
    return (0);
}

/*
 * do_compile - Execute compile phase
 * Why: Compilation takes time_to_compile ms
 * Problem solved: Logs action and waits for duration
 */
void do_compile(t_coder *coder, t_shared *shared, long long now)
{
    long long end;

    coder->state = STATE_COMPILE;
    log_message(shared, coder->number, "is compiling");
    end = now + shared->args.time_to_compile;
    while (get_timestamp() < end)
        usleep(1000);
}

/*
 * do_debug - Execute debug phase
 * Why: After compiling, coder debugs
 * Problem solved: Logs action and waits for duration
 */
void do_debug(t_coder *coder, t_shared *shared, long long now)
{
    long long end;

    coder->state = STATE_DEBUG;
    log_message(shared, coder->number, "is debugging");
    end = now + shared->args.time_to_debug;
    while (get_timestamp() < end)
        usleep(1000);
}