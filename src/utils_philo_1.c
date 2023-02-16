/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:37:52 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/16 20:36:04 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	find_lock_values(t_philo *philo)
{
	philo->hi = philo->thread_id + 1;
	if (philo->hi >= philo->nb_forks)
		philo->hi = 0;
	philo->lo = philo->thread_id - 1;
	if (philo->lo < 0)
		philo->lo = philo->nb_forks -1;
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

int	check_death(t_data *data, t_philo *philo)
{
	if (data->death_hour[philo->thread_id] <= get_sim_msec(data, 0)
		|| data->end == 1)
	{
		data->end = 1;
		return (1);
	}
	return (0);
}
