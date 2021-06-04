include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/dxsdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

add_library(mfuuid mfplay.c mfuuid.c)
