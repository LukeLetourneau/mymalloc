#include <time.h>
#include <stdio.h>
#include <stdlib.h>

//#define USE_STD_MALLOC // Define to test the test cases using malloc() and free() from the standard library

#ifndef USE_STD_MALLOC
	#include "mymalloc.h"
#endif

#define TEST_ITERATIONS 50
#define MEASURE_MICRO_SEC(x) ({clock_t start = clock(); x(); clock_t diff = clock() - start; diff * 1000000 / CLOCKS_PER_SEC;})
#define AVG_TIME(x) ({double tot = 0; for(int i = 0; i < TEST_ITERATIONS; i++) { tot += MEASURE_MICRO_SEC(x); } tot / TEST_ITERATIONS;})

// Allocate and immediately free 120 1-byte chunks
void test1() {
	for(int i = 0; i < 120; i++) {
		char *n = (char*) malloc(sizeof(char));
		free(n);
	}
}

// Allocate 120 1-byte chunks and store pointer in array, then free all chunks
void test2() {
	char *ptrs[120];

	for(int i = 0; i < 120; i++) {
		ptrs[i] = (char*)malloc(sizeof(char));
		//printf("%d\n", i);
	}

	for(int i = 0; i < 120; i++) {
		free(ptrs[i]);
	}
}

// Randomly choose between allocating a 1-byte chunk or freeing one (if possible) 120 times
void test3() {
	int numAllocated = 0;
	char *ptrs[120];
	for(int i = 0; i < 120; i++) {
		if(rand() % 2) {
			ptrs[numAllocated++] = (char*)malloc(sizeof(char));
		} else {
			if(numAllocated > 0) {
				free(ptrs[--numAllocated]);
			}
		}
	}
	for(int i = 0; i < numAllocated; i++) 
	{
		free(ptrs[i]);
	}
}



// Custom test #1
void test4() {
	char* ptrs[MEMSIZE];
	int ptridx = -1;
	int memidx = 0;
	/*int memcount = 0;
	int length = rand() % (10) + 1;
	char* pointer = (char*)malloc(length);
	while(pointer != NULL) {
		ptrs[ptridx++] = pointer;
		//memcount += 3 + length;
		length = rand() % (10) + 1;
		//if(memcount+3+length > MEMSIZE)
		{
			break;
		}
		pointer = (char*)malloc(length);
	}*/
	while(memidx + metasize() + 10 < MEMSIZE) {
		int len = rand() % 10 + 1;
		ptrs[++ptridx] = (char*)malloc(len);
		memidx += metasize() + len;
	}

	/*if(ptrs[ptridx - 1] + sizeof(meta) + sizeof(char) <= MEMSIZE) {
		pointer = (char*)malloc(sizeof(char));
		while(pointer!=NULL) {
			ptrs[ptridx++] = pointer;
			memcount += 4;
			if(memcount + 4 > MEMSIZE) {
				break;
			}
			pointer = (char*)malloc(sizeof(char));
		}
	}*/
	while(memidx + metasize() + sizeof(char) < MEMSIZE) {
		ptrs[++ptridx] = (char*)malloc(sizeof(char));
		memidx += metasize() + sizeof(char);
	}
	//memdump();
	for(int i = 0; i <= ptridx; i += 2) {
		//printf("freeing %u \n",ptrs[i]);
		free(ptrs[i]);
	}

	for(int i = 1; i <= ptridx; i += 2) {
		//printf("freeing %u \n",ptrs[i]);
		free(ptrs[i]);
	}
	//memdump();
	void* pointer = malloc(MEMSIZE - metasize());
	if(pointer == NULL) {
		printf("FAILED\n");
	}
	free(pointer);
	//printf("SUCCESS\n");
}

int empty(char* pointers[], int index)
{
	for (int i = 0; i<index;i++)
	{
		if(pointers[i]!=NULL)
		{
			return 1;
		}
	}
	return 0;
}

// Custom test #2
void test5() {
	int count = 0;
	int index = 0;
	char* ptrs[4096];
	while(count+4<=4096)
	{
		char* pointer = (char*)malloc(sizeof(char));
		count+=4;
		ptrs[index] = pointer;
		index++;
	}
	while(empty(ptrs,index)==1)
	{
		int length = rand() % index;
		if(ptrs[length] == NULL)
		{
			continue;
		}
		free(ptrs[length]);
		ptrs[length] = NULL;
	}

	char* temp = (char*)malloc(4096 - 3);
	if(temp == NULL) {
		printf("FAILED\n");
	}
	free(temp);
}

int main(int argc, char* argv) {
	time_t t;
	srand(time(&t));

	printf("Test 1 average: %f μs\n", AVG_TIME(test1));
	printf("Test 2 average: %f μs\n", AVG_TIME(test2));
	printf("Test 3 average: %f μs\n", AVG_TIME(test3));
	printf("Test 4 average: %f μs\n", AVG_TIME(test4));
	printf("Test 5 average: %f μs\n", AVG_TIME(test5));
	//test4();
	// printf("init");
	// memdump();
	// printf("____\n");

	// char* short_str = malloc(sizeof(char) * 6);
	// printf("first %u \n", short_str);
	// short_str[0] = '1';
	// short_str[1] = '2';
	// short_str[2] = '3';
	// short_str[3] = '4';
	// short_str[4] = '5';
	// short_str[5] = '6';
	// memdump();
	// printf("____\n");

	// char* short_str2 = malloc(sizeof(char) * 6);
	// printf("second %u \n", short_str2);
	// short_str2[0] = 'a';
	// short_str2[1] = 'b';
	// short_str2[2] = 'c';
	// short_str2[3] = 'd';
	// short_str2[4] = 'e';
	// short_str2[5] = 'f';
	// memdump();
	// printf("____\n");


	// char* short_str3 = malloc(sizeof(char) * 5);
	// printf("third %u \n", short_str3);
	// short_str3[0] = 't';
	// short_str3[1] = 'u';
	// short_str3[2] = 'v';
	// short_str3[3] = 'w';
	// short_str3[4] = 'x';
	// memdump();
	// printf("____\n");


	

	// char* short_str4 = malloc(sizeof(char) * 3);
	// if(short_str4!=NULL)
	// {
	// printf("fourth %u \n", short_str4);
	// short_str4[0] = 'a';
	// short_str4[1] = 'b';
	// short_str4[2] = 'c';
	// }

	// memdump();
	// printf("____\n");
	
	// printf("third freed\n");
	// free(short_str3);
	// memdump();
	// printf("____\n");
	// printf("secind freed\n");
	// free(short_str2);
	// memdump();
	// printf("____\n");
	// printf("first freed\n");
	// free(short_str);
	// memdump();
	// printf("____\n");
	// printf("fourth freed\n");
	// free(short_str4);
	// memdump();
	// printf("____\n");


	// printf("first freed again\n");
	// free(short_str);
	// memdump();
	// printf("____\n");
	
}
