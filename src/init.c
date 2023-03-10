/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:39:19 by kpawlows          #+#    #+#             */
/*   Updated: 2023/03/10 03:44:04 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	destroy_everything(t_data *data, int i)
{
	while (++i < data->nb_forks)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->printf_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->init_lock);
	pthread_mutex_destroy(&data->hour_lock);
	free(data->forks);
	free(data->death_hour);
	free(data->meals_had);
	free(data->thread);
	free(data);
}

int	handle_error(char **s, int argnb)
{
	char	err;

	err = check_input(s, argnb, -1, -1);
	if (err == 'D')
		ft_putendl_fd("Error : enter only positive digits", 2);
	if (err == 'V')
		ft_putendl_fd("Error : enter numbers from 1 to 2147483647", 2);
	if (err == 'T')
	{
		ft_putendl_fd("Error : the philosophers can only die, check values", 2);
		ft_putendl_fd("[phil_nb] [death_ms] [eat_ms] [sleep_ms] *[meals]", 1);
	}
	if (err == 'N')
	{
		ft_putendl_fd("Error : enter between 4 and 5 numbers", 2);
		ft_putendl_fd("[phil_nb] [death_ms] [eat_ms] [sleep_ms] *[meals]", 1);
	}
	if (err == 'O')
		ft_putendl_fd("Error : there must be 2+ philosophers", 2);
	if (err == 0)
		return (0);
	return (1);
}

char	check_input(char **s, int argnb, int i, int j)
{
	long	val;

	while (++i < argnb)
	{
		j = -1;
		if (ft_strlen(s[i]) > 14)
			return ('V');
		val = ft_atol(s[i]);
		while (s[i][++j] != '\0')
		{
			if (ft_isdigit(s[i][j]) == 0)
				return ('D');
		}
		if (val > INT_MAX || val < 1)
			return ('V');
	}
	if (argnb < 4 || argnb > 5)
		return ('N');
	if (ft_atol(s[1]) < (ft_atol(s[2]) + ft_atol(s[3])))
		return ('T');
	if (ft_atol(s[0]) == 1)
		return ('O');
	return (0);
}

int	init_mutex(t_data *data, int i)
{
	int	err;

	err = 0;
	if (data->nb_phil == 1)
		data->nb_forks++;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_forks);
	if (data->forks == NULL)
		return (1);
	while (++i < data->nb_forks)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) == 1)
			return (1);
	}
	err += pthread_mutex_init(&data->printf_lock, NULL);
	err += pthread_mutex_init(&data->meal_lock, NULL);
	err += pthread_mutex_init(&data->init_lock, NULL);
	err += pthread_mutex_init(&data->hour_lock, NULL);
	if (err > 0)
		return (1);
	return (0);
}

int	init_data(t_data *data, char **s, int argnb)
{
	data->nb_phil = (int)ft_atol(s[0]);
	data->nb_forks = data->nb_phil;
	data->time_death = (int)ft_atol(s[1]);
	data->time_eat = (int)ft_atol(s[2]);
	data->time_sleep = (int)ft_atol(s[3]);
	data->max_meals = -1;
	if (argnb == 5)
		data->max_meals = (int)ft_atol(s[4]);
	data->end = 0;
	data->start_sec = -1;
	data->thread = ft_calloc(data->nb_phil + 1, sizeof(pthread_t));
	data->meals_had = ft_calloc(data->nb_phil, sizeof(int));
	data->death_hour = ft_calloc(data->nb_phil, sizeof(unsigned long));
	memset(data->death_hour, 1, data->nb_phil * sizeof(unsigned long));
	if (!(data->thread) || !(data->meals_had) || !(data->death_hour))
		return (1);
	return (init_mutex(data, -1));
}
