#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * is_dongle_available - Check if dongle is available including cooldown
 * Why: Dongles have cooldown period after release
 * Problem solved: Returns 1 if available, 0 otherwise
 */
int is_dongle_available(t_dongle *dongle, long long now)
{
    if (dongle->is_available)
        return (1);
    if (now >= dongle->release_time)
    {
        dongle->is_available = 1;
        return (1);
    }
    return (0);
}

/*
 * try_acquire_dongle - Thread-safe attempt to acquire dongle
 * Why: Multiple coders might try to take the same dongle
 * Problem solved: Uses mutex to safely check and mark dongle
 */
int try_acquire_dongle(t_dongle *dongle, long long now)
{
    int available;

    pthread_mutex_lock(&dongle->mutex);
    available = is_dongle_available(dongle, now);
    if (available)
        dongle->is_available = 0;
    pthread_mutex_unlock(&dongle->mutex);
    return (available);
}

/*
 * release_dongle - Release dongle with cooldown
 * Why: After use, dongle must respect cooldown period
 * Problem solved: Marks unavailable and sets release time
 */
void release_dongle(t_dongle *dongle, long long now, long long cooldown)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->release_time = now + cooldown;
    dongle->is_available = 0;
    pthread_mutex_unlock(&dongle->mutex);
}

/*
 * get_left_dongle_id - Get left dongle ID for coder
 * Why: Each coder has left and right dongles (circular)
 * Problem solved: Returns correct dongle index
 */
int get_left_dongle_id(t_coder *coder)
{
    return ((int)coder->number - 1);
}

/*
 * get_right_dongle_id - Get right dongle ID for coder
 * Why: Each coder has left and right dongles (circular)
 * Problem solved: Returns correct dongle index
 */
int get_right_dongle_id(t_coder *coder, long long total)
{
    if (coder->number == total)
        return ((int)coder->number - 1);
    return ((int)coder->number);
}