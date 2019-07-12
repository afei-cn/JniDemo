# 设置 Cmake 的最低版本号
cmake_minimum_required(VERSION 3.4.1)

# 指定包含的头文件路径
include_directories( src/main/cpp/inc )

# 搜索自动目录下的所有源代码文件，并存储在 SRC_LIST 变量中
aux_source_directory( src/main/cpp/src SRC_LIST )

# 指定生成的库的名称、类型、源码
add_library( # 设置生成库的名称
             native-lib
             # 设置生成库的类型（SHARED 动态库，STATIC 静态库）
             SHARED
             # 指定源码的路径
             ${SRC_LIST} )

# 查找预编译库，并将其保存到指定的变量名中
find_library( # 变量名，可以自己随便取
              log-lib
              # 预编译库的路径，默认搜索路径为 cmake 包含的系统库，因此如果是 NDK 的公共库只需要指定库的名称即可。
              log )

# 指定目标库需要链接的其它库
target_link_libraries( # 目标库名称
                       native-lib

                       # 需要链接的库，log-lib 的值在上述 `find_library` 方法中指定
                       ${log-lib} )