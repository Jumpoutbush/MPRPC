# aux_source_directory(. SRC_LIST)
set(SRC_LIST
    logger.cc
    mprpcapplication.cc
    mprpcconfig.cc
    mprpccontroller.cc
    mprpcchannel.cc
    mprpcprovider.cc
    rpcheader.pb.cc
    zookeeperutil.cc
    )
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)