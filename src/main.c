/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/15 23:48:05 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	main(int argc, char **argv)
{
	int			i;
	t_data		data;

	i = -1;
	if (check_input(++argv, --argc) != 0)
		return (1);
	init_data(&data, argv, argc);
	i = -1;
	while (++i < data.nb_phil)
		pthread_create(&data.thread[i], NULL, be_born, &data);
	i = -1;
	while (++i < data.nb_phil)
		pthread_join(data.thread[i], NULL);
	i = -1;
	while (++i < data.nb_phil)
		pthread_detach(data.thread[i]);
	destroy_everything(&data);
	return (0);
}
