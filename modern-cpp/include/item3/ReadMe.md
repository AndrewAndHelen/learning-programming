## Item 3: Understand decltype

条款三:理解decltype

* 引言

```
1.decltype总是不加修改的产⽣变量或者表达式的类型。
2.对于T类型的左值表达式，decltype总是产出T的引⽤即T&。
3.C++14⽀持decltype(auto) ，就像auto⼀样，推导出类型，但是它使⽤⾃⼰的独特规则进⾏推
导。
```

* 3.1 基本使用方法

```
Widget w;
const Widget& cw = w;
auto myWidget1 = cw; //auto类型推导
//myWidget1的类型为Widget
decltype(auto) myWidget2 = cw; //decltype类型推导
//myWidget2的类型是const Widget&
```

* 3.2 函数返回引用
```
template<typename Container,typename Index> //最终的C++11版本
auto
authAndAccess(Container&& c,Index i)
->decltype(std::forward<Container>(c)[i])
{
authenticateUser();
return std::forward<Container>(c)[i];
}

template<typename Container,typename Index> //最终的C++14版本
decltype(auto)
authAndAccess(Container&& c,Index i){
authenticateUser();
return std::forward<Container>(c)[i];
}
```

* 3.3 特殊情况
```
decltype(auto) f1()
{
int x = 0;
...
return x; //decltype(x）是int，所以f1返回int
}

decltype(auto) f2()
{
int x =0l;
return (x); //decltype((x))是int&，所以f2返回int&
}
```
