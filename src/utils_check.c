/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:37:10 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/15 23:48:02 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	get_start_time(t_data *data)
{
	struct timeval	tv;
	struct timezone	tz;

	gettimeofday(&tv, &tz);
	data->start_sec = tv.tv_sec;
}

unsigned long	get_msec(t_data *data)
{
	struct timeval	tv;
	struct timezone	tz;
	unsigned long	milisec;
	unsigned long	seconds;

	pthread_mutex_lock(&data->time_lock);
	gettimeofday(&tv, &tz);
	seconds = tv.tv_sec - data->start_sec;
	milisec = (tv.tv_usec / M_SEC) + (seconds * M_SEC);
	pthread_mutex_unlock(&data->time_lock);
	return (milisec);
}

int	p_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long	ft_atol(const char *s)
{
	long	res;
	long	sign;
	int		i;

	res = 0;
	sign = 1;
	i = 0;
	if (s[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (s[i] && p_isdigit(s[i]) == 1)
	{
		res *= 10;
		res += (s[i] - '0');
		i++;
	}
	return (res * sign);
}

void	arr_print(int *arr, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		printf("%d ", arr[i]);
		i++;
	}
	printf("\n");
}
