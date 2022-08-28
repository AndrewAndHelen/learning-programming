## Item23: 理解std::move和std::forward
转载自：https://blog.csdn.net/zhangyifei216/article/details/53472182
* 引言
```
1.std::move 执⾏到右值的⽆条件的转换，但就⾃⾝而⾔，它不移动任何东西。
2.std::forward 只有当它的参数被绑定到⼀个右值时，才将参数转换为右值。
3.std::move 和 std::forward 在运⾏期什么也不做。
```

* std::move的实现
```
template <typename T>
typename remove_reference<T>::type&& move(T&& param) {
  using ReturnType = typename remove_reference<T>::type&&;
  return static_cast<ReturnType>(param);
}
```
上面就是 std::move的简易实现，说白了就是一个类型转换，只不过在类型转换之前使用了remove_reference去除类型的引用标示符。在后面的文章中说到，当T是一个引用类型的时候T&&会发生引用折叠就会变成一个普通的引用类型。因此上面的代码为了防止T带有引用，所以调用了remove_reference去除类型的引用部分。在C++14中上面的代码可以实现的更简洁:
```
template<typename T>
decltype(auto) move(T&& param) {
  using ReturnType = remove_refernce_t<T>&&;
  retrun static_cast<ReturnType>(param);
}
```

所以std::move，并没有像它的名字一样神奇，只是做了一个类型转换而已，std::move的目的就是让编译器把修饰的变量看做是右值，进而就可以调用其移动构造函数。然后std::move并不是总能成功，好在即使std::move不成功的话，它也会偷偷的使用拷贝构造完成功能，不让用户察觉到错误。

```
class Annotation {
  public:
    explicit Annotation(std::string text) : text_(text) //会进行两次拷贝
    std::string text_;
}

class Annotation {
 public:
  explicit Annotation(std::string text) : text_(std::move(text)) {} //一次拷贝，一次移动
  std::string text_;
}; 

可以在构造Annotation的时候，传入一个右值，这样就是两次移动了。

class Annotation {
 public:
  //这里换成了带有const
  explicit Annotation(const std::string text) : text_(std::move(text)) {} //二次拷贝
  std::string text_;
}; 
```

当把参数换成带有const的时候，std::move失效了，但是程序执行正常，因为它会偷偷的调用拷贝构造函数。std::move(text)返回的是一个const右值，而标准库的std::string的移动构造参数是一个右值引用类型，它只能指向右值(不带const)，因此在这里不能调用移动构造函数，只能转而去调用拷贝构造函数，拷贝构造函数的参数是一个const引用，它是可以指向一个const右值的。从另外一个反面也也看出这种做法是对的，如果这里可以移动，那么因为移动后可以是不带const的，那么这就会将一个带有const的变量，间接的转化成不带const的变量，这违背了const的含义。

​讨论完std::move后，让我们来看看std::forward的真正面目的，它和std::move很像，std::forward本质上也是一个类型转换，只不过std::move是无条件的转换，而std::forward是有条件的转换。先来回忆下std::forward的典型用法:

```
void process(const Widget& lvalArg);
void process(Widget&& rvalArg);

template<typename T>
void logAndProcess(T&& param) {
  auto now = std::chrono::system_clock::now();
  process(std::forward<T>(param));
}

Widget w;
logAndProcess(w);   //左值调用
logAndProcess(std::move(w)); //右值调用
```

上面的代码期望当传入logAndProcess是左值的时候，调用process的左值重载，当传入的是右值的时候，调用process右值的重载，但是如果参数传入后，被行参接收后都会变成左值(T&& param这里的param是左值，尽管它的类型是T&&，这只是表明了它可以接收右值)。那么std::forward的目的就是在当传入的是右值的时候将其转换为右值类型。也就是上面说到的有条件的转换，现在的问题就是std::forward是如何知道传入的是右值?这将又是一个新的话题，后续文章会进行分析。
