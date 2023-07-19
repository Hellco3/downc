一个在Linux下基于libcurl开发的支持HTTP、HTTPS 协议的多线程下载器
提供了两种编译方式，直接通过makefile以及使用cmake，推荐通过cmake进行编译
编译方式：

```shell
#在源码目录下
cd bin
sh make.sh
```

此时在bin目录下会生成一个可执行文件`downc`，运行`downc --help`可以查看帮助信息

注：该程序的运行依赖以libcurl，liblog4cpp和libpthread，需要现在本机安装好这三个库