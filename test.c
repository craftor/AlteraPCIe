#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 4096*256

int main(int argc, char **argv)
{
	int fd;
	unsigned char buf[BUF_SIZE] = {0};
	unsigned char rcv_buf[BUF_SIZE] = {0};
	unsigned int i;
	struct timeval tv;
	long long start_time,stop_time,delta_time;
	off_t offset;
	float speed;

	int test_times;

	for(i=0; i<BUF_SIZE; i++) {
		buf[i] = 0xAA;
	}

	fd = open("/dev/altpciechdma",O_RDWR);
	if(fd < 0) {
	  printf("open fail\n");
	  return 0;
	}

	if (argv[1]) {
		test_times = atol(argv[1]);
	}else{
		printf("Usage: ./test 10\n");
		return 0;
	}

	printf("Test Start, Transfer Size = %d KB.\n", BUF_SIZE/1024);

	for (i=0; i< test_times; i++) {

		printf("Test No.%-3d | ",i);
		gettimeofday (&tv, NULL);
		start_time = tv.tv_usec;
		write(fd,buf,sizeof(buf));
		gettimeofday (&tv, NULL);
		stop_time = tv.tv_usec;
		delta_time = (stop_time - start_time + 1000000)%1000000;
		printf("t=%lld us , ", delta_time);
		speed = BUF_SIZE*0.9536/delta_time;
		printf("WR = %-4.2f MB/s | ",speed);

		offset = lseek(fd, 0, SEEK_SET);
		gettimeofday(&tv, NULL);
		start_time = tv.tv_usec;
		read(fd,rcv_buf,sizeof(rcv_buf));
		gettimeofday(&tv, NULL);
		stop_time = tv.tv_usec;
		delta_time = (stop_time - start_time + 1000000)%1000000;
		printf("t=%lld us , ", delta_time);
		speed = BUF_SIZE*0.9536/delta_time;
		printf("RD = %-4.2f MB/s \n",speed);

		usleep(1000*50);
	}

	close(fd);
	return 0;
}

