## Item33:对于std::forward的auto&&形参使用decltype
转载自：https://blog.csdn.net/zhangyifei216/article/details/66970792
* 引言
```
对 auto&& 参数使⽤ decltype 来（ std::forward ）转发参数
```
 在C++14中最令人激动的特性当属通用lambda了，通用lambda指的就是使用auto来作为lambda的参数。实现通用lambda也是很方便的，我们知道lambda本质上是一个仿函数，也就是实现了operator()操作符的类。而实现通用lambda其实就是对operator()操作符进行模版化了，下面这个例子就是通用模版的简单实现：
```
auto f = [](auto x){ return func(normalize(x)); };

class SomeCompilerGeneratedClassName { 
 public:
    template<typename T>
    auto operator()(T x) const
    { return func(normalize(x)); }
    ... 
};
```
面的代码中，参数x会转发给normalize，但是如果normalize希望接收一个右值的话，上面的代码是无法满足的，因为x总是左值(参考Item3)。所以上面的代码并不完善，需要改成完美转发的形式。这就需要对上面的代码进行改造，首先需要将行参x的类型变成通用引用(参考Item24)。接着通过std::forward(参考Item25)将参数x转发给normalize。改造后的代码如下：
```
auto f = [](auto&& x)
{ return func(normalize(std::forward<???>(x))); };
```

但是上面的代码似乎出现了一点问题，std::forward需要指定转换的类型，但是通用lambda中参数类型都是auto，用户传入的类型也无法事先知道，所以这就导致了无法使用std::forward。在Item28中提到过，对于通用引用来说，如果传递的是一个左值那么最终参数的类型就是左值，如果传递的是右值那么最终参数的类型会被推导成右值。因此通过通用引用可以正确的推导出用户传入的参数类型。既然用户的参数在传入的时候其类型被正确推导出来了，那么如何得到参数的类型，然后传递给std::forward呢?，在Item3中提及的decltype就可以得到参数的类型。经过改造后的通用lambda实现如下：
```
auto f = [](auto&& param) {
    return func(normalize(std::forward<decltype(param)>(param)));
};
```