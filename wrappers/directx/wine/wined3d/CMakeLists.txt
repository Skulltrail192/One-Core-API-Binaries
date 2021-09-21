
add_definitions(
    -D__WINESRC__
    -D_USE_MATH_DEFINES
    -DUSE_WIN32_OPENGL
    -D__ROS_LONG64__
    -Dcopysignf=_copysignf)

include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

# We name this wined3d.dll, because the Virtualbox additions ship with a custom wined3d.dll
# and it breaks everything if it is installed.
spec2def(wined3d.dll wined3d.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	adapter_gl.c
	adapter_vk.c
    arb_program_shader.c
    ati_fragment_shader.c
    buffer.c
    context.c
    context_gl.c
    context_vk.c
    cs.c
    device.c
    directx.c
    gl_compat.c
    glsl_shader.c
    nvidia_texture_shader.c
    palette.c
    query.c
    resource.c
    sampler.c
    shader.c
    shader_sm1.c
    shader_sm4.c
    shader_spirv.c
    state.c
    stateblock.c
    surface.c
    swapchain.c
    texture.c
    utils.c
    vertexdeclaration.c
    view.c
    wined3d_main.c
    precomp.h)

add_library(wined3d MODULE
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wined3d.def)

set_module_type(wined3d win32dll)
target_link_libraries(wined3d wine)
add_importlibs(wined3d user32 opengl32 gdi32 advapi32 msvcrt kernel32 ntdll)
add_pch(wined3d precomp.h SOURCE)
add_cd_file(TARGET wined3d DESTINATION reactos/system32 FOR all)
