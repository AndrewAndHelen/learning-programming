## functional

* std::functional
```
C++11 std::function 是一种通用、多态的函数封装，它的实例可以对任何可以调用的目标实体进
行存储、复制和调用操作，它也是对C++ 中现有的可调用实体的一种类型安全的包裹（相对来说，函数
指针的调用不是类型安全的），换句话说，就是函数的容器。当我们有了函数的容器之后便能够更加方便
的将函数、函数指针作为对象进行处理。例如：

#include <functional>
#include <iostream>
int foo(int para) {
	return para;
}
int main() {
    // std::function 包装了一个返回值为int, 参数为int 的函数
    std::function<int(int)> func = foo;
    int important = 10;
    std::function<int(int)> func2 = [&](int value) -> int {
    return 1+value+important;
    };
    std::cout << func(10) << std::endl;
    std::cout << func2(10) << std::endl;
}
```

* std::bind 和std::placeholder

```
而std::bind 则是用来绑定函数调用的参数的，它解决的需求是我们有时候可能并不一定能够一次
性获得调用某个函数的全部参数，通过这个函数，我们可以将部分调用参数提前绑定到函数身上成为一
个新的对象，然后在参数齐全后，完成调用。例如：

int foo(int a, int b, int c) {
...
}

int main() {
// 将参数1,2 绑定到函数foo 上，但是使用std::placeholders::_1 来对第一个参数进行占位
auto bindFoo = std::bind(foo, std::placeholders::_1, 1,2);
// 这时调用bindFoo 时，只需要提供第一个参数即可
bindFoo(1);
}

提示：注意auto 关键字的妙用。有时候我们可能不太熟悉一个函数的返回值类型，但是我们
却可以通过auto 的使用来规避这一问题的出现。
```

