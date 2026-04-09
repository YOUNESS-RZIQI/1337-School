#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stddef.h>
# include <string.h>

# define CDX_FIFO 0
# define CDX_EDF  1
# define MAX_CODERS 250

typedef struct s_args
{
	int			num_coders;
	long long	ttb;
	long long	ttc;
	long long	ttd;
	long long	ttr;
	int			compiles_req;
	long long	cooldown;
	int			scheduler;
}	t_args;

typedef struct s_request
{
	int			coder_id;
	long long	sort_key;
}	t_request;

typedef struct s_pqueue
{
	t_request	data[MAX_CODERS];
	int			size;
}	t_pqueue;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				is_held;
	long long		available_at;
	t_pqueue		queue;
}	t_dongle;

typedef struct s_shared	t_shared;

typedef struct s_coder
{
	int			number;
	int			compile_count;
	long long	last_compile_start;
	long long	deadline;
	int			left_id;
	int			right_id;
	int			first_id;
	int			second_id;
	pthread_t	thread;
	t_shared	*shared;
}	t_coder;

struct s_shared
{
	t_args			args;
	t_dongle		*dongles;
	t_coder			*coders;
	int				running;
	long long		start_time;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	state_mutex;
	int				finished_count;
	pthread_t		monitor_thread;
};

t_args		convert_args(int argc, char **argv);
int			is_empty_args(t_args args);
int			is_full_digit(char *s);
long long	ft_atoi(const char *s);
long long	get_time_ms(void);
long long	get_timestamp(t_shared *shared);
void		log_message(t_shared *shared, int num, const char *msg);
void		precise_sleep(long long ms, t_shared *shared);
void		ft_putnbr_buf(long long n, char *buf, int *pos);
int			init_simulation(t_shared *shared, t_args *args);
void		cleanup_simulation(t_shared *shared);
void		pq_init(t_pqueue *pq);
void		pq_push(t_pqueue *pq, t_request req);
t_request	pq_pop(t_pqueue *pq);
int			pq_is_empty(t_pqueue *pq);
int			get_left_dongle_id(t_coder *coder);
int			get_right_dongle_id(t_coder *coder, int total);
void		acquire_dongle(t_dongle *d, t_coder *c, t_shared *s);
void		release_dongle(t_dongle *d, t_shared *s);
void		*run_simulation(void *arg);
void		*monitor_routine(void *arg);
int			is_sim_running(t_shared *shared);
void		do_compile(t_coder *c, t_shared *s);
void		do_debug(t_coder *c, t_shared *s);
void		do_refactor(t_coder *c, t_shared *s);

#endif
