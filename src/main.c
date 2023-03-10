/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/03/09 01:20:30 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
