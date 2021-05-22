## Item 9:Prefer alias declarations to typedefs

条款九:优先考虑别名声明而⾮typedefs

* 引言
```
1.typedef不⽀持模板化，但是别名声明⽀持。
2.别名模板避免了使⽤"::type"后缀，而且在模板中使⽤typedef还需要在前⾯加上typename
3.C++14提供了C++11所有类型转换的别名声明版本
```

* 9.1 using命名别名
```
template<typename T>
using MyAllocList = std::list<T,MyAlloc<T>>;
MyAllocList<Widget> lw;

如果你在⼀个模板内部使⽤类型参数，你也需要在它们前⾯加上typename。
template <class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
```