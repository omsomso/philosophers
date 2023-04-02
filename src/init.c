/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:39:19 by kpawlows          #+#    #+#             */
/*   Updated: 2023/04/02 17:53:26 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	write_error(char **s, int argnb)
{
	char	err;

	err = check_input(s, argnb, -1, -1);
	if (err == 'D')
		ft_putendl_fd("Error : enter only positive digits", 2);
	if (err == 'V')
		ft_putendl_fd("Error : enter numbers from 1 to 2147483647", 2);
	if (err == 'N')
		ft_putendl_fd("Error : enter between 4 and 5 numbers", 2);
	if (err == 'F')
		ft_putendl_fd("Error : brainpower overload, 200 thinkers max", 2);
	if (err != 0)
		ft_putendl_fd("[phil_nb] [death_ms] [eat_ms] [sleep_ms] *[meals]", 1);
	if (err == 0)
		return (0);
	return (1);
}

char	check_input(char **s, int argnb, int i, int j)
{
	long	val;

	if (argnb < 4 || argnb > 5)
		return ('N');
	if (ft_atol(s[0]) > 200)
		return ('F');
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
	return (0);
}

int	init_mutex(t_data *data)
{
	int	err;
	int	i;

	err = 0;
	i = -1;
	while (++i < data->nb_phil)
		err += (pthread_mutex_init(&data->forks[i], NULL) == 1);
	err += pthread_mutex_init(&data->meal_lock, NULL);
	err += pthread_mutex_init(&data->check_dth_lock, NULL);
	err += pthread_mutex_init(&data->end_lock, NULL);
	err += pthread_mutex_init(&data->print_lock, NULL);
	if (err > 0)
		return (1);
	return (0);
}

int	init_data(t_data *data, char **s, int argnb)
{
	int	i;

	data->nb_phil = (int)ft_atol(s[0]);
	data->time_death = (int)ft_atol(s[1]);
	data->time_eat = (int)ft_atol(s[2]);
	data->time_sleep = (int)ft_atol(s[3]);
	data->max_meals = -1;
	i = -1;
	if (argnb == 5)
	{
		data->max_meals = (int)ft_atol(s[4]);
		while (++i <= data->nb_phil)
			data->meals_had[i] = 0;
	}
	data->end = 0;
	return (init_mutex(data));
}
