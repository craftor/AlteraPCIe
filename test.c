#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 4096*256

int main()
{
	int fd;
	unsigned char buf[BUF_SIZE] = {0};
	unsigned char rcv_buf[BUF_SIZE] = {0};
	unsigned int i;
	struct timeval tv;
	long long start_time,stop_time;
	off_t offset;
	float speed;

	int fout;

	for(i=0; i<BUF_SIZE; i++) {
		buf[i] = i/4096;
	}

	fd = open("/dev/altpciechdma",O_RDWR);
	if(fd < 0) {
	  printf("open fail\n");
	  return 0;
	}

	for (i=0; i< 10; i++) {
		printf("Test Time %-6d : ",i);
		gettimeofday (&tv, NULL);
		start_time = tv.tv_usec;
		write(fd,buf,sizeof(buf));
		gettimeofday (&tv, NULL);
		stop_time = tv.tv_usec;

		//printf("stop time is %lld\n",stop_time);
		//printf("start time is %lld\n",start_time);

		/**
		 * delta_time = stop_time - start_time;
		 * speed = (BUF_SIZE/(1024*1024))/(delta_time*1000*1000);
		 */
		speed = BUF_SIZE*0.9536/(stop_time - start_time);
		printf("Write Speed = %-8.2f MB/s | ",speed);

		offset = lseek(fd, 0, SEEK_SET);
		gettimeofday(&tv, NULL);
		start_time = tv.tv_usec;
		read(fd,rcv_buf,sizeof(rcv_buf));
		gettimeofday(&tv, NULL);
		stop_time = tv.tv_usec;

		//printf("stop time is %lld\n",stop_time);
		//printf("start time is %lld\n",start_time);
		speed = BUF_SIZE*0.9536/(stop_time - start_time);
		printf("Read Speed = %-8.2f MB/s \n",speed);

		usleep(1000*100);
	}
	
	fout = open("./read.bin", O_RDWR);
	write(fout, rcv_buf, BUF_SIZE);
	close(fout);

	close(fd);
	return 0;
}

