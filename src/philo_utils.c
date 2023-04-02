/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42lausanne.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:37:52 by kpawlows          #+#    #+#             */
/*   Updated: 2023/04/02 17:50:20 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	find_forks(t_data *data, t_philo *philo)
{
	philo->lo = philo->id - 1;
	if (philo->lo < 0)
		philo->lo = data->nb_phil -1;
}

int	check_meals(t_data *data)
{
	int	i;
	int	ret;

	i = 0;
	ret = 1;
	pthread_mutex_lock(&data->meal_lock);
	while (i < data->nb_phil)
	{
		if (data->meals_had[i] < data->max_meals)
			ret = 0;
		i++;
	}
	pthread_mutex_unlock(&data->meal_lock);
	return (ret);
}

int	add_meal(t_data *data, int thread_id)
{
	if (data->max_meals == -1)
		return (0);
	pthread_mutex_lock(&data->meal_lock);
	data->meals_had[thread_id]++;
	pthread_mutex_unlock(&data->meal_lock);
	return (2);
}

int	check_death(t_data *data)
{
	int				i;

	i = 0;
	pthread_mutex_lock(&data->check_dth_lock);
	while (i < data->nb_phil)
	{
		if (data->philo[i].death_time <= get_sim_time(data))
		{
			pthread_mutex_unlock(&data->check_dth_lock);
			return (i);
		}
		i++;
	}
	pthread_mutex_unlock(&data->check_dth_lock);
	return (-1);
}

int	check_end(t_data *data)
{
	pthread_mutex_lock(&data->end_lock);
	if (data->end == 1)
	{
		pthread_mutex_unlock(&data->end_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->end_lock);
	return (0);
}
