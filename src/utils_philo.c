/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:37:52 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/15 23:53:35 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	set_table_status(t_data *data, int id, int hi, int lo)
{
	int	status;

	status = 1;
	if (data->table_status[id] == 1)
		status = 0;
	data->table_status[id] = status;
	data->table_status[hi] = status;
	data->table_status[lo] = status;
}

int	check_table_status(t_data *data, int id, int hi, int lo)
{
	if (data->table_status[id] == 0 && data->table_status[hi] == 0
		&& data->table_status[lo] == 0)
		return (0);
	return (1);
}

void	find_lock_values(t_data *data, int thread_id, int *hi, int *lo)
{
	*hi = thread_id + 1;
	if (*hi >= data->nb_phil)
		*hi = 0;
	*lo = thread_id - 1;
	if (*lo < 0)
		*lo = data->nb_phil -1;
}

int	check_meals_had(t_data *data, int thread_id)
{
	int	i;

	i = -1;
	if (data->max_meals == -1)
		return (0);
	pthread_mutex_lock(&data->meal_lock);
	data->meals_had[thread_id]++;
	while (++i < data->nb_phil)
	{
		if (data->meals_had[i] < data->max_meals)
		{
			pthread_mutex_unlock(&data->meal_lock);
			return (0);
		}
	}
	pthread_mutex_unlock(&data->meal_lock);
	data->end = 1;
	return (2);
}

int	check_death(t_data *data, int thread_id)
{
	if (data->death_hour[thread_id] <= get_msec(data) || data->end == 1)
	{
		data->end = 1;
		return (1);
	}
	return (0);
}
