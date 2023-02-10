#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

void	*thread_test(void *arg)
{
	int i = 0;

	i++;
	sleep(1);
	printf("hello this is process %d\n", i);
	printf("and process %d ends now :(\n", i);
	return (NULL);
}

int	main(void)
{
	pthread_t	thread_id;
	int			i = 0;

	while (i < 3)
	{
		pthread_create(&thread_id, NULL, thread_test, NULL);
		i++;
	}
	//pthread_join(thread_id, NULL);
	return (0);
}