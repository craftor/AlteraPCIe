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

	for(i=0; i<BUF_SIZE; i+=4096) {
		memset(buf+i,i/4096,4096);
	}
	fd = open("/dev/altpciechdma",O_RDWR);
	if(fd < 0) {
	  printf("open fail\n");
	  return 0;
	}

	//write(fd,buf,sizeof(buf));

	for (i=0; i< 65535; i++) {
		printf("Test Time %-6d : ",i);
		gettimeofday (&tv, NULL);
		start_time = tv.tv_usec;
		write(fd,buf,sizeof(buf));
		gettimeofday (&tv, NULL);
		stop_time = tv.tv_usec;

		//printf("stop time is %lld\n",stop_time);
		//printf("start time is %lld\n",start_time);
		speed = 0.5*1000*1000/(stop_time - start_time);
		printf("Write Speed = %-8.2f MB/s | ",speed);

		offset = lseek(fd, 0, SEEK_SET);
		gettimeofday(&tv, NULL);
		start_time = tv.tv_usec;
		read(fd,rcv_buf,sizeof(rcv_buf));
		gettimeofday(&tv, NULL);
		stop_time = tv.tv_usec;

		//printf("stop time is %lld\n",stop_time);
		//printf("start time is %lld\n",start_time);
		speed = 0.5*1000*1000/(stop_time - start_time);
		printf("Read Speed = %-8.2f MB/s \n",speed);

		usleep(1000*10);
	}
	/*
	for(i=0; i<BUF_SIZE; i++)
	{
	  if(i % 32 == 0) printf("\n %04d : ",i);
	  printf("%02X ",rcv_buf[i]);
	}
	*/
	printf("\n");
	close(fd);
	return 0;
}

