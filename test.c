/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/14 15:54:01 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define U_SEC 1000000

typedef struct	s_data
{
	pthread_mutex_t	lock;
	int				nb_phil;
	int				table_status[100]; //n
	int				meals_had[100]; //n
	long			death_hour[100]; //n
	int				time_sleep;
	int				time_eat;
	int				time_death;
	int				max_meals;
} t_data;

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t'
		|| c == '\v')
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *s)
{
	int	res;
	int	sign;
	int	i;

	res = 0;
	sign = 1;
	i = 0;
	while (s[i] && ft_isspace(s[i]) == 1)
		i++;
	if (s[i] == '+' && ft_isdigit(s[i + 1]) == 0)
		return (0);
	if (s[i] == '+')
		i++;
	if (s[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (s[i] && ft_isdigit(s[i]) == 1)
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

int	philosophise(t_data *data, int thread_id, int hi, int lo)
{
	struct	timeval tv;
	struct	timezone tz;

	gettimeofday(&tv, &tz);
	find_lock_values(data, thread_id, &hi, &lo);
	pthread_mutex_lock(&data->lock);
	if (data->death_hour[thread_id] <= tv.tv_sec)
		return (1);
	if (check_table_status(data, thread_id, hi, lo) == 0)
	{
		set_table_status(data, thread_id, hi, lo, 1);
		pthread_mutex_unlock(&data->lock); //let other philosophers check if forks are available
		printf("philosopher %d eats...\n", thread_id);
		if (++data->meals_had[thread_id] >= data->max_meals)
			return (2);
		usleep(data->time_eat * U_SEC); //philosopher eats
		data->death_hour[thread_id] = tv.tv_sec + data->time_death; //death hour reset
		set_table_status(data, thread_id, hi, lo, 0); //put the forks back on the table
		printf("philosopher %d sleeps...\n", thread_id);
		usleep(data->time_sleep * U_SEC); //start sleeping
		printf("philosopher %d thinks...\n", thread_id); //thinking is waiting to eat
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
	struct	timeval tv;
	struct	timezone tz;
	int		end_status;

	data =(t_data*)tmp;
	thread_id = i;
	i++;
	end_status = 0;
	gettimeofday(&tv, &tz);
	data->death_hour[thread_id] = tv.tv_sec + data->time_death;
	while (end_status == 0)
		end_status = philosophise(data, thread_id, 0, 0);
	if (end_status == 1)
		printf("philosopher %d died to death, simulation ends\n", thread_id);
	if (end_status == 2)
		printf("philosopher %d ate to immortality, simulation ends\n", thread_id);
	exit (0); //not allowed...
	return (NULL);
}

void	init_data(t_data *data, char **s)
{
	//number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
	data->nb_phil = 10;
	data->time_death = 20;
	data->time_eat = 2;
	data->time_sleep = 4;
	data->max_meals = 5; 

	/*data->nb_phil = ft_atoi(s[0]);
	data->time_death = ft_atoi(s[1]);
	data->time_eat = ft_atoi(s[2]);
	data->time_sleep = ft_atoi(s[3]);
	data->max_meals = ft_atoi(s[4]); //optional*/
}

int	check_input(char **s)
{
	int	i;

	i = 0;
	while (s[i] != NULL)
		i++;
	//if (i < 5 || i > 6)
	//	return (1);
	//check if digit, max and min int
	//porb not needed to check if death < sleep or smth
	return (0);
}

int	main(int argc, char **argv)
{
	int			i = -1;
	t_data		data;

	if (check_input(++argv) != 0)
		return (1);
	init_data(&data, argv);
	
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
	printf("program ends now\n");
	//i = -1;
	//while (++i < n)
	//	pthread_join(thread[i], NULL);
	//pthread_join(thread[1], NULL);
	pthread_mutex_destroy(&data.lock);
	return (0);
}