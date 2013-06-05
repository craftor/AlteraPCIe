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
	long long start_time,stop_time;
	off_t offset;
	float speed;
	int fout;
	int read_times;

	for(i=0; i<BUF_SIZE; i++) {
		buf[i] = i/4096;
	}

	fd = open("/dev/altpciechdma",O_RDWR);
	if(fd < 0) {
	  printf("open fail\n");
	  return 0;
	}

	if (argv[1]) {
		read_times = atol(argv[1]);
	}else{
		printf("Usage: ./read 10\n");
		return 0;
	}

	for(i=0;i<read_times;i++) {
		offset = lseek(fd, 0, SEEK_SET);
		gettimeofday(&tv, NULL);
		start_time = tv.tv_usec;
		read(fd,rcv_buf,sizeof(rcv_buf));
		gettimeofday(&tv, NULL);
		stop_time = tv.tv_usec;

		speed = BUF_SIZE*0.9536/(stop_time - start_time);
		printf("RD Speed = %-4.2f MB/s \n",speed);

		usleep(1000*100);
	}
	for(i=0;i<256;i++) {
		if (i%32==0)
		  printf("\n");
		printf("%02X ",rcv_buf[i]);
	}
	printf("\n");

	/*
	fout = open("./read.bin", O_RDWR);
	write(fout, rcv_buf, BUF_SIZE);
	close(fout);
	*/

	close(fd);
	return 0;
}

