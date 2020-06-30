#添加头文件

#添加源文件
add_library(
        metadata_retriever

        SHARED

        #library
        Metadata.cpp
        com_cgfay_media_MediaMetadataRetriever.cpp
        MediaMetadataRetriever.cpp
)

#链接动态库

target_link_libraries(
        metadata_retriever

        ffmpeg

        android
        jnigraphics
        log
)