#include "codexion.h"
#include <unistd.h>

void	do_compile(t_coder *c, t_shared *s)
{
	c->last_compile_start = get_time_ms();
	c->deadline = c->last_compile_start + s->args.ttb;
	log_message(s, c->number, "is compiling");
	precise_sleep(s->args.ttc, s);
	c->compile_count++;
}

void	do_debug(t_coder *c, t_shared *s)
{
	log_message(s, c->number, "is debugging");
	precise_sleep(s->args.ttd, s);
}

void	do_refactor(t_coder *c, t_shared *s)
{
	log_message(s, c->number, "is refactoring");
	precise_sleep(s->args.ttr, s);
}
