## Item41:对于那些可移动总是被拷贝的形参使用传值方式
转载自：https://blog.csdn.net/zhangyifei216/article/details/72803828
* 引言
```
1. 对于可复制，移动开销低，而且⽆条件复制的参数，按值传递效率基本与按引⽤传递效率⼀致，而且易于实现，⽣成更少的⽬标代码 
2. 通过构造函数拷⻉参数可能⽐通过赋值拷⻉开销⼤的多 
3. 按值传递会引起切⽚问题，所说不适合基类类型的参数
```

函数的参数传递方式有很多种，可以是值拷贝、传指针、传引用、甚至是移动。下面这个addName函数就是将外部传入的值存放到其内部的一个私有的容器中，为了让传递的过程更加高效，addName同时支持引用传递和移动两种方式。
```
class Widget {
  public:
   void addName(const std::string& newName) {
     names.push_back(newName);
   }
   void addName(std::string&& newName) {
     names.push_back(std::move(newName));
   }
  private:
    std::vector<std::string> names;
}
```
同样的一个功能但是却有两份代码，需要写两份函数文档，存在一定的冗余。为此可以通过C++的模版将上面两个函数进行合并，代码如下：
```
class Widget {
  public:
   template<typename T>
   void addName(T&& newName) {
     names.push_back(std::forward<T>(newNames));
   }
}
```
虽然通过上面的代码减少了代码的冗余，但是通用引用会带来一些其它的问题，作为一个模版，addName的实现必需放在头文件中，于此同时它也会实例化出不同的类型。在item30中就提到了很多关于通用引用失效的问题，还有在Item26中提到的的函数重载问题。因此通用引用的这个方案并没有想象中那么好用。那是否意味着我们要放弃这个方案呢，也不尽然，凡事都是要分情况讨论的，没办法一概而论。上文中的addName更适合传值，可以是下面这种写法：
```
class Widget {
  public:
   void addName(std::string newName) {
     names.push_back(std::move(newName));   // 避免一次拷贝，直接move过来
   }
}
```

上面的代码中，在内部通过std::move避免了一次拷贝，因为我们知道行参newName其实是一个临时的值，不会被其它代码访问到，这也是最后一次对newName的访问，所以是可以直接移动过来的。这种方法的好处就是避免了将代码写在头文件中，避免了通用引用带来的一些失败的case，避免出现一些莫名奇妙的编译错误。坏处自然就是多了一次字符串的拷贝，性能可能会有损失。

在C++98中无论用户传递的是什么，上面的代码都避免不了一次字符串的拷贝过程，但是到了C++11则不然，如果传递的是左值，那么很自然是要进行拷贝的，如果传递的是右值会直接调用移动构造赋值操作符，这样就不会进行拷贝了。

总结一下，函数的参数传递可以归为下面三种形式。
```
// 第一种基于重载的
class Widget {
  public:
   void addName(const std::string& newName) {
     names.push_back(newName);
   }
   void addName(std::string&& newName) {
     names.push_back(std::move(newName));
   }
........
}

// 第二种基于通用引用的
class Widget {
  public:
   template<typename T>
   void addName(T&& newName) {
     names.push_back(std::forward<T>(newNames));
   }
}

// 第三种基于值拷贝的
class Widget {
  public:
   void addName(std::string newName) {
     names.push_back(std::move(newName));   // 避免一次拷贝，直接move过来
   }
}
```

前两种可以统称为基于引用的方法，因为它们都是通过引用来传递参数的，下面是一组函数传递的场景，值得好好研究一下：
```
Widget w;
std::string name("Bart");
w.addName(name);
w.addName(name + "Jenne");
```

现在我么来谈论一下上面这段代码的性能，首先忽略掉编译器对上面代码可能做的优化，因为这类优化都是和编译器相关的，有一定的上下文，我们分析性能的时候应该撇开这些干扰因素。

* `基于重载` 这种参数传递方式对于传递的值来说，无论是左值还是右值都是可以传递的，对于上面两个场景来说几乎是没有开销的，前者基于引用传递，后者是直接移动构造。到了内部前者需要进行一次拷贝才能 放到容器中，后者是直接move进去的，所以总的来说对于左值需要一次拷贝，对于右值需要一次move。
* `基于通用引用` 开销和基于重载的一致，对于左值需要一次拷贝，对于右值需要一次move，std::forward可以区分传入的是否是左值还是右值。但是如果用户传入的是char*并不是string的话，基于重载的方法开销会变得更大，因为char*需要进行临时构造为一个std::string对象，然后再进行一次内部拷贝，但是通用引用的这个版本不会导致这个问题(Item25对这个问题有了近一步讲解)。
* `传值` 如果传递的是左值就需要进行一次拷贝构造，如果是右值则需要进行一次移动构造，无论左值还是右值在函数内部都需要进行一次move操作。总的来说就是对于左值是一次拷贝一次move，对于右值就是两次move。无论是左值还是右值传递相比于上面两种都多了一次额外的拷贝和移动。

​让我们重新来看一下本篇文章的标题，Consider pass by value for conpyable parameters that are cheap to move and always copied，为何要这么说呢，有以下四点原因：

1. 如果只考虑传值的话，那么就不会有多份代码，不用重载，也避免使用通用引用带来的问题，但是却带来了性能损耗，但是事实并不总是这样的。

2. 对于可拷贝的参数考虑使用传值的方式， 对于这种情况只有当参数是只能移动的时候才会失败。因为如果参数不是可拷贝的，那么就必须通过移动构造进行拷贝。所以当参数只能移动的时候只需要提供一个右值引用的版本来进行参数传递，例如std::unique_ptr这就是一个move-only的类型，所以当要传递std::unique_ptr的参数时，只需要提供右值引用的版本即可，代码如下：
```
class Widget {
 public:
  void setPtr(std::unique_ptr<std::string>&& ptr) {
    p = std::move(ptr);
  }
 private:
  std::unique_ptr<std::string> p;
};

Widget w;
w.setPtr(std::make_unique<std::string>("Modern C++"));
```

上面的代码总共需要花费1次move的开销，如果上面的代码只提供了值传递的方式的话，会导致隐式的调用移动赋值操作符(因为std::unique_ptr是move-only类型的，所以不会调用拷贝构造)，然后内部再做一次move，所以总共需要花费2次move的开销。

1. 只有当值拷贝比移动开销低的时候才选择使用传值方式。当move操作开销低的时候，即使多一次move动作也是可以接受的，但是当move操作开销大的时候，执行一次不必要的move操作和执行一次不必要的拷贝操作是类似的，都违反了C++98中避免值拷贝这一规则。

2. 只有当参数总是需要拷贝的时候才使用传值的方式，考虑一种情况，假设当我们拷贝一个参数到一个内部的容器时，我们可能会在此之前会对这个参数进行一些合法性检查，只有当满足条件的时候才会考虑添加到容器中，否则不添加，可以使用下面代码表示:
```
class Widget {
 public:
  void addName(std::string newName) {
    if ((newName.length() >= minLen) &&
        (newName.length() <= maxLen)) {
        names.push_back(std::move(newName));      
    }
  }
}
```

上面的代码中，newName并不总会拷贝到内部容器中，如果添加的newName不符合条件就会被丢弃，就会造成newName的构造和析构的开销，这种情况下引用传递则会显得开销更小。

即使当拷贝开销要比移动开销低的时候，某些情况下也不适合使用值传递的方式，这是因为函数拷贝参数有两种方式，一种是构造(拷贝构造或移动构造)，另外一种则是赋值(拷贝赋值或移动赋值)。上文中的addName使用的就是构造的方式，其参数newName通过拷贝构造创建了一个新的元素放在vector::push_back的尾部。文章的一开始就分析了这种情况，无论传递的是左值还是右值都会导致额外的一次move的开销。

当参数传递使用赋值的方式，这种情况相比前者要复杂的多，例如有一个表示密码的类，因为密码可能会被改变，所以这个类提供了一个setter函数changeto，这个函数通过值传递的策略，下面是这个类的实现。

```
  class Password {
   public:
    explicit Password(std::string pwd)
        : text(std::move(pwd)) {}

    void changeTo(std::string newPwd) {
        text = std::move(newPwd);
    }
    ....
   private:
    std::string text;
  }

  std::string initPwd("Beware the Jabberwock");
  Password p(initPwd);
```
上面的代码中，p.text通过构造函数进行了密码的初始化，通过之前的分析可知，这里使用的是值传递的方式，额外多了一次std::string的move构造的开销，如果使用完美转发的话可以省去这一开销。密码使用一段时间后，为了安全需要更改密码，于是使用下面这段代码进行更改：
```
std::string newPasswrd = "test password";
p.changeTo(newPassword);
```

changeTo中使用的是赋值构造，函数的参数使用的是值传递的方式，这种情况导致了程序有比较严重的性能问题。传递进来的参数newPwd是一个左值，因此当newPwd被构造的时候，std::string的拷贝构造函数会被调用，在这个构造函数中会进行内存的分配，等构造完成后内部会使用std::move进行移动赋值构造，这会导致p.text原来指向的内存被释放。整个更改密码的过程中涉及了两次内存的分配或释放。但是如果使用下面这段代码将有可能避免一次内存的分配或释放。
```
class Password {
 public:
    .....
    void changeTo(const std::string& newPwd) {
      text = newPwd;
    }
 private:
    std::string text;
}
```
上文对赋值构造的分析只适用于参数传递的是左值的情况，如果传递的是右值，那么开销还是很低的，因为不需要进行拷贝构造了，没有拷贝构造时的内存分配开销了。

总而言之，对于上文的赋值构造的分析，要考虑传递的是左值还是右值，考虑赋值的类型是什么，是否有动态内存分配，甚至还要考虑要赋值的类型的实现是什么样子的，例如: std::string，如果它使用了SSO优化进行实现，那么赋值的操作会将要赋值的内容放到SSO的buffer中，那么情况又不一样了。因此我说使用赋值的方式进行参数传递这种情况分析起来很复杂。因此通常情况最实际的做法是采用”有罪推定”的策略，也就是使用重载或者是引用的方式进行参数传递，除非在目标类型的情况下值传递被表明其开销要比前者更高效。

现如今，软件要尽可能的快，所以值传递的方式可能不再是一个可行的策略，因为避免一次廉价的move开销也是很重要的，因为我们没办法知道一个操作会有多少次这样的move操作，例如Widget::addName通过值传递造成了一次额外的move操作，但是这个函数内部又调用了Widget::validateName，这个函数也是值传递的方式，这就又造成了一次额外的move开销，validateName内部如果再调用其他的函数，并且这个函数同样是值传递的方式呢?，这就造成了累加效应，如果使用 引用传递的话就不会有这样的累加效应了。

在本文的最后我们来讨论一个与性能无关的话题，但是这个话题仍然值得我们记住。值传递不像引用传递，它容易受到切割问题的影响。如果你有一个函数被设计成接收一个基类或是任何一个派生类，你不应该将其声明为值传递的方式，因为这会造成类型切割的问题。例如下面这个例子:
```
class Widget { ... };
class SpecialWidget : public Widget { ... };
void processWidget(Widget w);
SpecialWidget sw;
processWidget(sw);  // 导致了类型切割的问题，processWidget看到的是Widget类型
```
