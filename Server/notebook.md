### linux socket
- accept第三个参数传入一个整型指针，既作为输入又作为输出
- recv 返回0代表对方关闭了连接(tcp)

### Qt(creator)
- moc Meta Object Compiler ; 把Qt的宏、关键字展开成标准c++
- 使用了Q_OBJECT的类，要用.h和.cpp分离开来，并且通过qmake来重新生成一下makefile(后台执行了moc xxx.h)

### C++
- class 声明要加";", 而namespace不用
- 函数模板应该在头文件里定义，否则会造成调用者找不到符号的错误
