/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:37:52 by kpawlows          #+#    #+#             */
/*   Updated: 2023/03/09 13:32:54 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	find_forks(t_philo *philo)
{
	philo->lo = philo->thread_id - 1;
	if (philo->lo < 0)
		philo->lo = philo->nb_forks -1;
}

int	check_meals(t_data *data)
{
	int	i;
	int	ret;

	i = 0;
	ret = 1;
	while (i < data->nb_phil)
	{
		if (data->meals_had[i] < data->max_meals)
			ret = 0;
		i++;
	}
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
	int	i;

	i = 0;
	pthread_mutex_lock(&data->hour_lock);
	while (i < data->nb_phil)
	{
		if (data->death_hour[i] <= get_sim_msec(data, 0))
		{
			pthread_mutex_unlock(&data->hour_lock);
			return (i);
		}
		i++;
	}
	pthread_mutex_unlock(&data->hour_lock);
	return (-1);
}
