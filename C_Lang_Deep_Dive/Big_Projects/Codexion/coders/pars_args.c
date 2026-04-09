#include "codexion.h"

static t_args	empty_args(void)
{
	t_args	args;

	memset(&args, 0, sizeof(t_args));
	args.scheduler = -1;
	return (args);
}

int	is_empty_args(t_args args)
{
	return (args.scheduler == -1);
}

static int	check_args(char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (!is_full_digit(argv[i]))
			return (0);
		i++;
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (0);
	return (1);
}

static int	validate_values(t_args *args)
{
	if (args->num_coders <= 0 || args->ttb <= 0
		|| args->ttc <= 0 || args->ttd <= 0
		|| args->ttr <= 0 || args->compiles_req <= 0
		|| args->cooldown < 0)
		return (0);
	return (1);
}

static t_args	args_converted(char **argv)
{
	t_args	args;

	args = empty_args();
	if (!check_args(argv))
		return (args);
	args.num_coders = (int)ft_atoi(argv[1]);
	args.ttb = ft_atoi(argv[2]);
	args.ttc = ft_atoi(argv[3]);
	args.ttd = ft_atoi(argv[4]);
	args.ttr = ft_atoi(argv[5]);
	args.compiles_req = (int)ft_atoi(argv[6]);
	args.cooldown = ft_atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		args.scheduler = CDX_FIFO;
	else
		args.scheduler = CDX_EDF;
	if (!validate_values(&args))
		return (empty_args());
	return (args);
}

t_args	convert_args(int argc, char **argv)
{
	if (argc != 9)
		return (empty_args());
	return (args_converted(argv));
}
