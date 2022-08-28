## Item16:让const成员函数线程安全

* 引言
```
1.确保const成员函数线程安全，除⾮你确定它们永远不会在临界区（concurrent context）中使⽤。
2.std::atomic可能⽐互斥锁提供更好的性能，但是它只适合操作单个变量或内存位置。
```

* 16.1 const成员函数用法

```
const成员函数需要在声明和定义的时候在函数头部的结尾加上 const 关键字

#include<iostream> 
using namespace std; 
class A  
{  
public:  
  void fun()const 
  { 
    cout<<"const 成员函数!"<<endl; 
    } 
  void fun() 
  { 
    cout<<"非const成员函数 !"<<endl; 
  } 
};  
int main() 
{ 
  const A a; 
  a.fun();//const类对象只能调用const成员函数 
  
  A b;
  b.fun();//非const类对象可以调用const/非const成员函数
} 


```

* 16.2 将互斥锁用于临界区变量的访问修改
```
class Polynomial {
public:
	using RootsType = std::vector<double>;
	RootsType roots() const
    {
        std::lock_guard<std::mutex> g(m); // lock mutex
        if (!rootsAreVaild) { // 如果缓存⽆效
        	rootsAreVaild = true; // 计算/存储roots
        }
        return rootsVals;
    } 	// unlock mutex
private:
    mutable std::mutex m;//const成员函数中不能改变成员变量，声明mutable则可以改变这个规则
    mutable bool rootsAreVaild { false };
    mutable RootsType rootsVals {};
};
```
