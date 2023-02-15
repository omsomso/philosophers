/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 23:35:35 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/15 23:47:48 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>

# define U_SEC 1000000UL
# define M_SEC 1000UL
# define INT_MAX 2147483647
# define INT_MIN -2147483648

typedef struct	s_data
{
	pthread_mutex_t	lock;
	pthread_mutex_t	end_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	init_lock;
	unsigned long	start_sec;
	pthread_t		*thread;
	int				nb_phil;
	int				*table_status;
	int				*meals_had;
	unsigned long	*death_hour;
	int				time_sleep;
	int				time_eat;
	int				time_death;
	int				max_meals;
	int				end;
	int				real_end;
}	t_data;

int	p_isdigit(int c);
long	ft_atol(const char *s);
void	set_table_status(t_data *data, int id, int hi, int lo);
int		check_table_status(t_data *data, int id, int hi, int lo);
void	find_lock_values(t_data *data, int thread_id, int *hi, int *lo);
int	check_meals_had(t_data *data, int thread_id);
void	get_start_time(t_data *data);
unsigned long	get_msec(t_data *data);
int	check_death(t_data *data, int thread_id);
int	eat_n_sleep(t_data *data, int thread_id, int hi, int lo);
int	philosophise(t_data *data, int thread_id, int hi, int lo);
void	*handle_end(t_data *data, int thread_id, int end_status);
void	*be_born(void *tmp);
void	init_mutex(t_data *data);
void	init_data(t_data *data, char **s, int argnb);
void	destroy_everything(t_data *data);
int	check_input(char **s, int argnb);

void	arr_print(int *arr, int n);

#endif