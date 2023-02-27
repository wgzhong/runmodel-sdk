#include <iostream>
#include <time.h>
#include <sys/time.h>    
#include <unistd.h> 
#include <stdlib.h>
long getTimeUsec()
{
    struct timeval t;
    gettimeofday(&t, 0);
    return (long)((long)t.tv_sec * 1000 * 1000 + t.tv_usec);
}

void get_local_time(char* szTime){
    time_t t; 
	time(&t); 
	time_t ta = t;
    struct tm * timelocalinfo = localtime(&ta);
	strftime(szTime, 128, "%Y-%m-%d:%H-%M-%S:%d", timelocalinfo);
    long ms = getTimeUsec();
    char buffer[16];
    snprintf(buffer,sizeof(buffer),"%d", ms);
    strcat(szTime, ":");
    strcat(szTime, buffer);
}
