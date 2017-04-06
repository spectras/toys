/** @file
 * Embedded resources.
 *
 * Used to declare binary resources embedded into the executable. Those are
 * used to avoid loading them from the disk, making the binary self-sufficient.
 *
 * Each resource declared in CMakeLists.txt is converted into a compilation
 * unit using [**xxd(1)**](http://linuxcommand.org/man_pages/xxd1.html) and
 * added during link phase. This header file declares those resources as
 * C identifiers so that can be referenced from other compilation units.
 *
 * @sa Function image_load_buffer() can load images stored in such a way.
 */
#ifndef RESOURCES_H
#define RESOURCES_H

/** Macro to declare embedded resource.
 *
 * Each resource will be defined as two symbols:
 *   - <b>`resources_&lt;name&gt;`</b> referencing the resource's data.
 *   - <b>`resources_&lt;name&gt;_len`</b> being the size of data in bytes.
 * @param name The name of the resource. Should match the filename of a binary
 *             file in the `resources` directory, with dots replaced with
 *             underscores, eg: `icon_png`.
 */
#define DECLARE_RESOURCE(name) \
    extern const unsigned char resources_##name[]; \
    extern const unsigned int resources_##name##_len;

DECLARE_RESOURCE(background_png)
DECLARE_RESOURCE(icon_png)

#undef DECLARE_RESOURCE

#endif
