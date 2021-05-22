## Item 10:优先考虑限域枚举而⾮未限域枚举

条款10:优先考虑限域枚举而⾮未限域枚举

* 引言
```
1.C++98的枚举即⾮限域枚举
2.限域枚举的枚举名仅在enum内可⻅。要转换为其它类型只能使⽤cast。
3.⾮限域/限域枚举都⽀持基础类型说明语法，限域枚举基础类型默认是int。⾮限域枚举没有默认基础类型。
4.限域枚举总是可以前置声明。⾮限域枚举仅当指定它们的基础类型时才能前置。
```

* 10.1 enum的缺点

```
1.enum没有限制枚举在其作用域内：
enum Color { black, white, red }; // black, white, red 和 Color⼀样都在相同作⽤域
auto white = false; // 错误! white早已在这个作⽤域中存在（MSVC可以通过测试，但最好不要用enum）

2.未限域枚举会发生隐式类型转换
enum Color { black, white, red }; // black, white, red
int array[10] = { 0,0 };
array[black] = 1;//等价array[0]
```

* 10.2 enum class的优点
```
1.不会导致枚举名泄漏
enum class Color { black, white, red }; // black, white, red
// 限制在Color域内
auto white = false; // 没问题，同样域内没有这个名字
Color c = white; //错误，这个域中没有white
Color c = Color::white; // 没问题
auto c = Color::white; // 也没问题（也符合条款5的建议）

2.不会发生隐式类型转换
enum class Color { black, white, red }; // black, white, red
int array[10] = { 0,0 };
array[black] = 1;//error
```
