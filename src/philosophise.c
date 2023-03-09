/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:38:31 by kpawlows          #+#    #+#             */
/*   Updated: 2023/03/09 01:00:53 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	monitor_food(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->meal_lock);
	while (i < data->nb_phil)
	{
		if (check_meals(data) == 1)
		{
			data->end = 1;
			pthread_mutex_unlock(&data->meal_lock);
			pthread_mutex_lock(&data->printf_lock);
			printf("\033[0;31mall philosophers ate to eternity :)\033[0m\n");
			pthread_mutex_unlock(&data->printf_lock);
			return (1);
		}
		i++;
		pthread_mutex_unlock(&data->meal_lock);
	}
	return (0);
}

void	*monitor_death(void *tmp)
{
	int		dead_one;
	t_data	*data;

	data = (t_data *)tmp;
	while (data->end == 0)
	{
		dead_one = check_death(data);
		if (dead_one > -1)
		{
			data->end = 1;
			pthread_mutex_lock(&data->printf_lock);
			printf("%011lu %d \033[0;31mdied\033[0m\n", get_sim_msec(data, 0), \
			dead_one + 1);
			pthread_mutex_unlock(&data->printf_lock);
			break ;
		}
		if (data->max_meals > -1)
		{
			if (monitor_food(data) == 1)
				return (NULL);
		}
		//usleep(200);
	}
	return (NULL);
}

int	take_fork(t_data *data, t_philo *philo)
{
	if (philo->thread_id % 2 != 0)
	{
		pthread_mutex_lock(&data->forks[philo->lo]);
		if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m") == 1)
			return (1);
		pthread_mutex_lock(&data->forks[philo->thread_id]);
		if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m") == 1)
			return (1);
	}
	else
	{
		pthread_mutex_lock(&data->forks[philo->thread_id]);
		if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m") == 1)
			return (1);
		pthread_mutex_lock(&data->forks[philo->lo]);
		if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m") == 1)
			return (1);
	}
	return (0);
}

int	philosophise(t_data *data, t_philo *philo)
{
	if (take_fork(data, philo) == 1)
		return (1);
	if (philo_log(data, philo, "\033[33mis eating\033[0m") == 1)
		return (1);
	set_death_hour(data, philo);
	usleep(data->time_eat * M_SEC);
	add_meal(data, philo->thread_id);
	pthread_mutex_unlock(&data->forks[philo->thread_id]);
	pthread_mutex_unlock(&data->forks[philo->lo]);
	if (philo_log(data, philo, "\033[0;34mis sleeping\033[0m") == 1)
		return (1);
	usleep(data->time_sleep * M_SEC);
	if (philo_log(data, philo, "is thinking") == 1)
		return (1);
	return (0);
}

void	*be_born(void *tmp)
{
	static int	i = 0;
	t_philo		*philo;
	t_data		*data;

	data = (t_data *)tmp;
	philo = malloc(sizeof(t_philo));
	if (philo == NULL)
		return (NULL);
	philo->end_status = 0;
	pthread_mutex_lock(&data->init_lock);
	get_sim_msec(data, 0);
	philo->nb_phil = data->nb_phil;
	philo->ms_to_die = data->time_death;
	philo->thread_id = i;
	philo->nb_forks = data->nb_forks;
	data->meals_had[i] = 0;
	i++;
	pthread_mutex_unlock(&data->init_lock);
	find_forks(philo);
	while (philo->end_status == 0 && data->end == 0)
		philo->end_status = philosophise(data, philo);
	return (NULL);
}
