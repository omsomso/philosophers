/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/03/21 01:39:02 by kpawlows         ###   ########.fr       */
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
	pthread_mutex_destroy(&data->end_lock);
	free(data->forks);
	free(data->death_hour);
	free(data->meals_had);
	free(data->thread);
	free(data);
}

int	main(int argc, char **argv)
{
	int			i;
	t_data		*data;

	data = malloc(sizeof(t_data));
	i = -1;
	if (handle_error(++argv, --argc) != 0 || init_data(data, argv, argc) != 0)
	{
		free(data);
		return (1);
	}
	while (++i < data->nb_phil)
		pthread_create(&data->thread[i], NULL, be_born, data);
	pthread_create(&data->thread[i + 1], NULL, monitor_death, data);
	i = -1;
	while (++i < data->nb_phil + 1)
		pthread_join(data->thread[i], NULL);
	i = -1;
	while (++i < data->nb_phil + 1)
		pthread_detach(data->thread[i]);
	destroy_everything(data, -1);
	return (0);
}
