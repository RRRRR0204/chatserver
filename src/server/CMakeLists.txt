# 定义SRC_LIST变量，包含了该目录下所有的源文件
aux_source_directory(. SRC_LIST)

# 指定由哪些文件生成可执行文件
add_executable(chatserver ${SRC_LIST})

# 需要依赖的库文件
target_link_libraries(chatserver muduo_net muduo_base pthread)