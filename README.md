# ChatServer
## 1. 简介
基于muduo网络库实现的集群聊天服务器

## 2. 开发环境
- centos
- [nlohmann](https://github.com/nlohmann) 编写的在C++下使用的 [JSON库](https://github.com/nlohmann/json)
- boost + muduo网络库开发环境
- mysql数据库
- nginx的tcp负载均衡模块
- redis以及C++开发用的[hiredis](https://github.com/redis/hiredis)
- CMake

## 3. 编译
- 本项目由CMake构建，执行`./autobuild`即可（gcc/g++编译器必须保证在5.4 - 7.8版本之间）

