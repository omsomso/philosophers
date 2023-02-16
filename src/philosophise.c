/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:38:31 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/16 14:13:38 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	eat_n_sleep(t_data *data, t_philo *philo)
{
	set_table_status(data, philo);
	pthread_mutex_unlock(&data->table_lock);
	if (philo_log(data, philo, "has taken a fork") == 1)
		return (1);
	if (philo_log(data, philo, "is eating") == 1)
		return (1);
	usleep(data->time_eat * M_SEC);
	pthread_mutex_lock(&data->table_lock);
	set_table_status(data, philo);
	pthread_mutex_unlock(&data->table_lock);
	set_death_hour(data, philo);
	if (check_meals_had(data, philo->thread_id) == 2)
		return (2);
	if (philo_log(data, philo, "is sleeping") == 1)
		return (1);
	usleep(data->time_sleep * M_SEC);
	if (philo_log(data, philo, "is thinking") == 1)
		return (1);
	return (0);
}

int	philosophise(t_data *data, t_philo *philo)
{
	if (philo_log(data, philo, "//just checkin death...") == 1)
		return (1);
	pthread_mutex_lock(&data->table_lock);
	if (check_table_status(data, philo) == 0 && data->end == 0)
		return (eat_n_sleep(data, philo));
	else
	{
		pthread_mutex_unlock(&data->table_lock);
	}
	return (0);
}

void	*handle_end(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&data->end_lock);
	if (philo->end_status == 1 && data->real_end == 0)
	{
		data->real_end = 1;
		philo_log(data, philo, "died");
	}
	if (philo->end_status == 2 && data->end == 1 && data->real_end == 0)
		data->real_end = 1;
	pthread_mutex_unlock(&data->end_lock);
	free(philo);
	return (NULL);
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
	philo->nb_phil = data->nb_phil;
	philo->ms_to_die = data->time_death;
	philo->thread_id = i;
	i++;
	pthread_mutex_unlock(&data->init_lock);
	find_lock_values(philo);
	get_start_time(philo);
	set_death_hour(data, philo);
	while (philo->end_status == 0 && data->end == 0)
	{
		philo->end_status = philosophise(data, philo);
	}
	return (handle_end(data, philo));
}
