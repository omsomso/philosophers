/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/14 12:24:18 by kpawlows         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define U_SEC 1000000

typedef struct	s_data
{
	pthread_mutex_t	lock;
	int				nb_phil;
	int				table_status[100]; //n
	int				time_sleep;
	int				time_eat;
	int				time_death;
} t_data;

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

void	set_table_status(t_data *data, int id, int hi, int lo, int status)
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

void	philosophise(t_data *data, int thread_id)
{
	int	hi;
	int	lo;

	//check death timer here and detach if elapsed
	find_lock_values(data, thread_id, &hi, &lo);
	pthread_mutex_lock(&data->lock);
	if (check_table_status(data, thread_id, hi, lo) == 0)
	{
		set_table_status(data, thread_id, hi, lo, 1);
		pthread_mutex_unlock(&data->lock); //let other threads lock while id sleeps
		printf("philosopher %d eats...\n", thread_id); 
		usleep(data->time_eat * U_SEC); //philosopher eats
		set_table_status(data, thread_id, hi, lo, 0); //put the forks back on the table
		printf("philosopher %d sleeps...\n", thread_id);
		usleep(data->time_sleep * U_SEC); //start sleeping
		printf("philosopher %d thinks...\n", thread_id); //thinking is waiting to eat woah
		//here the death counter should be reset
	}
	else
	{
		pthread_mutex_unlock(&data->lock);
		return ;
	}
}

void	*timer2(void *tmp)
{
	t_data	*data;
	data = (t_data*)tmp;
	sleep(data->time_death); // sleep bc it is in a loop i think idl
	return (NULL);
}

void	*be_born(void *tmp)
{
	static int	i = 0;
	int			thread_id;
	t_data		*data;
	pthread_t	thread_timer;

	data =(t_data*)tmp;
	thread_id = i;
	i++;
	//pthread_create(&thread_timer, NULL, timer2, &data);
	//if (thread_id == 3213)
	while (1)
		philosophise(data, thread_id);
	return (NULL);
}

int	main(void)
{
	int			i = -1;
	int			n = 10;
	pthread_t	thread[n];
	t_data		data;

	data.nb_phil = n;
	data.time_eat = 2;
	data.time_sleep = 5;
	pthread_mutex_init(&data.lock, NULL);
	while (++i < n)
		data.table_status[i] = 0;
	i = -1;
	while (++i < n)
		pthread_create(&thread[i], NULL, be_born, &data);
	//pthread_create(&thread[0], NULL, thread_test, &data);
	sleep(60);
	arr_print(data.table_status, 10);
	printf("program ends now\n");
	//i = -1;
	//while (++i < n)
	//	pthread_join(thread[i], NULL);
	//pthread_join(thread[1], NULL);
	i = -1;
	pthread_mutex_destroy(&data.lock);
	return (0);
}