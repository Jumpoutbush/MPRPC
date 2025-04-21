#aux_source_directory(. SRC_LIST)

set(SRC_LIST 
    test.pb.cc
    main.cc
    )
add_library(mprpc ${SRC_LIST})

# 为mprpc设置依赖，需要注意muduo库的连接顺序
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)