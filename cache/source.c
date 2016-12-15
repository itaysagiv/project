/*
 * source.c
 *
 *  Created on: Dec 15, 2016
 *      Author: itays
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MISS_TIME_TH 50 //nano sec
#define CACHE_LINE_LEN 64
#define CACHE_SET_SIZE 12
#define MALLOC_SIZE 100000


int isInCache(char*);

int main()
{

	int i,j,set_size;
	char** perm;
	char* arr = malloc(MALLOC_SIZE*CACHE_LINE_LEN);

	//put the address of the next line into the previews line to mess with the cache optimizing
	for(i=0;i<MALLOC_SIZE;i++)
	{
		arr[i*CACHE_LINE_LEN]=arr+(i+1)*CACHE_LINE_LEN;
	}


	i=0;
	//add first at least a set-size amount of entries before start checking
	//keep inserting until the first entry misses
	while((isInCache(arr) && i<MALLOC_SIZE) || i<CACHE_SET_SIZE)
	{
		//for(j=0;j<i;j++)
		arr[i*CACHE_LINE_LEN]++;
		i++;
	}

	printf("miss after %d\n",i);


	set_size = i;
	while(set_size > 12)
	{

	}

	free(arr);
	return 0;
}

int isInCache(char* addr)
{
	struct timespec start,end;

	clock_gettime(CLOCK_REALTIME,&start);
	(*addr)++;
	clock_gettime(CLOCK_REALTIME,&end);

	//printf("%d\n",(end.tv_nsec - start.tv_nsec));

	if((end.tv_nsec - start.tv_nsec)>MISS_TIME_TH)
		return 0;
	return 1;
}
