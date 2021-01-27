# STL

- Utility



## 一. 了解typename的双重意义

- **声明template类型参数**
- **验明嵌套从属类型**

### 1. 声明template类型参数

```c++
template<class T>class Widget;			//其中class与typename意义相同。
template<typename T>class Widget;		
```


### 2. 验明嵌套从属类型

**从属名称（dependent names）**：template内出现的名称如果相依于某个template参数，称之为从属名称（dependent names）。

**嵌套从属名称（nested dependent names）**:如果从属名称在class内呈嵌套状，我们称它为嵌套从属名称（nested dependent names）。

```c++
template<typename C>
void print2nd(const C& container)
{
	if(container.size()>2){
		C::const_iterator iter(container.begin());
		++iter;
		int value = *iter;
		std::cout << value;
	}
}
```

- C::const_iterator也是一个类型，则C::const_iterator是一个嵌套类，也叫嵌套从属名称，此时我们必须要告诉c++说C::const_iterator是个类型，所以在它之前放置关键字typename即可。

```c++
template<typename C>
void print2nd(const C& container)
{
	if(container.size()>2){
		typename C::const_iterator iter(container.begin());
		++iter;
		int value = *iter;
		std::cout << value;
	}
}
```

- 使用关键字**typename**标识嵌套从属类型名称时，不能在**base class lists**（基类列）或**member initialization list**（成员初始列）内以他作为base class修饰符。

```c++
template<typename T>
class Derived: public Base<T>::Nested{			//base class list 不允许“typename”
public:
    explict Derived(int x)
        :Base<T>::Nested(x)						//member initialization list不允许“typename”
        {
            typename Base<T>::Nested temp;		//嵌套从属类型名称
            ...								//即不在base class lists或member initialization list
        }  									//作为一个base class修饰符需加上“typename”
    ...
}
```



## 二. Using关键字用法总结

- **配合命名空间，对命名空间权限进行管理**
- **类型重命名**
- **继承体系中，改变部分接口的继承权限**

### 1. **配合命名空间，对命名空间权限进行管理**

```c++
using namespace std;//释放整个命名空间到当前作用域
using std::cout;    //释放某个变量到当前作用域
```

### 2.**类型重命名**

```
#include <iostream>
using namespace std;
#define DString std::string    //! 不建议使用！

typedef std::string TString;   //! 使用typedef的方式

using Ustring = std::string;   //！使用 using typeName_self = stdtypename;

//更直观
typedef void (tFunc*)(void);
using uFunc = void(*)(void);

int main(int argc, char *argv[])
{

    TString ts("String!");
    Ustring us("Ustring!");    
    string s("sdfdfsd");　　 cout<<ts<<endl;
    cout<<us<<endl;
    cout<<s<<endl;
    return 0;
}
```

### 3.**继承体系中，改变部分接口的继承权限**

- 应用场景1，我们需要私有继承一个基类，然后又想将基类中的某些public接口在子类对象实例化后对外开放直接使用。

```c++
#include <iostream>
#include <typeinfo>

using namespace std;

class Base
{
public:
    Base()
    {}
    ~Base(){}

    void dis1()
    {
        cout<<"dis1"<<endl;
    }
    void dis2()
    {
        cout<<"dis2"<<endl;
    }
};

class BaseA:private Base
{
public:
    using Base::dis1;				//需要在BaseA的public下释放才能对外使用，
    void dis2show()
    {
        this->dis2();
    }
};

int main(int argc, char *argv[])
{

    BaseA ba;
    ba.dis1();						//如果不使用	using Base::dis1；子类无法使用dis1()方法
    ba.dis2show();

    return 0;
}
```

- 应用场景2，子类如果想重写父类函数，就会隐藏父类中所有同名函数。此时只有通过显示方式调用父类同名函数

```c++
class Base
{
    public:
        void func() { // some code}
        int func(int n) { // some code}
}

class Sub : public Base
{
    public:
        using Base::func;
        void func() { // some code}
        
}

int main()
{
    Sub s;
    s.func();
    s.func(1); // Success!
}

```



## 二. 调用模板类的模板函数注意事项

> According to C++'03 Standard 14.2/4:
>
> When the name of a member template specialization appears after `.` or `->` in a postfix-expression, or after nested-name-specifier in a qualified-id, and the postfix-expression or qualified-id explicitly depends on a template-parameter (14.6.2), the member template name must be prefixed by the keyword `template`. Otherwise the name is assumed to name a non-template.

> 翻译：当成员模板专门化名称出现在后缀表达式中的`.`或者`->`后面，或者在定义的变量中嵌套名称说明符后面，并且后缀表达式或者变量显式的依赖一个模板参数，成员模板名称必须显式的加入前缀`template`。否则该成员被声明为一个非模板成员。

```c++
template<typename PointSource, typename PointTarget>
class LsqRegistration : public pcl::Registration<PointSource, PointTarget, float> {
public:
  using Scalar = float;
  using Matrix4 = typename pcl::Registration<PointSource, PointTarget, Scalar>::Matrix4;		//Matrix4是一个模板类
  ...
}

template<typename PointTarget, typename PointSource>
void LsqRegistration<PointTarget, PointSource>::computeTransformation(PointCloudSource& output, const Matrix4& guess) {
	Eigen::Isometry3d x0 = Eigen::Isometry3d(guess.template cast<double>());					//cast是Matrix4模板类中模板成员函数
	...																							//调用时需要加template
}
```

> *Tip:[cpp-template material.](https://github.com/Walton1128/CPP-Templates-2nd--.git)*:eyes:  