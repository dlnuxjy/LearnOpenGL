# 构建方法1
cmake指定vs2017和Win64会报错。
这里使用cmake vs2017，以及默认的Win32配置。
但是由于工程中提供的库都是X64位的，工程生成后，需要手动配置，将x86改为X64的工程。

# 构建方法2
使用命令行
新建build文件夹，切换到build路径下
``shell
cd build
cmake -G "Visual Studio 15 2017 Win64" ../
``