#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // Create a buffer
    BYTE* buffer = malloc(BLOCK_SIZE);
    char filename[8];
    int i = 0;

    // Iterate over the raw file, read 512 bytes (1 block) at once
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // The start of jpg is found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // It is the first jpg
            if (i == 0)
            {
                FILE *img = fopen("000.jpg", "w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
                fclose(img);
                i++;
            }
            else
            {
                sprintf(filename, "%03i.jpg", i);
                FILE *img = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, img);
                fclose(img);
                i++;
            }
        }
        else
        {
            // Have found a jpg
            if (i != 0)
            {
                sprintf(filename, "%03i.jpg", i-1);
                FILE *img = fopen(filename, "a");
                fwrite(buffer, 1, BLOCK_SIZE, img);
                fclose(img);
            }
        }
    }
    // Close file and free memory allocation
    fclose(raw_file);
    free(buffer);
    return 0;
}