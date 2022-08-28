## Item 8: Prefer nullptr to 0 and NULL
条款⼋:优先考虑nullptr而⾮0和NULL

* 引言
```
1.优先考虑nullptr而⾮0和NULL
2.避免重载指针和整型
```

* 8.1 C的NULL
```
在C语言中，我们使用NULL表示空指针，也就是我们可以写如下代码：

int *i = NULL;
foo_t *f = NULL;
实际上在C语言中，NULL通常被定义为如下：

#define NULL ((void *)0)
也就是说NULL实际上是一个void *的指针，然后吧void *指针赋值给int *和foo_t *的指针的时候，隐式转换成相应的类型。而如果换做一个C++编译器来编译的话是要出错的，因为C++是强类型的，void *是不能隐式转换成其他指针类型的，所以通常情况下，编译器提供的头文件会这样定义NULL：

#ifdef __cplusplus ---简称：cpp c++ 文件
#define NULL 0
#else
#define NULL ((void *)0)
#endif
```

* 8.2 C++的0
```
0具有二义性。
void f(int); //三个f的重载函数
void f(bool);
void f(void*);
f(0); //调⽤f(int)而不是f(void*)
f(NULL); //可能不会被编译，⼀般来说调⽤f(int),绝对不会调⽤f(void*)
```

* 8.3 nullptr
```
使用 nullptr 初始化对象，就能避免 0 指针的二义性的问题
```