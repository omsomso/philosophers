#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define U_SEC 1000000

typedef struct	s_data
{
	pthread_mutex_t	lock[4];
	int				nb_phil;
	int				time_sleep;
	int				time_eat;
	int				time_death;
} t_data;

void	take_fourchette(t_data *data, int thread_id)
{
	int	i;
	int	lckd_hi;
	int	lckd_lo;

	i = thread_id;
	
	lckd_hi = i + 1;
	if (lckd_hi == data->nb_phil)
		lckd_hi = 0;
	lckd_lo = i - 1;
	if (lckd_lo < 0)
		lckd_lo = data->nb_phil -1;
	

	printf("thread %d tries to access mutex idx %d, %d, %d\n", i, i, lckd_hi, lckd_lo);

	pthread_mutex_lock(&data->lock[i]);
	pthread_mutex_lock(&data->lock[lckd_hi]);
	pthread_mutex_lock(&data->lock[lckd_lo]);

	printf("thread %d locks mutex idxs %d, %d, %d, sleep...\n", thread_id, i, lckd_hi, lckd_lo);
	sleep(1);
	printf("thread %d unlocks mutex idxs %d, %d, %d, sleep...\n", thread_id, i, lckd_hi, lckd_lo);

	pthread_mutex_unlock(&data->lock[i]);
	pthread_mutex_unlock(&data->lock[lckd_hi]);
	pthread_mutex_unlock(&data->lock[lckd_lo]);
}

void	*thread_test(void *tmp)
{
	static int	i = 0;
	int			thread_id;
	t_data		*data;

	data =(t_data*)tmp;
	thread_id = i;
	i++;
	take_fourchette(tmp, thread_id);
	printf("and thread %d ends now :(\n", thread_id);
	return (NULL);
}

int	main(void)
{
	int			i = -1;
	int			n = 4;
	pthread_t	thread[n];
	t_data		data;

	data.nb_phil = 0;
	while (++i < n)
	{
		pthread_mutex_init(&data.lock[i], NULL);
	//	usleep(0.1 * U_SEC);
		data.nb_phil++;
	}
	/*printf("waiting...\n");
	sleep(1);
	printf("over\n");*/
	i = -1;
	while (++i < n)
		pthread_create(&thread[i], NULL, thread_test, &data);
	//pthread_create(&thread[0], NULL, thread_test, &data);
	sleep(n * 5);
	printf("program ends now\n");
	//pthread_join(thread[0], NULL);
	//pthread_join(thread[1], NULL);
	i = -1;
	while (++i < n)
		pthread_mutex_destroy(&data.lock[i]);
	return (0);
}