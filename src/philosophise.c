/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:38:31 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/15 23:47:58 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	eat_n_sleep(t_data *data, int thread_id, int hi, int lo)
{
	set_table_status(data, thread_id, hi, lo);
	printf("%lu %d has taken a fork\n", get_msec(data), thread_id + 1);
	pthread_mutex_unlock(&data->lock);
	if (check_death(data, thread_id) == 1)
		return (1);
	printf("%lu %d is eating\n", get_msec(data), thread_id + 1);
	if (check_meals_had(data, thread_id) == 2)
		return (2);
	usleep(data->time_eat * M_SEC);
	data->death_hour[thread_id] = get_msec(data) + data->time_death;
	set_table_status(data, thread_id, hi, lo);
	if (check_death(data, thread_id) == 1)
		return (1);
	printf("%lu %d is sleeping\n", get_msec(data), thread_id + 1);
	usleep(data->time_sleep * M_SEC);
	if (check_death(data, thread_id) == 1)
		return (1);
	printf("%lu %d is thinking\n", get_msec(data), thread_id + 1);
	return (0);
}

int	philosophise(t_data *data, int thread_id, int hi, int lo)
{
	find_lock_values(data, thread_id, &hi, &lo);
	if (check_death(data, thread_id) == 1)
		return (1);
	pthread_mutex_lock(&data->lock);
	if (check_table_status(data, thread_id, hi, lo) == 0 && data->end == 0)
		return (eat_n_sleep(data, thread_id, hi, lo));
	else
		pthread_mutex_unlock(&data->lock);
	return (0);
}

void	*handle_end(t_data *data, int thread_id, int end_status)
{
	pthread_mutex_lock(&data->end_lock);
	if (end_status == 1 && data->end == 1 && data->real_end == 0)
	{
		data->real_end = 1;
		usleep(500);
		printf("%lu %d died\n", get_msec(data), thread_id);
	}
	if (end_status == 2 && data->end == 1 && data->real_end == 0)
	{
		data->real_end = 1;
		usleep(500);
		printf("all philosophers ate >= %d meals\n", data->max_meals);
	}
	pthread_mutex_unlock(&data->end_lock);
	return (NULL);
}

void	*be_born(void *tmp)
{
	static int	i = 0;
	int			thread_id;
	t_data		*data;
	int			end_status;

	data = (t_data *)tmp;
	end_status = 0;
	pthread_mutex_lock(&data->init_lock);
	thread_id = i;
	i++;
	pthread_mutex_unlock(&data->init_lock);
	get_start_time(data);
	data->death_hour[thread_id] = get_msec(data) + data->time_death;
	while (end_status == 0 && data->end == 0)
		end_status = philosophise(data, thread_id, 0, 0);
	return (handle_end(data, thread_id, end_status));
}
