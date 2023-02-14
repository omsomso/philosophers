/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpawlows <kpawlows@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 09:42:42 by kpawlows          #+#    #+#             */
/*   Updated: 2023/02/14 10:01:54 by kpawlows         ###   ########.fr       */
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
	pthread_mutex_t	lock[100]; //n + 1 for the general mutex
	int				nb_phil;
	int				table_status[100];
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

int	stepcounter(void)
{
	static int	steps = -1;

	steps++;
	return (steps);
}

void	set_table_status(t_data *data, int p, int lckd_hi, int lckd_lo, int status)
{
		data->table_status[p] = status;
		data->table_status[lckd_hi] = status;
		data->table_status[lckd_lo] = status;
}

int		check_table_status(t_data *data, int p, int lckd_hi, int lckd_lo)
{
	if (data->table_status[p] == 0 && data->table_status[lckd_hi] == 0 && data->table_status[lckd_lo] == 0)
		return (0);
	return (1);
}

void	take_fourchette(t_data *data, int thread_id)
{
	int	i;
	int	lckd_hi;
	int	lckd_lo;

	i = thread_id;

	lckd_hi = i + 1;
	if (lckd_hi >= data->nb_phil)
		lckd_hi = 0;
	lckd_lo = i - 1;
	if (lckd_lo < 0)
		lckd_lo = data->nb_phil -1;

	pthread_mutex_lock(&data->lock[data->nb_phil]);
	printf("thread %d locked locking\n", thread_id);
	arr_print(data->table_status, 10);
	if (check_table_status(data, i, lckd_hi, lckd_lo) == 0)
	{
		set_table_status(data, i, lckd_hi, lckd_lo, 1);

		//lock
		pthread_mutex_lock(&data->lock[i]);
		pthread_mutex_lock(&data->lock[lckd_hi]);
		pthread_mutex_lock(&data->lock[lckd_lo]);

		pthread_mutex_unlock(&data->lock[data->nb_phil]);
		printf("thread %d unlocked locking\n", thread_id);

		printf("thread %d locked mutex idxs %d, %d, %d, sleep...\n", thread_id, i, lckd_hi, lckd_lo);

		sleep(2);

		printf("thread %d unlocks mutex idxs %d, %d, %d\n", thread_id, i, lckd_hi, lckd_lo);
		pthread_mutex_unlock(&data->lock[i]);
		pthread_mutex_unlock(&data->lock[lckd_hi]);
		pthread_mutex_unlock(&data->lock[lckd_lo]);
		
		set_table_status(data, i, lckd_hi, lckd_lo, 0);
	}
	else
	{
		printf("thread %d here, i missed my turn :(\n", thread_id);
		pthread_mutex_unlock(&data->lock[data->nb_phil]);
		return ;
	}

}
int	timer2(int sec)
{
	sleep(sec);
	return (1);
}

void	*thread_test(void *tmp)
{
	static int	i = 0;
	int			thread_id;
	t_data		*data;

	data =(t_data*)tmp;
	thread_id = i;
	i++;
	take_fourchette(data, thread_id);
	printf("and thread %d ends now ~\n", thread_id);
	return (NULL);
}

int	main(void)
{
	int			i = -1;
	int			n = 10;
	pthread_t	thread[n];
	t_data		data;

	data.nb_phil = n;
	while (++i <= n)
		pthread_mutex_init(&data.lock[i], NULL);
	i = -1;
	while (++i < n)
		data.table_status[i] = 0;
	i = -1;
	while (++i < n)
		pthread_create(&thread[i], NULL, thread_test, &data);
	//pthread_create(&thread[0], NULL, thread_test, &data);
	sleep(n * 2);
	printf("program ends now\n");
	//i = -1;
	//while (++i < n)
	//	pthread_join(thread[i], NULL);
	//pthread_join(thread[1], NULL);
	i = -1;
	while (++i <= n)
		pthread_mutex_destroy(&data.lock[i]);
	return (0);
}