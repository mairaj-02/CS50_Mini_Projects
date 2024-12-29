#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Correct Usage: ./recover input_file.\n");
        return 1;
    }
    FILE* input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Could not open the file.\n");
        return 2;
    }

    unsigned char buffer[512];
    int counter = 0;
    unsigned char hex1 = 0xff;
    unsigned char hex2 = 0xd8;
    unsigned char hex3 = 0xff;
    unsigned char hex4 = 0xe0;
    unsigned char hex_temp = 0xf0;

    FILE* output_file = NULL;

    while (fread(buffer, 512, 1, input_file) == 1)
    {
        if (buffer[0] == hex1 && buffer[1] == hex2 && buffer[2] == hex3 && (buffer[3]&hex_temp) == hex4)
        {
            if (output_file != NULL)
            {
                fclose(output_file);
            }
                char filename[8];
                sprintf(filename, "%03i.jpg", counter);
                counter++;
                output_file = fopen(filename, "w");
                if (output_file == NULL)
                {
                    printf("Could not open output_file.\n");
                    fclose(input_file);
                    return 3;
                }
                fwrite(buffer, 512, 1, output_file);
        }
        else
        {
            if (output_file != NULL)
            {
                fwrite(buffer, 512, 1, output_file);
            }

        }
    }
    if (output_file != NULL)
    {
        fclose(output_file);
    }
    fclose(input_file);
    return 0;
}