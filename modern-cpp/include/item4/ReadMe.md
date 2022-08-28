## Item 4:Know how to view deduced types

条款四:学会查看类型推导结果

* 引言

```
类型推断可以从IDE看出，从编译器报错看出，从⼀些库的使⽤看出
这些⼯具可能既不准确也⽆帮助，所以理解C++类型推导规则才是最重要的
```

* 4.1 IDE编译器
```
const int theAnswer = 42;
auto x = theAnswer;//x int
auto y = &theAnswer;//y const int *
```

* 4.2 编译器诊断
```
//声明类模板但不定义
template<typename T> //只对TD进⾏声明
class TD; //TD == "Type Displayer"

TD<decltype(x)> xType; //引出错误消息
TD<decltype(y)> yType; //x和y的类型

编译器错误信息：
error C2079: “xType”使用未定义的 class“TD<int>”
error C2079: “yType”使用未定义的 class“TD<const int *>”
```

* 4.3 运行时输出
```
std::cout<<typeid(x).name()<<"\n"; //显⽰x和y的类型
std::cout<<typeid(y).name()<<"\n";

输出：
int
int const * __ptr64

//复杂例子
template<typename T>
void f(const T& param)param){
using std::cout;
cout<<"T= "<<typeid(T).name()<<"\n";
cout<<"param = "<<typeid(param).name()<<"\n";
...
}

std::vector<Widget> createVec();
const auto vw = createVec();
if(!vw.empty()){
f(&vw[0]);
...
}

在GNU、Clang和Microsoft中显示：
T= class Widget const *
param= class Widget const *

//结果都不可信
使用boost中类型推导库：
//GNU和Clang：
T= Widget const *
param= Widget const * const&
//Microsoft：
T= class Widget const *
param= class Widget const * const&
```