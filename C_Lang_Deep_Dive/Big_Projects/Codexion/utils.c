#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

long long get_timestamp(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

void log_message(t_shared *shared, long long coder_num, const char *msg)
{
    long long   ts;
    char        buf[128];

    ts = get_timestamp();
    pthread_mutex_lock(&shared->log_mutex);
    snprintf(buf, sizeof(buf), "%lld %lld %s\n", ts, coder_num, msg);
    write(1, buf, strlen(buf));
    pthread_mutex_unlock(&shared->log_mutex);
}