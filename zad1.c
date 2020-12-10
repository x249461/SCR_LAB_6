#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main()
{
	char userInput[32];
	char *pMap = NULL;
	const char *pChangedPart = "CHANGED ";
	int fd;
	struct stat statbuf;

	while(1)
	{
		printf("Enter a filename for mapping: ");
		scanf("%s", userInput);

		//Kontrolowane zakonczenie programu
		if(strcmp(userInput, "quit") == 0)
			break;

		fd = open(userInput, O_RDWR);

		if(fd < 0)
		{
			printf("\n%s couldn't be opened!\n", userInput);
			return -1;
		}
		//Zczytanie rozmiaru pliku
		int err = fstat(fd, &statbuf);
		
		if(err < 0)
		{
			printf("\n%s couldn't be opened!\n", userInput);
			return -1;
		}
	
		//Mapowanie pliku do pamieci roboczej
		pMap = mmap(NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		if(pMap == MAP_FAILED)
		{
			printf("\nMapping failed!\n");
			return -2;
		}
	
	
		close(fd);
		
		//Zmiana zawartosci w pliku by bylo to widoczne
		for(unsigned int i = 0; i < strlen(pChangedPart); i++)
			pMap[i] = pChangedPart[i];

		//Wypisanie zawartosci zmienionego juz obszaru pamieci
		ssize_t n = write(1, pMap, statbuf.st_size);
		if(n != statbuf.st_size)
		{
			return -3;
		}

		err = munmap(pMap, statbuf.st_size);
		if(err != 0)
		{
			return -3;
		}

	}

	printf("\nThe selected files have been changed. To test the program again,"
			 	 "the files mentioned should be changed.\n");
	return 0;
}
