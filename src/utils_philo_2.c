/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:37:52 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/16 12:57:55 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	philo_log(t_data *data, t_philo *philo, char *s)
{	
	pthread_mutex_lock(&data->printf_lock);
	if (s[0] == 'd')
		printf("%lu %d %s\n", get_msec(philo), philo->thread_id + 1, s);
	if (check_death(data, philo) == 1)
	{
		pthread_mutex_unlock(&data->printf_lock);
		return (1);
	}
	if (s[0] == '/')
	{
		pthread_mutex_unlock(&data->printf_lock);
		return (check_death(data, philo) == 1);
	}
	if (data->end == 0)
		printf("%lu %d %s\n", get_msec(philo), philo->thread_id + 1, s);
	pthread_mutex_unlock(&data->printf_lock);
	return (0);
}

/*death now based on philosopher birth(philo->start_sec), not sim start*/
void	get_start_time(t_philo *philo)
{
	struct timeval	tv;
	struct timezone	tz;

	gettimeofday(&tv, &tz);
	philo->start_sec = tv.tv_sec;
}

unsigned long	get_msec(t_philo *philo)
{
	struct timeval	tv;
	struct timezone	tz;
	unsigned long	milisec;
	unsigned long	seconds;

	gettimeofday(&tv, &tz);
	seconds = tv.tv_sec - philo->start_sec;
	milisec = (tv.tv_usec / M_SEC) + (seconds * M_SEC);
	return (milisec);
}
