#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 4096*256

int main(int argc, char *argv[])
{
	int fd;
	unsigned char buf[BUF_SIZE] = {0};
	unsigned char rcv_buf[BUF_SIZE] = {0};
	unsigned int i;
	struct timeval tv;
	long long start_time,stop_time;
	off_t offset;
	float speed;

	int write_times = -1;

	for(i=0; i<BUF_SIZE; i++) {
		buf[i] = i/4096;
	}

	fd = open("/dev/altpciechdma",O_RDWR);
	if(fd < 0) {
	  printf("open fail\n");
	  return 0;
	}

	if (argv[1]){
		write_times = atol(argv[1]);
	}else{
		printf("Usage: ./write 10\n");
		return 0;
	}

	for (i=0;i<write_times;i++) {
		gettimeofday (&tv, NULL);
		start_time = tv.tv_usec;
		write(fd,buf,sizeof(buf));
		gettimeofday (&tv, NULL);
		stop_time = tv.tv_usec;
		printf("No.%d | t=%lld us , ", i, stop_time - start_time);
		/**
		 * delta_time = stop_time - start_time;
		 * speed = (BUF_SIZE/(1024*1024))/(delta_time*1000*1000);
		 */
		speed = BUF_SIZE*0.9536/(stop_time - start_time);
		printf("WR = %-4.2f MB/s \n",speed);

		usleep(1000*100);
	}

	close(fd);
	return 0;
}

