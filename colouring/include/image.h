/** @file
 * Image processing utilities.
 *
 * Enables loading images into SDL surfaces
 */
#ifndef IMAGE_H
#define IMAGE_H

#include <stddef.h>

struct SDL_Surface;

/** Load an image from a memory buffer into a SDL surface
 *
 * The image must be a full, valid image file in
 * supported format. This means a PNG file at the moment, but it can be
 * extended to transparently identify and decode other image types.
 * @param[in] buffer Image data in supported format.
 * @param length Size in bytes of raw image data.
 * @return The loaded SDL_Surface. The caller is reponsible for calling
 *         [SDL_FreeSurface()](https://wiki.libsdl.org/SDL_FreeSurface)
 *         when done with it. Returns `NULL` if loading the image failed.
 */
SDL_Surface * image_load_buffer(const void * buffer, size_t length);

#endif
