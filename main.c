#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void errorCheck(int arg){
	if(arg < 0) printf("Error: %s\n", strerror(errno));
}

int getRandomNumber(){
	int fd = open("/dev/random",O_RDONLY);
	errorCheck(fd);
	int buffer;
	errorCheck(read(fd, &buffer, sizeof(int)));

	close(fd);
	return buffer;
}

int main(){

	int* randomNumbersBuffer = malloc(sizeof(int) * 10);
	char* toWriteFile = "toWriteFile";
	int randomFD, toWriteFD, toReadFD;

	toWriteFD = creat(toWriteFile, 0666);
	errorCheck(toWriteFD);
	toReadFD = open(toWriteFile, O_RDONLY);
	errorCheck(toReadFD);

	printf("Getting random numbers..\n\n");
	for(int x = 0; x < 10; x++) randomNumbersBuffer[x] = getRandomNumber();
	printf("Here are the numbers:\n");
	for(int x = 0; x < 10; x++) printf("[%d]\n", *(randomNumbersBuffer + x));

	printf("Writing numbers to file [%s]\n\n", toWriteFile);
	errorCheck(write(toWriteFD, randomNumbersBuffer, sizeof(int) * 10));

	printf("Reading numbers from file [%s]\n\n", toWriteFile);
	errorCheck(read(toReadFD, randomNumbersBuffer, sizeof(int) * 10));
	printf("These numbers should be the same as the ones above:\n");
	for(int x = 0; x < 10; x++) printf("[%d]\n", *(randomNumbersBuffer + x));

	free(randomNumbersBuffer);
	close(randomFD);
	close(toWriteFD);
	close(toReadFD);

	return 0;
}