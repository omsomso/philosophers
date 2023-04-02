/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42lausanne.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/04/02 05:50:40 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	destroy_everything(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_phil)
		pthread_join(data->philo[i].thread, NULL);
	i = -1;
	while (++i < data->nb_phil)
		pthread_detach(data->philo[i].thread);
	i = -1;
	while (++i < data->nb_phil)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->print_lock);
	pthread_mutex_destroy(&data->check_dth_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->end_lock);
}

int	monitor_food(t_data *data)
{
	if (data->max_meals == -1)
		return (0);
	if (check_meals(data) == 1)
	{
		pthread_mutex_lock(&data->end_lock);
		data->end = 1;
		pthread_mutex_unlock(&data->end_lock);
		pthread_mutex_lock(&data->print_lock);
		printf("\033[0;31mall philosophers ate enough\033[0m\n");
		pthread_mutex_unlock(&data->print_lock);
		return (1);
	}
	return (0);
}

void	monitor(void *tmp)
{
	int		dead_one;
	t_data	*data;

	data = (t_data *) tmp;
	while (1)
	{
		dead_one = check_death(data);
		if (dead_one > -1)
		{
			pthread_mutex_lock(&data->end_lock);
			data->end = 1;
			pthread_mutex_unlock(&data->end_lock);
			pthread_mutex_lock(&data->print_lock);
			printf("%011lu ", get_sim_time(data));
			printf("%d \033[0;31mdied\033[0m\n", dead_one + 1);
			pthread_mutex_unlock(&data->print_lock);
			return ;
		}
		if (monitor_food(data) == 1)
			return ;
		usleep(100);
	}
	return ;
}

int	start_sim(t_data *data)
{
	int	i;

	i = -1;
	data->start_msec = get_time();
	while (++i < data->nb_phil)
	{
		data->philo[i].id = i;
		data->philo[i].data = data;
		set_death_hour(data, &data->philo[i]);
		pthread_create(&data->philo[i].thread, NULL, be_born, &data->philo[i]);
	}
	monitor(data);
	destroy_everything(data);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;

	if (write_error(++argv, --argc) != 0 || init_data(&data, argv, argc) != 0)
		return (1);
	start_sim(&data);
	return (0);
}
