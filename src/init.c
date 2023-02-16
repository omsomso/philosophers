/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:39:19 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/16 13:53:54 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	destroy_everything(t_data *data)
{
	pthread_mutex_destroy(&data->table_lock);
	pthread_mutex_destroy(&data->end_lock);
	pthread_mutex_destroy(&data->printf_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->init_lock);
	pthread_mutex_destroy(&data->hour_lock);
	free(data->table_status);
	free(data->death_hour);
	free(data->meals_had);
	free(data->thread);
}

int	check_input(char **s, int argnb)
{
	long	val;
	int		i;
	int		j;

	i = -1;
	while (++i < argnb)
	{
		j = -1;
		while (s[i][++j] != 0x00)
		{
			if (p_isdigit(s[i][j] == 0))
				return (1);
		}
		val = ft_atol(s[i]);
		if (val > INT_MAX || val < 1)
			return (1);
	}
	if (argnb < 4 || argnb > 5)
		return (1);
	return (0);
}

void	init_mutex(t_data *data)
{
	pthread_mutex_init(&data->table_lock, NULL);
	pthread_mutex_init(&data->end_lock, NULL);
	pthread_mutex_init(&data->printf_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->init_lock, NULL);
	pthread_mutex_init(&data->hour_lock, NULL);
}

void	init_data(t_data *data, char **s, int argnb)
{
	data->nb_phil = (int)ft_atol(s[0]);
	data->time_death = (int)ft_atol(s[1]);
	data->time_eat = (int)ft_atol(s[2]);
	data->time_sleep = (int)ft_atol(s[3]);
	data->max_meals = -1;
	if (argnb == 5)
		data->max_meals = (int)ft_atol(s[4]);
	data->end = 0;
	data->real_end = 0;
	data->thread = malloc(sizeof(pthread_t) * data->nb_phil);
	data->table_status = malloc(sizeof(int) * data->nb_phil);
	data->meals_had = malloc(sizeof(int) * data->nb_phil);
	data->death_hour = malloc(sizeof(unsigned long) * data->nb_phil);
	memset(data->table_status, 0, data->nb_phil * sizeof(int));
	memset(data->meals_had, 0, data->nb_phil * sizeof(int));
	memset(data->death_hour, 0, data->nb_phil * sizeof(unsigned long));
	init_mutex(data);
}
