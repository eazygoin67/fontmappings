#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

static uint8_t * read_entire_file_to_buffer(const char *filename)
{
    if (filename == 0 || *filename == 0)
    {
        return NULL;
    }

    uint8_t *result = NULL;

    FILE *file = fopen(filename, "rb");
    if (file)
    {
        fseek(file, 0, SEEK_END);
        int32_t filelen = ftell(file);
        fseek(file, 0, SEEK_SET);

        if (filelen > 0)
        {
            result = (uint8_t *)malloc((size_t)filelen);
            if (result)
            {
                fread(result, 1, filelen, file);
            }
        }

        fclose(file);
        file = NULL;
    }

    return result;
}

int main(int arg_count, char **args)
{
    if (arg_count < 2)
    {
        printf("Error: No font file specified");
        return -1;
    }

    const uint8_t *font_file_data = read_entire_file_to_buffer(args[1]);
    if (font_file_data)
    {
        stbtt_fontinfo font;
        if (stbtt_InitFont(&font, font_file_data, 0))
        {
            const int32_t start_codepoint = 0;
            const int32_t end_codepoint = 10000;

            printf("Finding glyph indexes for codepoints in file '%s'\n", args[1]);

            for(int32_t codepoint = start_codepoint; codepoint <= end_codepoint; ++codepoint)
            {
                const int32_t glyph_index = stbtt_FindGlyphIndex(&font, codepoint);
                printf("  - CODEPOINT: %d (%#08x) -- GLYPH INDEX: %d\n", codepoint, codepoint, glyph_index);
            }

            printf("Done!\n");
        }
        else
        {
            printf("Error: File is not a valid font file.");
        }

        free((void *)font_file_data);
        font_file_data = NULL;
    }
    else
    {
        printf("Error: Unable to open font file");
    }
    
    return 0;
}