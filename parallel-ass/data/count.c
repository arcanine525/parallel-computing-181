#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main()
{
    clock_t begin = clock();
    char alphabets[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    FILE *file;
    //char path[100];

    char ch, letter[1], path;
    int lines = 0;

    file = fopen("data.txt", "r");
    /* Check if file opened successfully */
    if (file == NULL)
    {
        printf("\nUnable to open file.\n");
        printf("Please check if file exists and you have read privilege.\n");

        exit(EXIT_FAILURE);
    }

    int count[strlen(alphabets)];
    for (int i = 0; i < strlen(alphabets); i++)
    {
        count[i] = 0;
    }

    // while ((ch = fgetc(file)) != EOF)
    // {

    //     for (int i = 0; i < strlen(alphabets); i++)
    //     {
    //         /* Check new line */
    //         if (ch == '\n')
    //             lines++;
    //         if (ch == alphabets[i])
    //             count[i]++;
    //     }
    // }
    char c;
    for (c = getc(file); c != EOF; c = getc(file))
    {
        if (c == '\n') // Increment count if this character is newline
            lines = lines + 1;

        for (int i = 0; i < strlen(alphabets); i++)
        {
            if (c == alphabets[i])
                count[i]++;
        }
       }

    for (int i = 0; i < strlen(alphabets); i++)
    {
        /* Print file statistics */
        printf("\n");
        printf("Total count for = %c\n", alphabets[i]);
        printf("Total count for = %d\n", count[i]);
    }
    /* Close files to release resources */

    fclose(file);
    printf("Total lines = %d\n", lines);
    clock_t end = clock();
	double runtime = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Runtime: %lfs\n", runtime / 1000);

    return 0;
}
