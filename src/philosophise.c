/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:38:31 by kpawlows          #+#    #+#             */
/*   Updated: 2023/04/02 18:41:03 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	philo_log(t_data *data, t_philo *philo, char *s)
{	
	if (check_end(data) == 1)
		return (1);
	pthread_mutex_lock(&data->print_lock);
	printf("%011lu ", get_sim_time(data));
	printf("%d ", philo->id + 1);
	printf("%s\n", s);
	pthread_mutex_unlock(&data->print_lock);
	return (0);
}

void	sleep_well(t_data *data, unsigned long time)
{
	unsigned long	st;

	st = get_time();
	while (check_end(data) == 0 && (get_time() - st) <= time)
		usleep(200);
}

void	take_fork(t_data *data, t_philo *philo)
{
	printf("lo = %d id = %d\n", philo->lo, philo->id);
	pthread_mutex_lock(&data->forks[philo->id]);
	if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m"))
		return ;
	pthread_mutex_lock(&data->forks[philo->lo]);
	if (philo_log(data, philo, "\033[0;32mhas taken a fork\033[0m"))
		return ;
}

void	philosophise(t_data *data, t_philo *philo)
{
	take_fork(data, philo);
	set_death_hour(data, philo);
	philo_log(data, philo, "\033[33mis eating\033[0m");
	sleep_well(data, data->time_eat);
	add_meal(data, philo->id);
	pthread_mutex_unlock(&data->forks[philo->id]);
	pthread_mutex_unlock(&data->forks[philo->lo]);
	philo_log(data, philo, "\033[0;34mis sleeping\033[0m");
	sleep_well(data, data->time_sleep);
}

void	*be_born(void *tmp)
{
	t_philo	*philo;
	int		end;

	end = 0;
	philo = (t_philo *)tmp;
	find_forks(philo->data, philo);
	if (philo->id % 2 != 0)
		usleep(10 * M_SEC);
	while (end == 0)
	{
		philosophise(philo->data, philo);
		end = check_end(philo->data);
	}
	return (NULL);
}
