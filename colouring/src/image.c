/** @file
 * @copydoc image.h
 */
#include <png.h>
#include <SDL.h>
#include <stdbool.h>
#include "image.h"

/** Memory buffer descriptor
 *
 * Tracks current position in a memory buffer, ensuring it cannot be
 * overread.
 */
struct buffer_pointer {
    const void *    buffer;     /**< Address of memory buffer */
    size_t          length;     /**< Total buffer size in bytes */
    size_t          offset;     /**< Current offset; if offset == length we are at EOF */
};

static void image_buffer_reader(png_structp png_ptr, png_bytep out, png_size_t length);

SDL_Surface * image_load_buffer(const void * buffer, size_t length)
{
    struct buffer_pointer pointer;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_uint_32 width, height;
    png_byte color_depth, color_type;
    bool has_alpha;
    png_bytep * row_pointers = NULL;
    SDL_Surface * surface = NULL;
    unsigned y;

    /* Setup libpng for reading an image */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) { return NULL; }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) { goto err_free_read; }

    pointer.buffer  = buffer;
    pointer.offset  = 0;
    pointer.length  = length;
    png_set_read_fn(png_ptr, &pointer, image_buffer_reader);

    /* Read image headers */
    png_read_info(png_ptr, info_ptr);
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_depth = png_get_bit_depth(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);

    switch (color_type) {
    case PNG_COLOR_TYPE_GRAY:
        png_set_gray_to_rgb(png_ptr);
        has_alpha = false;
        break;
    case PNG_COLOR_TYPE_GRAY_ALPHA:
        png_set_gray_to_rgb(png_ptr);
        has_alpha = true;
        break;
    case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(png_ptr);
        /* fall-through */
    case PNG_COLOR_TYPE_RGB:
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(png_ptr);
            has_alpha = true;
        } else {
            has_alpha = false;
        }
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        has_alpha = true;
        break;
    default:
        goto err_free_read;
    }
    if (color_depth == 16) { png_set_strip_16(png_ptr); }

    /* Create SDL surface matching libpng pixel format */
    surface = SDL_CreateRGBSurface(
        0,
        width, height,
        has_alpha ? 32 : 24,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        0xff000000, 0x00ff0000, 0x0000ff00, has_alpha ? 0x000000ff : 0
#else
        0x000000ff, 0x0000ff00, 0x00ff0000, has_alpha ? 0xff000000 : 0
#endif
    );
    if (surface == NULL) { goto err_free_read; }

    /* Decompress image into SDL surface */
    SDL_LockSurface(surface);
    row_pointers = malloc(sizeof(png_bytep) * height);
    if (row_pointers == NULL) { goto err_free_surface; }
    for (y = 0; y < height; y += 1) {
        row_pointers[y] = (unsigned char*)surface->pixels + y * surface->pitch;
    }
    png_read_image(png_ptr, row_pointers);
    free(row_pointers);
    SDL_UnlockSurface(surface);

    /* Cleanup */
    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return surface;

err_free_surface:
    SDL_FreeSurface(surface);
err_free_read:
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return NULL;
}

/** Memory-based image reader for libpng
 *
 * Gets the next data chunk from the memory buffer.
 * @param[in] png_ptr Pointer to the png read structure.
 * @param[out] out Memory address where to put read data.
 * @param length Number of bytes to read.
 */
static void image_buffer_reader(png_structp png_ptr, png_bytep out, png_size_t length)
{
    struct buffer_pointer * pointer = (struct buffer_pointer *)png_get_io_ptr(png_ptr);
    if(pointer == NULL) { return; }

    size_t clamped_length = pointer->offset + length <= pointer->length
                          ? length
                          : pointer->length - pointer->offset;

    memcpy(out, (char*)pointer->buffer + pointer->offset, clamped_length);
    pointer->offset += clamped_length;
}
