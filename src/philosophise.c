/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:38:31 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/16 22:52:05 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	take_fork_odd(t_data *data, t_philo *philo)
{
	if (philo->thread_id % 2 != 0)
	{
		pthread_mutex_lock(&data->forks[philo->lo]);
		if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m") == 1)
		{
			pthread_mutex_unlock(&data->forks[philo->lo]);
			return (1);
		}
		pthread_mutex_lock(&data->forks[philo->thread_id]);
		if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m") == 1)
		{
			pthread_mutex_unlock(&data->forks[philo->thread_id]);
			pthread_mutex_unlock(&data->forks[philo->lo]);
			return (1);
		}
	}
	return (0);
}

int	take_fork_even(t_data *data, t_philo *philo)
{
	if (philo->thread_id % 2 == 0)
	{
		pthread_mutex_lock(&data->forks[philo->thread_id]);
		if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m") == 1)
		{
			pthread_mutex_unlock(&data->forks[philo->thread_id]);
			return (1);
		}
		pthread_mutex_lock(&data->forks[philo->lo]);
		if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m") == 1)
		{
			pthread_mutex_unlock(&data->forks[philo->thread_id]);
			pthread_mutex_unlock(&data->forks[philo->lo]);
			return (1);
		}
	}
	return (0);
}

int	eat_n_sleep(t_data *data, t_philo *philo)
{
	usleep(data->time_eat * M_SEC);
	set_death_hour(data, philo);
	if (check_meals_had(data, philo->thread_id) == 2)
	{
		pthread_mutex_unlock(&data->forks[philo->thread_id]);
		pthread_mutex_unlock(&data->forks[philo->lo]);
		return (2);
	}
	if (philo_log(data, philo, "\033[0;34mis sleeping\033[0m") == 1)
	{
		pthread_mutex_unlock(&data->forks[philo->thread_id]);
		pthread_mutex_unlock(&data->forks[philo->lo]);
		return (1);
	}
	pthread_mutex_unlock(&data->forks[philo->thread_id]);
	pthread_mutex_unlock(&data->forks[philo->lo]);
	usleep(data->time_sleep * M_SEC);
	if (philo_log(data, philo, "is thinking") == 1)
		return (1);
	return (0);
}

int	philosophise(t_data *data, t_philo *philo)
{
	if (philo_log(data, philo, "//just checkin death...") == 1)
		return (1);
	if (take_fork_odd(data, philo) == 1)
		return (1);
	if (take_fork_even(data, philo) == 1)
		return (1);
	if (philo_log(data, philo, "\033[0;32mis eating\033[0m") == 1)
	{
		pthread_mutex_unlock(&data->forks[philo->thread_id]);
		pthread_mutex_unlock(&data->forks[philo->lo]);
		return (1);
	}
	return (eat_n_sleep(data, philo));
}

void	*be_born(void *tmp)
{
	static int	i = 0;
	t_philo		*philo;
	t_data		*data;

	data = (t_data *)tmp;
	philo = malloc(sizeof(t_philo));
	philo->end_status = 0;
	pthread_mutex_lock(&data->init_lock);
	get_sim_msec(data, 0);
	philo->nb_phil = data->nb_phil;
	philo->ms_to_die = data->time_death;
	philo->thread_id = i;
	philo->nb_forks = data->nb_forks;
	i++;
	pthread_mutex_unlock(&data->init_lock);
	find_lock_values(philo);
	while (philo->end_status == 0 && data->end == 0)
		philo->end_status = philosophise(data, philo);
	return (handle_end(data, philo));
}
