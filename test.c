/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/15 22:50:30 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define	U_SEC 1000000UL
#define	M_SEC 1000UL
#define	INT_MAX 2147483647
#define	INT_MIN -2147483648

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
	pthread_mutex_lock(&data->meal_lock);
	data->meals_had[thread_id]++;
	//printf("meals had = \n");
	while (++i < data->nb_phil)
	{
		if (data->meals_had[i] < data->max_meals)
		{
			pthread_mutex_unlock(&data->meal_lock);
			//arr_print(data->meals_had, data->nb_phil);
			return (0);
		}
	}
	//arr_print(data->meals_had, data->nb_phil);
	pthread_mutex_unlock(&data->meal_lock);
	data->end = 1;
	return (2);
}

void	get_start_time(t_data *data)
{
	struct	timeval tv;
	struct	timezone tz;

	gettimeofday(&tv, &tz);
	data->start_sec = tv.tv_sec;
}

unsigned long	get_msec(t_data *data)
{
	struct	timeval tv;
	struct	timezone tz;
	unsigned long	milisec;
	unsigned long	seconds;


	pthread_mutex_lock(&data->time_lock);
	gettimeofday(&tv, &tz);

	seconds = tv.tv_sec - data->start_sec;
	milisec = (tv.tv_usec / M_SEC) + (seconds * M_SEC);
	pthread_mutex_unlock(&data->time_lock);
	return (milisec);
}

int	check_death(t_data *data, int thread_id)
{
	if (data->death_hour[thread_id] <=  get_msec(data) || data->end == 1)
	{
		data->end = 1;
		return (1);
	}
	return (0);
}

int	philosophise(t_data *data, int thread_id, int hi, int lo)
{
	find_lock_values(data, thread_id, &hi, &lo);
		if (check_death(data, thread_id) == 1)
			return (1);
	pthread_mutex_lock(&data->lock);

	if (check_table_status(data, thread_id, hi, lo) == 0 && data->end == 0)
	{
		set_table_status(data, thread_id, hi, lo, 1);
		printf("%lu %d has taken a fork\n",  get_msec(data), thread_id + 1);
		pthread_mutex_unlock(&data->lock);
		if (check_death(data, thread_id) == 1)
			return (1);
		printf("%lu %d is eating\n",  get_msec(data), thread_id + 1);
		if (check_meals_had(data, thread_id) == 2)
			return (2);
		usleep(data->time_eat * M_SEC); //philosopher eats
		data->death_hour[thread_id] =  get_msec(data) + data->time_death; //death hour reset
		set_table_status(data, thread_id, hi, lo, 0); //put the forks back on the table
		if (check_death(data, thread_id) == 1)
			return (1);
		printf("%lu %d is sleeping\n",  get_msec(data), thread_id + 1);
		usleep(data->time_sleep * M_SEC); //start sleeping
		if (check_death(data, thread_id) == 1)
			return (1);
		printf("%lu %d is thinking\n",  get_msec(data), thread_id + 1); //thinking is waiting to eat
	}
	else
		pthread_mutex_unlock(&data->lock);
	return (0);
}

void	*be_born(void *tmp)
{
	static int	i = 0;
	int			thread_id;
	t_data		*data;
	int		end_status;

	data =(t_data*)tmp;
	end_status = 0;
	pthread_mutex_lock(&data->init_lock);
	thread_id = i;
	i++;
	pthread_mutex_unlock(&data->init_lock);
	printf("thread id == %d\n", thread_id);
	get_start_time(data);
	data->death_hour[thread_id] =  get_msec(data) + data->time_death;
	//printf("dh of %d = %lu\n", thread_id, data->death_hour[thread_id]);
	while (end_status == 0 && data->end == 0)
		end_status = philosophise(data, thread_id, 0, 0);
	pthread_mutex_lock(&data->end_lock);
	if (end_status == 1 && data->end == 1 && data->real_end == 0)
	{
		data->real_end = 1;
		usleep(500);
		printf("%lu %d died\n",  get_msec(data), thread_id);
	}
	if (end_status == 2 && data->end == 1 && data->real_end == 0)
	{
		data->real_end = 1;
		usleep(500);
		printf("all philosophers ate >= %d meals, simulation ends\n", data->max_meals);
	}
	pthread_mutex_unlock(&data->end_lock);
	//exit(0);
	return (NULL);
}

void	init_mutex(t_data *data)
{
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->end_lock, NULL);
	pthread_mutex_init(&data->time_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->init_lock, NULL);
}

void	init_data(t_data *data, char **s, int argnb)
{

	//number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
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
	//pthread_t	thread[data->nb_phil];
}

void	destroy_everything(t_data *data)
{
	pthread_mutex_destroy(&data->lock);
	pthread_mutex_destroy(&data->end_lock);
	pthread_mutex_destroy(&data->time_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->init_lock);
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


int	main(int argc, char **argv)
{
	int			i = -1;
	t_data		data;

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