#include "wall_texture.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int wall_texture_load(Texture* texture, const char* filename) {
    texture->height = 0;
    texture->width = 0;

    //initcialize
    for (int row = 0; row < TEXTURE_MAX_SIZE; row++) {
        for (int col = 0; col < TEXTURE_MAX_SIZE; col++) {
            texture->pixels[row][col] = 5;
        }
    }

    FILE* file = NULL;
    errno_t err = fopen_s(&file, filename, "r");

    if (err != 0 || file == NULL) {
        perror(filename);
        return 0;
    }

    char line[TEXTURE_MAX_SIZE + 2];

    while (texture->height < TEXTURE_MAX_SIZE && fgets(line, sizeof(line), file) != NULL) {
        size_t len = strcspn(line, "\r\n");

        if ((int)len > TEXTURE_MAX_SIZE) {
            len = TEXTURE_MAX_SIZE;
        }

        for (int pos = 0; pos < (int)len; pos++) {
            if ((int)(line[pos] - '0') < 0 && (int)(line[pos] - '0') > 9)
                return 0;
            texture->pixels[texture->height][pos] = (int)(line[pos] - '0');
        }

        if ((int)len > texture->width) {
            texture->width = (int)len;
        }

        texture->height++;
    }

    fclose(file);

    if (texture->height == 0 || texture->width == 0)
        return 0;

    return 1;
}