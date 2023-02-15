/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/15 16:30:46 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define	U_SEC 1000000
#define	M_SEC 1000
#define	INT_MAX 2147483647
#define	INT_MIN -2147483648

typedef struct	s_data
{
	pthread_mutex_t	lock;
	unsigned long	start_time;
	int				nb_phil;
	int				table_status[100]; //n
	int				meals_had[100]; //n
	long			death_hour[100]; //n
	int				time_sleep;
	int				time_eat;
	int				time_death;
	int				max_meals;
	int				end;
} t_data;

int	p_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long	ft_atol(const char *s)
{
	long	res;
	long	sign;
	int		i;

	res = 0;
	sign = 1;
	i = 0;
	if (s[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (s[i] && p_isdigit(s[i]) == 1)
	{
		res *= 10;
		res += (s[i] - '0');
		i++;
	}
	return (res * sign);
}

void	arr_print(int *arr, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		printf("%d ", arr[i]);
		i++;
	}
	printf("\n");
}

void	set_table_status(t_data *data, int id, int hi, int lo, char status)
{
		data->table_status[id] = status;
		data->table_status[hi] = status;
		data->table_status[lo] = status;
}

int		check_table_status(t_data *data, int id, int hi, int lo)
{
	if (data->table_status[id] == 0 && data->table_status[hi] == 0 
		&& data->table_status[lo] == 0)
		return (0);
	return (1);
}

void	find_lock_values(t_data *data, int thread_id, int *hi, int *lo)
{
	*hi = thread_id + 1;
	if (*hi >= data->nb_phil)
		*hi = 0;
	*lo = thread_id - 1;
	if (*lo < 0)
		*lo = data->nb_phil -1;
}

int	check_meals_had(t_data *data, int thread_id)
{
	int	i;

	i = -1;
	if (data->max_meals == -1)
		return (0);
	data->meals_had[thread_id]++;
	//printf("phil %d, meals %d\n", thread_id, data->meals_had[thread_id]);
	while (++i < data->nb_phil)
	{
		if (data->meals_had[i] < data->max_meals)
			return (0);
	}
	data->end = 1;
	return (2);
}

unsigned long	get_sec()
{
	struct	timeval tv;
	struct	timezone tz;

	gettimeofday(&tv, &tz);
	return (tv.tv_usec / M_SEC);
}

int	philosophise(t_data *data, int thread_id, int hi, int lo)
{
	find_lock_values(data, thread_id, &hi, &lo);
	pthread_mutex_lock(&data->lock);
	if (data->death_hour[thread_id] <= get_sec() - data->start_time)
		return (1);
	if (check_table_status(data, thread_id, hi, lo) == 0)
	{
		set_table_status(data, thread_id, hi, lo, 1);
		printf("%lu %d has taken a fork\n", get_sec() - data->start_time, thread_id);
		pthread_mutex_unlock(&data->lock); //let other philosophers check if forks are available
		printf("%lu %d is eating\n", get_sec() - data->start_time, thread_id);
		if (check_meals_had(data, thread_id) == 2)
			return (2);
		usleep(data->time_eat * M_SEC); //philosopher eats
		data->death_hour[thread_id] = get_sec() + data->time_death; //death hour reset
		set_table_status(data, thread_id, hi, lo, 0); //put the forks back on the table
		printf("%lu %d is sleeping\n", get_sec() - data->start_time, thread_id);
		usleep(data->time_sleep * M_SEC); //start sleeping
		printf("%lu %d is thinking\n", get_sec() - data->start_time, thread_id); //thinking is waiting to eat
	}
	else
	{
		//usleep(10); //for some reason it makes threads check faster
		pthread_mutex_unlock(&data->lock);
	}
	return (0);
}

void	*be_born(void *tmp)
{
	static int	i = 0;
	int			thread_id;
	t_data		*data;
	int		end_status;

	data =(t_data*)tmp;
	thread_id = i;
	i++;
	end_status = 0;
	data->death_hour[thread_id] = get_sec() + data->time_death;
	data->start_time = get_sec();
	while (end_status == 0 || data->end == 0)
		end_status = philosophise(data, thread_id, 0, 0);
	if (end_status == 1)
		printf("%lu %d died\n", get_sec() - data->start_time, thread_id);
	if (end_status == 2)
	{
		printf("all philosophers ate >= %d meals, simulation ends\n", data->max_meals);
		//how to quit?????
	}
	exit (0);
	return (NULL);
}

void	init_data(t_data *data, char **s, int argnb)
{
	//number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
	/*data->nb_phil = 3;
	data->time_death = 20;
	data->time_eat = 2;
	data->time_sleep = 4;
	data->max_meals = 1;*/

	data->nb_phil = (int)ft_atol(s[0]);
	data->time_death = (int)ft_atol(s[1]);
	data->time_eat = (int)ft_atol(s[2]);
	data->time_sleep = (int)ft_atol(s[3]);
	data->max_meals = -1;
	if (argnb == 5)
		data->max_meals = (int)ft_atol(s[4]);
	data->end = 0;

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

int	main(int argc, char **argv)
{
	int			i = -1;
	t_data		data;

	if (check_input(++argv, --argc) != 0)
		return (1);
	init_data(&data, argv, argc);
	
	pthread_t	thread[data.nb_phil];
	
	pthread_mutex_init(&data.lock, NULL);
	memset(data.table_status, 0, data.nb_phil * sizeof(int));
	memset(data.death_hour, 0, data.nb_phil * sizeof(int));
	memset(data.meals_had, 0, data.nb_phil * sizeof(int));
	while (++i < data.nb_phil)
		data.table_status[i] = 0;
	i = -1;
	while (++i < data.nb_phil)
		pthread_create(&thread[i], NULL, be_born, &data);
	sleep(360);
	//arr_print(data.table_status, 10);
	//printf("program ends now\n");
	i = -1;
	//while (++i < data.nb_phil)
	//	pthread_join(thread[i], NULL);
	//pthread_join(thread[1], NULL);
	pthread_mutex_destroy(&data.lock);
	return (0);
}