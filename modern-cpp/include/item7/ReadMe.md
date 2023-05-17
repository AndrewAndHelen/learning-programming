## Item 7:Distinguish between () and {} when creating objects
条款七:区别使⽤()和{}创建对象

* 引言
```
1.花括号初始化是最⼴泛使⽤的初始化语法，它防⽌变窄转换，并且对于C++最令⼈头疼的解析有天⽣的免疫性
2.在构造函数重载决议中，花括号初始化尽最⼤可能与std::initializer_list参数匹配，即便其他构造函数看起来是更好的选择
3.对于数值类型的std::vector来说使⽤花括号初始化和小括号初始化会造成巨⼤的不同
4.在模板类选择使⽤小括号初始化或使⽤花括号初始化创建对象是⼀个挑战。
```