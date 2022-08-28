## Item25: 对右值引用使用std::move，对通用引用使用std::forward
转载自：https://blog.csdn.net/zhangyifei216/article/details/53898473
* 引言
```
1. 应用std::move到右值引用，std::forward到通用引用
2. 对于右值引用和通用引用作为值从函数返回时本质上都是做了相同的事情
3. 如果希望通过编译器进行返回值优化，则不要将std::move或std::forward应用到局部对象
```

右值引用只能绑定到那些可以移动的对象上，如果你有一个右值引用的参数，你应该知道这个参数是用来接收可以移动的对象的。而一个通用引用既可以绑定到一个右值，也可以绑定到一个左值。然后std::forward则可以根据传入的值的类型进行有条件的类型转换，只有当传入的是一个右值的时候才会将其转换为右值，否则就转换为左值，而std::move则是无条件的转换为右值，这在Item23中详细解释了这两个函数的区别。而本文则将详细的讨论在何种情况下调用std::move或者std::forward。
* 对通用引用使用std::move

在前面的文章中我们已经知道了std::move可以对一个左值使用，无条件的将其转换为右值，那么std::move能否对通用引用使用呢 ? 通过试验可以验证这是可以的，但是会导致一个不太好的行为，容易导致程序出现莫名奇妙的问题，例如下面这个例子:
```
class Widget {
public:
    template<typename T>
    void setName(T&& newName) {     //通用引用版本
        name_ = std::move(newName);
    }

private:
    std::string name_;
};


Widget w;
std::string n  = "test";
w.setName(n);
std::cout << n << std::endl;
```
上面的程序在调用完setName后，n的值就变成空了，因为setName内部对std::string调用了move，导致std::string的移动构造函数被调用，使得其内部数据被移动到了Widget的成员变量name_，自己本身的数据被清空了。从字面上去理解setName的话，这个方法不应该去修改用户传入的参数。但是上面的代码很显然违背了这一思想。但是有的时候用户又想避免数据的拷贝。正确的做法应该把这个避免数据拷贝的操作交给用户去发起，让用户主动去调用std::move，因此有了下面这个比较稳妥的实现。
```
class Widget {
public:
    void setName(const std::string& newName) {  // 版本1
      name_ = newName;
    }
    void setName(std::string&& newName) {       // 版本2
      name_ = std::move(newName);
    }
 private:
    std::string name_;
};
Widget w;
std::string n  = "test";
w.setName(n);   //调用的是第一个版本的setName
std::cout << n << std::endl;    //不为空
w.setName(std::move(n));    // 调用的是第二个版本的setName
```
上面的这个版本就比较全面了，当用户不想修改传入的参数的时候就使用第一个版本即可，内部会进行数据拷贝不会修改用户传入的参数，当用户想避 免数据拷贝的时候，就主动对传入的参数调用std::move，然后调用第二个版本的setName就可以避免数据拷贝了。但是上面的代码略显臃肿，对每一个成员变量要想实现set方法都要写两个版本。对于有多个数据成员的对象来说就需要写大量的set系列重载方法。 除此之外相比于通用引用的版来说，还有另外缺点，当用户调用w.setName("test")如果是通用引用将会直接通过std::string的赋值操作赋值给内部的name_，但是对于上面的版本2来说则需要先构造一个std::string临时变量，然后经过两次移动到内部的name_变量，最后临时变量进行析构操作，其开销相比于通用引用的版本要大。无论是通用引用的版本，还是普通版本，还是右值引用版本，或许或少都有一些问题，那么到底有没有比较不错的方案呢，这就是本文即将要讨论的话题std::forward on universal references，对于通用引用应该优先使用std::forward。
* 对通用引用使用std::forward
```
class Widget {
public:
    template<typename T>
    void setName(T&& newName) {     //完美转发版本
        name_ = std::forward<T>(newName);
    }

private:
    std::string name_;
};
Widget w;
std::string n  = "test";
w.setName(n);   //n是左值，所以std::forward<T>(newName)返回的也是左值
std::cout << n << std::endl;    //不为空
w.setName(std::move(n));    // 传入的是右值，所以std::forward<T>(newName)返回的是右值
```
* 对函数返回的左值使用std::move

如果你有一个函数返回一个左值，通过对左值使用std::move可以使得代码更高效，如下:
```
Widget operator+(Widget&& lhs, const Widget& rhs) {
  lhs += rhs;
  return std::move(lhs);    
}
```
如果上面不使用std::move的话，那么在编译器不进行任何优化的前提下，lhs会首先拷贝给一个临时变量接着lhs析构，然后这个临时变量再进行拷贝构造给函数的返回值接收者，最后临时变量析构，这样的一个小小的操作居然会导致两次拷贝操作，开销如此之大，使用了std::move后，就省去了一次拷贝操作。如果在调用这个函数的地方再次借助移动操作那么又会省去一次拷贝操作。
> 注: 虽然正常情况下上面需要两次拷贝操作，效率不高，但是通常编译器会进行RVO优化，效率还是很可观的。

编译器借助于RVO的优化其效率要高于std::move，并且当要返回的对象不具备移动的语义的时候，使用std::move会导致调用拷贝构造函数，并没有节省拷贝的开销。目前的编译器还做不到对使用了std::move的对象(return std::move(lhs))进行RVO优化，IBM的Zhan Wu发表了一篇关于[std::move和RVO对比的文章](https://community.ibm.com/community/user/power/communities/community-home?CommunityKey=5d23d564-1e3e-47e6-8e47-71b8b65eedfd)
