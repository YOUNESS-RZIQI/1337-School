#include "codexion.h"

int	is_full_digit(char *s)
{
	int	i;

	i = 0;
	if (!s || !s[0])
		return (0);
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	ft_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\f' || c == '\v');
}

static int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static long long	conversion(const char *s, int sign)
{
	long long	result;
	long long	prev;

	result = 0;
	while (ft_isdigit((unsigned char)*s))
	{
		prev = result;
		result = result * 10 + (*s - '0');
		if (prev != result / 10)
			return (0);
		s++;
	}
	return (result * sign);
}

long long	ft_atoi(const char *s)
{
	int	sign;

	if (!s)
		return (0);
	sign = 1;
	while (ft_is_space((unsigned char)*s))
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	return (conversion(s, sign));
}
