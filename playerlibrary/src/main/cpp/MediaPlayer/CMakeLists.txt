
#根据API版本判断使用哪个版本的OpenGLES
if (${ANDROID_PLATFORM_LEVEL} LESS 12)
    message()
    return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
    add_definitions("-DDYNAMIC_ES3")
    set(GLES-lib GLESv2)
else()
    set(GLES-lib GLESv3)
endif(${ANDROID_PLATFORM_LEVEL LESS 11})

# 添加 soundtouch 动态库
set(soundtouch_dir ../SoundTouch)

# 添加GLFIlter库
set(filter_dir ../CainFilter)

# 添加头文件路径
include_directories(
        source
        android
        ${soundtouch_dir}
        ${filter_dir}
)

# 添加源文件
add_library(
        media_player

        SHARED

        #library TODO
        source/common/FFmpegUtils.cpp
        )

# 链接静态库
target_link_libraries(
        media_player

        ffmpeg
        soundtouch
        yuv
        cainfilter

        -landroid
        -llog
        -lOpenSLES
        -lEGL
        ${GLES-lib}
)