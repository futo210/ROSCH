/*
 * rt_task_qos_xcpu.c
 *
 * Sample program that executes one periodic real-time task, 
 * with CPU resource reservation that receives XCPU signal.
 * Each job consumes 2 seconds, if it is not preempted, but
 * the reserved CPU time per period is 1 second.
 * That is, only the first 1 second is ensured to run in the reserve.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <resch/api.h>

static struct timespec ms_to_timespec(unsigned long ms)
{
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms - ts.tv_sec*1000) * 1000000LL;
	return ts;
}

int xcpu_flag = 0;
void xcpu_handler(void)
{
	xcpu_flag = 1;
}

int main(int argc, char* argv[])
{
	int i;
	unsigned long prio;
	struct timespec period, runtime, timeout;
	struct timeval tv;

	if (argc != 3) {
		printf("Error: invalid option\n");
	}
	prio = atoi(argv[1]);					/* priority. */
	period = ms_to_timespec(atoi(argv[2]));	/* period. */
	runtime = ms_to_timespec(1000);			/* execution time. */
	timeout = ms_to_timespec(1000);			/* timeout. */

	/* bannar. */
	printf("sample program\n");

	rt_init(); 
	rt_set_period(period);
	rt_set_runtime(runtime);
	rt_set_scheduler(SCHED_FP); /* you can also set SCHED_EDF. */
	rt_set_priority(prio);
	rt_reserve_start(runtime, xcpu_handler); /* QoS is guaranteed. */
	rt_run(timeout);

	for (i = 0; i < 20; i++) {
		xcpu_flag = 0;
		gettimeofday(&tv, NULL);
		printf("start %lu:%06lu\n", tv.tv_sec, tv.tv_usec);
		fflush(stdout);
		while (!xcpu_flag) {
			gettimeofday(&tv, NULL);
		}
		printf("finish %lu:%06lu\n", tv.tv_sec, tv.tv_usec);
		fflush(stdout);

		if (!rt_wait_period()) {
			printf("deadline is missed!\n");
		}
	}
	rt_reserve_stop();
	rt_exit();
	
	return 0;
}
