/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:37:52 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/16 17:19:15 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	philo_log(t_data *data, t_philo *philo, char *s)
{	
	pthread_mutex_lock(&data->printf_lock);
	if (s[0] == 'd')
		printf("%lu %d \033[0;31mdied\033[0m\n", get_sim_msec(data, 0), \
		philo->thread_id + 1);
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
		printf("%lu %d %s\n", get_sim_msec(data, 0), \
		philo->thread_id + 1, s);
	pthread_mutex_unlock(&data->printf_lock);
	return (0);
}

/*death based on sim start, not philo birth*/
void	set_death_hour(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->hour_lock);
	data->death_hour[philo->thread_id] = get_sim_msec(data, 0) \
	+ philo->ms_to_die;
	pthread_mutex_unlock(&data->hour_lock);
}

unsigned long	get_sim_msec(t_data *data, int start)
{
	struct timeval	tv;
	struct timezone	tz;
	unsigned long	milisec;
	unsigned long	seconds;
	int				ret;

	pthread_mutex_lock(&data->time_lock);
	if (data->start_sec == (unsigned long) -1)
	{
		start = 1;
		data->start_sec = tv.tv_sec;
	}
	gettimeofday(&tv, &tz);
	seconds = tv.tv_sec - data->start_sec;
	milisec = (tv.tv_usec / M_SEC) + (seconds * M_SEC);
	if (start == 1)
		data->start_msec = milisec;
	ret = milisec - data->start_msec;
	pthread_mutex_unlock(&data->time_lock);
	if (ret == 0)
			ret += 1;
	return (ret);
}

unsigned long	get_philo_msec(t_philo *philo)
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
