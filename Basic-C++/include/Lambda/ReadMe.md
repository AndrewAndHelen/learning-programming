## Lambda

* 基础
```
Lambda 表达式的基本语法如下：
[捕获列表](参数列表) mutable(可选) 异常属性-> 返回类型{
// 函数体
}
```

* 1.值捕获
```
与参数传值类似，值捕获的前提是变量可以拷贝，不同之处则在于，被捕获的变量在lambda
表达式被创建时拷贝，而非调用时才拷贝：

void lambda_value_capture() {
    int value = 1;
    auto copy_value = [value] {
    	return value;
    };
    value = 100;
    auto stored_value = copy_value();
    std::cout << "stored_value = " << stored_value << std::endl;
    // 这时, stored_value == 1, 而value == 100.
    // 因为copy_value 在创建时就保存了一份value 的拷贝
}
```

* 2.引用捕获
```
与引用传参类似，引用捕获保存的是引用，值会发生变化。
void lambda_reference_capture() {
    int value = 1;
    auto copy_value = [&value] {
    	return value;
    };
    value = 100;
    auto stored_value = copy_value();
    std::cout << "stored_value = " << stored_value << std::endl;
    // 这时, stored_value == 100, value == 100.
    // 因为copy_value 保存的是引用
}
```

* 3.隐式捕获
```
手动书写捕获列表有时候是非常复杂的，这种机械性的工作可以交给编译器来处理，这时候可以在捕获列表中写一个& 或= 向编译器声明采用引用捕获或者值捕获。
总结一下，捕获提供了lambda 表达式对外部值进行使用的功能，捕获列表的最常用的四种形式可以是：
	[] 空捕获列表
	[name1, name2, . . . ] 捕获一系列变量
	[&] 引用捕获, 让编译器自行推导捕获列表
	[=] 值捕获, 让编译器执行推导引用列表
```

* 4.表达式捕获
```
上面提到的值捕获、引用捕获都是已经在外层作用域声明的变量，因此这些捕获方式捕获的均为左值，而不能捕获右值。
C++14 给与了我们方便，允许捕获的成员用任意的表达式进行初始化，这就允许了右值的捕获，被声明的捕获变量类型会根据表达式进行判断，判断方式与使用auto 本质上是相同的：

int main() {
    auto important = std::make_unique<int>(1);
    auto add = [v1 = 1, v2 = std::move(important)](int x, int y) -> int {
    	return x+y+v1+(*v2);
    };
    std::cout << add(3,4) << std::endl;
    return 0;
}

在上面的代码中，important 是一个独占指针，是不能够被捕获到的，这时候我们需要将其转移为右值，在表达式中初始化。
```

* 泛型Lambda
```
从C++14 开始，Lambda 函数的形式参数可以使用auto关键字来产生意义上的泛型：
auto add = [](auto x, auto y) {
	return x+y;
};
add(1, 2);
add(1.1, 2.2);
```