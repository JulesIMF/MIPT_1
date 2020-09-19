#include <stdio.h>
#include <stdlib.h>

void jerror(const char* const message)
{
	if (!message)
		printf("An uncnown error occured\n");
	else
		printf("An error occured: %s\n", message);
	exit(-1);
}