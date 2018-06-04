#ifndef RESOURCES_H
#define RESOURCES_H

#ifdef __cplusplus
extern "C" {
#endif
#define RESOURCE(name) \
    extern const char name[]; \
    extern const unsigned int name ## _len;


RESOURCE(shaders_fragment_glsl)
RESOURCE(shaders_vertex_glsl)

#undef RESOURCE
#ifdef __cplusplus
}
#endif

#endif
