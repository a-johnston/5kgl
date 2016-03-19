set(HEADERS 5kgl.h)
set(SOURCES color.c mesh.c render.c space_math.c util.c)

add_library(5kgl ${SOURCES} ${HEADERS})

set_target_properties(5kgl_PROPERTIES
    OUTPUT_NAME "lib5kgl"
    POSITION_INDEPENDENT_CODE ON)
