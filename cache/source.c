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
#define MALLOC_SIZE 100000
#define BUFF_SIZE 128

//global variables to hold cache parameters
int cache_line_len;//Bytes
int num_of_sets;
int lines_in_set;
int cache_size;//Bytes

int isInCache(char*);
int command(const char* str);

int main()
{

	int i,j,set_size;
	char** perm;
	char* arr;

	//getting machine cache parameters
	cache_line_len = command("cat //sys//devices//system//cpu//cpu0//cache//index3//coherency_line_size ");
	num_of_sets = command("cat //sys//devices//system//cpu//cpu0//cache//index3//number_of_sets");
	cache_size = command("cat //sys//devices//system//cpu//cpu0//cache//index3//size");
	cache_size *= 1000;//return value is in KBytes
	lines_in_set = cache_size/(num_of_sets*cache_line_len);

	arr=malloc(MALLOC_SIZE*cache_line_len);

	//put the address of the next line into the previews line to mess with the cache optimizing
	for(i=0;i<MALLOC_SIZE;i++)
	{
		arr[i*cache_line_len]=arr+(i+1)*cache_line_len;
	}


	i=0;
	//add first at least a set-size amount of entries before start checking
	//keep inserting until the first entry misses
	while((isInCache(arr) && i<MALLOC_SIZE) || i<lines_in_set)
	{
		//for(j=0;j<i;j++)
		arr[i*cache_line_len]++;
		i++;
	}

	printf("miss after %d\n",i);


	set_size = i;
	//while(set_size > 12)
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

int command(const char* str)
{
	FILE* fp;
	char buff[BUFF_SIZE];
	fp = popen(str,"r");
	while(fgets(buff,BUFF_SIZE,fp)!= NULL);
	fclose(fp);
	return atoi(buff);
}
