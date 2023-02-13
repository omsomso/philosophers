#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

typedef struct	s_data
{
	pthread_mutex_t	lock[4];
	//int	lock;
} t_data;

void	*thread_test(void *data)
{
	static int i = 0;

	t_data	*tmp;
	tmp =(t_data*)data;
	pthread_mutex_lock(&tmp->lock[0]);
	i++;
	printf("waiting in thread %d...\n", i);
	sleep(1);
	printf("and thread %d ends now :(\n", i);
	pthread_mutex_unlock(&tmp->lock[0]);
	return (NULL);
}

int	main(void)
{
	int			i = -1;
	int			n = 4;
	pthread_t	thread[n];
	t_data		data;

	while (++i < n)
		pthread_mutex_init(&data.lock[i], NULL);
	/*printf("waiting...\n");
	sleep(1);
	printf("over\n");*/
	i = -1;
	while (++i < n)
		pthread_create(&thread[i], NULL, thread_test, &data);
	//pthread_create(&thread[0], NULL, thread_test, &data);
	sleep(n);
	printf("program ends now\n");
	//pthread_join(thread[0], NULL);
	//pthread_join(thread[1], NULL);
	i = -1;
	while (++i < n)
		pthread_mutex_destroy(&data.lock[i]);
	return (0);
}