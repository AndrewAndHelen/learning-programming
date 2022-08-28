## Item27:熟悉通用引用重载的替代方法
转载自：https://blog.csdn.net/zhangyifei216/article/details/54236383
* 引言
```
1. 通⽤引⽤和重载的组合替代⽅案包括使⽤不同的函数名，通过const左值引⽤传参，按值传递参数，使⽤tag dispatch
2. 通过 std::enable_if 约束模板，允许组合通⽤引⽤和重载使⽤， std::enable_if 可以控制编译器哪种条件才使⽤通⽤引⽤的实例
3. 通⽤引⽤参数通常具有⾼效率的优势，但是可⽤性就值得斟酌
```

在Item26中对通用引用进行重载的时候会导致各种各样的问题进行了详细的描述，但是这些重载在某些场合下的确很有用，比如构造函数的重载。导致这种问题出现的原因主要是因为重载函数的类型匹配规则(最佳匹配，模版总是能实例化出最match的类型参数，这导致了在类型参数不是完全一致的情况下都会去调用通用引用版本的函数)导致，本文的目的就是为了解决这个重载问题，提出一种更通用的设计(在看本文之前最好还是要回顾一下Item26)。

* 放弃重载

在Item26中提到的logAndAdd这个函数，它有一些重载版本，为了避免和通用引用带来的问题，这里可以将这些重载函数取不同的名字来避免这个问题。这个方法虽然可以在一定程度上解决了这类情况所带来的问题，但是对于构造函数重载的情况就无能为力了，因为构造函数重载是没办法通过取不同的名字来规避，他的名字是固定无法改变的。

* 传递const T&

另外一种方法就是使用C++98中的const T&替换通用引用这种方式，这种方式在Item26中其实也介绍了，效率不高。

* 值传递

直接传值，然后在内部通过std::move，避免内部的一次拷贝操作，方法如下:
```
class Person {
  public:
    explicit Person(std::string n) : name(std::move(n)) {
      ....
    }
    explicit Person(int idx) : name(nameFromIdx(idx)) {
      .....
    }
  private:
    std::string name;   
}
```
其效率等同于传递constT&，并且当参数不支持move的时候会导致需要额外的一次复制操作。

* 基于Tag分发

 传const T&，还是值传递效率都不高，通用引用又没办法避免重载的问题，难道就没有什么办法让我们既可以使用通用引用带来效率上的提高，又可以使用重载呢？实际上这并不难，通过添加额外的参数作为tag进行区分和转发，例如下面这个例子。
```
template<typename T>
void logAndAddImpl(T&& name, std::false_type) {
  auto now = std::chrono::system_clock::now();
  log(now, "logAndAdd");
  names.emplace(std::forward<T>(name));
}

void logAndAddImpl(int idx, std::true_type) {
    logAndAdd(nameFromIdx(idx));
}

template<typename T>
void logAndAdd(T&& name) {
  logAndAddImpl(std::forward<T>(name),
                std::is_integral<T>());
}
```
通过`std::is_integral`(C++11后引入的`type_traits`)，可以判定传入的类型是什么，然后根据类型和额外添加的bool参数就可以完美解决了通用引用重载的问题(本质上是通过额外的参数来避免模版实例化出参数类型一致的版本)。
> std::is_integral在判断类型是否是int类型的时候，如果T是带有引用的时候会存在问题，所以在调用它的时候需要先去除引用，正确的写法如下:logAndAddImpl(std::forward(name), std::is_integral<std::remove_reference<T>::type>())

* 对通用引用模版进行约束

基于Tag转发这种方式固然很好，但是需要对现有的函数进行一定的改造，添加额外的参数作为Tag，在构造函数这种重载的情况下就束手无策了，因为构造函数的重载是编译器自动生成的，我们没办法去改造。还有在Item26中提到的，拷贝构造函数的参数是const T&，当要拷贝构造的对象是非const这就导致调用的是通用引用的版本。诸如此类的问题通过Tag来做的话并不轻松。

说到这里我们发现上文中提到的这些方案都没办法很好的解决Item26中提到的问题，究其原因可以认为因为通用引用是一个模版，当我们发生调用的时候，模版总是能实例化出一个参数类型一模一样的版本来，导致其参数类型兼容的重载版本没有去调用，如果我们限制了模版可以实例化出的类型的话那么就可以达到我们想要的效果了。在C++11中借助std::enable_if可以实现在满足某些情况才进行实例化模版，下面是一个简单的示例:

```
class Person {
  public:
    template<typename T,
    typename = typename std::enable_if<condition>::type> //只有当满足condition的时候才实例化模版
    explicit Person(T&& n);
    .....
};
```

> 关于std::enable_if的实现原理可以搜索下SFINAE idiom，可以参考作者之前写的一篇文章[C++惯用法之SFINAE](https://blog.csdn.net/zhangyifei216/article/details/51729915)

有了std::enable_if后剩下的要处理的问题就是要知道在哪些条件下才进行实例化操作，例如上面的Person类来说就希望当T不是Person类型的时候才会去调用通用引用版本，所以std::enable_if的condition要像如下的方式来写。
```
class Person {
  public:
    template<typename T,
    typename = typename std::enable_if<!std::is_same<Person, typename std::decay<T>::type>
    ::value>::type>
    explicit Person(T&& n);
    .....
};
```

上面的代码一眼看上去并不是那么容易理解，下面我来一点点的分析，在上面这段代码中最为核心的是std::is_same，用来判断两个类型是否相等，std::decay则是用来去除类型中附带的CV修饰符和引用等，因为用户传入的T可能是带有CV限制符的，或者是带有引用的。std::enable_if则是上文中提到的用来限制模版只在符合条件的情况下才实例化，现在的条件就是当传入的类型不是Person的时候进行模版的实例化。通过上面这个手段很好的解决了通用引用重载的问题，当用户传入的是一个Person类型的时候就不会再调用通用引用的模版函数，而是会调用编译器生成的默认拷贝构造函数。但是这里还存在另外一个问题，就是用户传入的类型可能并不是通用引用版本的模版函数所需要的，比如下面这个例子:
```
class Person {
  public:
    template<typename T,
    typename = typename std::enable_if<!std::is_same<Person, 
    typename    std::decay<T>::type>::value>> 
    explicit Person(T&& n) : name(std::forward<T>(n)) {}
    .....
  private:
    string name;
};
```
上面的代码希望用户传入的参数是可以构造成string类型的，当用户传入的类型是无法初始化name的时候，编译器在编译的时候就会抛出一大堆错误，但是很难从这些错误中发掘出重要的信息。为此在这里可以引入静态断言来辅助提供一些错误信息来告诉用户应该要传入一个可以构造std::string的类型，改造后的代码如下:

```
class Person {
public:
    template<typename T,
            typename = typename std::enable_if<!std::is_same<Person, 
            typename std::decay<T>::type>::value>>
    explicit Person(T&& n) : name(std::forward<T>(n)) {
        // 当传入的类型不可以构造std::string的时候，打印出错误信息，帮助使用者定位问题
        static_assert(std::is_constructible<std::string, T>::value, 
                      "Parameter n can't be used to construct a std::string");
    }

private:
    std::string name;
};
```
到此为此本文结束，算是比较完美的解决了通用引用和重载的问题了，整个过程从最简单的放弃重载，放弃效率，到后面的追求效率一步一步完善，并最终得以解决。