# C++ 面试

部分转载于：https://leetcode-cn.com/leetbook/read/cpp-interview-highlights

* [1 编译内存相关](#1)
  * [1.1 C++程序编译过程](#1-1)
  * [1.2 C++内存管理](#1-2)
  * [1.3 栈和堆的区别](#1-3)
  * [1.4 变量的区别](#1-4)
  * [1.5 全局变量定义在头文件中有什么问题？](#1-5)
  * [1.6 对象创建限制在堆或栈](#1-6)
  * [1.7 内存对齐](#1-7)
  * [1.8 类的大小](#1-8)
  * [1.9 什么是内存泄漏](#1-9)
  * [1.10 如何防止内存泄漏](#1-10)
  * [1.11 智能指针的种类](#1-11)
* [2 面向对象](#2)
  * [2.1 什么是面向对象？面向对象的三大特性](#2-1)
  * [2.2 重载、重写、隐藏的区别](#2-2)
  * [2.3 如何理解 C++ 是面向对象编程](#2-3)
  * [2.4 什么是多态？多态如何实现？](#2-4)
  * [2.5 C++成员变量、构造函数的初始化顺序](#2-5)
* [3 关键字库函数](#3)
  * [3.1 sizeof和strlen的区别](#3-1)
  * [3.2 lambda表达式](#3-2)
  * [3.3 explict的作用](#3-3)
  * [3.4 static的作用](#3-4)
  * [3.5 const的作用](#3-5)
  * [3.6 define和typedef的区别](#3-6)
* 类相关
* 语言特性相关
* 设计模式

-------------------------------

## <span id ="1">1 编译内存相关</span>

### <span id = "1-1">1.1 C++程序编译过程</span>

编译过程分为四个过程：编译（编译预处理、编译、优化），汇编，链接。
* **编译预处理**：处理以 # 开头的指令；
* **编译、优化**：将源码 .cpp 文件翻译成 .s 汇编代码；
* **汇编**：将汇编代码 .s 翻译成机器指令 .o 文件；
* **链接**：汇编程序生成的目标文件，即 .o 文件，并不会立即执行，因为可能会出现：.cpp 文件中的函数引用了另一个 .cpp 文件中定义的符号或者调用了某个库文件中的函数。那链接的目的就是将这些文件对应的目标文件连接成一个整体，从而生成可执行的程序 .exe 文件。

链接分为两种：
* **静态链接**：代码从其所在的静态链接库中拷贝到最终的可执行程序中，在该程序被执行时，这些代码会被装入到该进程的虚拟地址空间中。
* **动态链接**：代码被放到动态链接库或共享对象的某个目标文件中，链接程序只是在最终的可执行程序中记录了共享对象的名字等一些信息。在程序执行时，动态链接库的全部内容会被映射到运行时相应进行的虚拟地址的空间。
二者的优缺点：

* **静态链接**：浪费空间，每个可执行程序都会有目标文件的一个副本，这样如果目标文件进行了更新操作，就需要重新进行编译链接生成可执行程序（更新困难）；优点就是执行的时候运行速度快，因为可执行程序具备了程序运行的所有内容。
* **静态链接**：节省内存、更新方便，但是动态链接是在程序运行时，每次执行都需要链接，相比静态链接会有一定的性能损失。

### <span id="1-2">1.2 C++内存管理</span>

C++ 内存分区：栈、堆、全局/静态存储区、常量存储区、代码区。

* **栈**
```
内存由编译器在需要时自动分配和释放。通常用来存储局部变量和函数参数。（为运行函数而分配的局部变量、函数参数、返回地址等存放在栈区）
栈运算分配内置于处理器的指令集中，效率很高，但是分配的内存容量有限。
```

* **堆**
```
内存使用new进行分配，使用delete或delete[]释放。如果未能对内存进行正确的释放，会造成内存泄漏。但在程序结束时，会由操作系统自动回收。
```

* **全局区/静态存储区（.bss 段和 .data 段）**
```
全局变量和静态变量被分配到同一块内存中，C语言中区分初始化和未初始化的，C++中不再区分了。（全局变量、静态数据、常量存放在全局数据区），使用静态关键字static声明，在静态存储区申请一个静态变量。
```

* **常量存储区（.data 段）**
```
存放的是常量，不允许修改，程序运行结束自动释放。
```

* **代码区（.text 段）**
```
存放代码，不允许修改，但可以执行。编译后的二进制文件存放在这里。
```

![img](./res/1-2-1.jpeg)

  说明：
从操作系统的本身来讲，以上存储区在内存中的分布是如下形式(从低地址到高地址)：.text 段 --> .data 段 --> .bss 段 --> 堆 --> unused --> 栈 --> env

* 栈内存与堆内存的区别

```
1.申请后系统的响应
栈：只要栈的剩余空间大于所申请的空间，系统将为程序提供内存，否则将报异常提示栈溢出。

堆：在记录空闲内存地址的链表中寻找一个空间大于所申请空间的堆结点，然后将该结点从空闲结点链表中删除，并将该结点的空间分配给程序。

另外，对于大多数系统会在这块内存空间的首地址出记录本次分配空间的大小，这样代码中的delete 才能正确释放本内存空间。

系统会将多余的那部分重新空闲链表中。

2.申请大小限制
栈：在Windows下,栈是向低地址扩展的数据结构，是一块连续的内存的区域。这句话的意思是栈顶的地址和栈的最大容量是系统预先规定好的，
在 WINDOWS下，栈的大小是2M（也有的说是1M，总之是一个编译时就确定的常数），如果申请的空间超过栈的剩余空间时，将提示overflow。

堆：堆是向高地址扩展的数据结构，是不连续的内存区域。这是由于系统是用链表来存储的空闲内存地址的，自然是不连续的，

而链表的遍历方向是由低地址向高地址。堆的大小受限于计算机系统中有效的虚拟内存。由此可见，堆获得的空间比较灵活，也比较大。

3.分配效率
栈：由系统自动分配，速度较快。但程序员是无法控制的。

堆：由new分配的内存，一般速度比较慢，而且容易产生内存碎片,不过用起来最方便. 另外，在WINDOWS下，最好的方式是用VirtualAlloc分配内存，不是在堆，也不是在栈是直接在进程的地址空间中保留一快内存，虽然用起来最不方便。但是速度快，也灵活 
```



程序实例：

```
#include <iostream>
using namespace std;

/*
说明：C++ 中不再区分初始化和未初始化的全局变量、静态变量的存储区，如果非要区分下述程序标注在了括号中
*/

int g_var = 0; // g_var 在全局区（.data 段）
char *gp_var;  // gp_var 在全局区（.bss 段）

int main()
{
    int var;                    // var 在栈区
    char *p_var;                // p_var 在栈区
    char arr[] = "abc";         // arr 为数组变量，存储在栈区；"abc"为字符串常量，存储在常量区
    char *p_var1 = "123456";    // p_var1 在栈区；"123456"为字符串常量，存储在常量区
    static int s_var = 0;       // s_var 为静态变量，存在静态存储区（.data 段）
    p_var = (char *)malloc(10); // 分配得来的 10 个字节的区域在堆区
    free(p_var);
    return 0;
}
```

### <span id="1-3">1.3 栈和堆的区别</span>

* 申请方式：栈是系统自动分配，堆是程序员主动申请。
* 申请后系统响应：分配栈空间，如果剩余空间大于申请空间则分配成功，否则分配失败栈溢出；申请堆空间，堆在内存中呈现的方式类似于链表（记录空闲地址空间的链表），在链表上寻找第一个大于申请空间的节点分配给程序，将该节点从链表中删除，大多数系统中该块空间的首地址存放的是本次分配空间的大小，便于释放，将该块空间上的剩余空间再次连接在空闲链表上。
* 栈在内存中是连续的一块空间（向低地址扩展）最大容量是系统预定好的，堆在内存中的空间（向高地址扩展）是不连续的。
* 申请效率：栈是有系统自动分配，申请效率高，但程序员无法控制；堆是由程序员主动申请，效率低，使用起来方便但是容易产生碎片。
  
### <span id="1-4">1.4 变量的区别</span>

**全局变量、局部变量、静态全局变量、静态局部变量的区别**
C++ 变量根据定义的位置的不同的生命周期，具有不同的作用域，作用域可分为 6 种：全局作用域，局部作用域，语句作用域，类作用域，命名空间作用域和文件作用域。

从作用域看：

* **全局变量**：具有全局作用域。全局变量只需在一个源文件中定义，就可以作用于所有的源文件。当然，其他不包含全局变量定义的源文件需要用 extern 关键字再次声明这个全局变量。
* **静态全局变量**：具有文件作用域。它与全局变量的区别在于如果程序包含多个文件的话，它作用于定义它的文件里，不能作用到其它文件里，即被 static 关键字修饰过的变量具有文件作用域。这样即使两个不同的源文件都定义了相同名字的静态全局变量，它们也是不同的变量。
* **局部变量**：具有局部作用域。它是自动对象（auto），在程序运行期间不是一直存在，而是只在函数执行期间存在，函数的一次调用执行结束后，变量被撤销，其所占用的内存也被收回。
* **静态局部变量**：具有局部作用域。它只被初始化一次，自从第一次被初始化直到程序运行结束都一直存在，它和全局变量的区别在于全局变量对所有的函数都是可见的，而静态局部变量只对定义自己的函数体始终可见。

从分配内存空间看：

* 静态存储区：全局变量，静态局部变量，静态全局变量。
* 栈：局部变量。

说明：

* 静态变量和栈变量（存储在栈中的变量）、堆变量（存储在堆中的变量）的区别：静态变量会被放在程序的静态数据存储区（.data 段）中（静态变量会自动初始化），这样可以在下一次调用的时候还可以保持原来的赋值。而栈变量或堆变量不能保证在下一次调用的时候依然保持原来的值。
* 静态变量和全局变量的区别：静态变量用 static 告知编译器，自己仅仅在变量的作用范围内可见。

### <span id="1-5">1.5 全局变量定义在头文件中有什么问题？</span>

如果在头文件中定义全局变量，当该头文件被多个文件 include 时，该头文件中的全局变量就会被定义多次，导致重复定义，因此不能再头文件中定义全局变量。

解决方法：https://www.learncpp.com/cpp-tutorial/sharing-global-constants-across-multiple-files-using-inline-variables/

### <span id="1-6">1.6 对象创建限制在堆或栈</span>

创建对象的两种方式：堆创建(使用new创建)和栈创建(一般创建)

* 限制对象只能堆创建：

1. 将析构函数设置为私有。原因：静态对象建立在栈上，是由编译器分配和释放内存空间，编译器为对象分配内存空间时，会对类的非静态函数进行检查，即编译器会检查析构函数的访问性。当析构函数设为私有时，编译器创建的对象就无法通过访问析构函数来释放对象的内存空间，因此，编译器不会在栈上为对象分配内存。
```
class A
{
public:
    A() {}
    void destory()
    {
        delete this;
    }

private:
    ~A()
    {
    }
};
```
用 new 创建的对象，通常会使用 delete 释放该对象的内存空间，但此时类的外部无法调用析构函数，因此类内必须定义一个 destory() 函数，用来释放 new 创建的对象。

无法解决继承问题，因为如果这个类作为基类，析构函数要设置成 virtual，然后在派生类中重写该函数，来实现多态。但此时，析构函数是私有的，派生类中无法访问。

2. 构造函数设置为 protected，并提供一个 public 的静态函数来完成构造，而不是在类的外部使用 new 构造；将析构函数设置为 protected。原因：类似于单例模式，也保证了在派生类中能够访问析构函数。通过调用 create() 函数在堆上创建对象。
```
class A
{
protected:
    A() {}
    ~A() {}

public:
    static A *create()
    {
        return new A();
    }
    void destory()
    {
        delete this;
    }
};
```

限制对象只能栈创建：
1. 将 operator new() 设置为私有。原因：当对象建立在堆上时，是采用 new 的方式进行建立，其底层会调用 operator new() 函数，因此只要对该函数加以限制，就能够防止对象建立在堆上。
```
class A
{
private:
    void *operator new(size_t t) {}    // 注意函数的第一个参数和返回值都是固定的
    void operator delete(void *ptr) {} // 重载了 new 就需要重载 delete
public:
    A() {}
    ~A() {}
};
```


### <span id="1-7">1.7 内存对齐</span>
内存为什么要对齐
```
尽管内存是以字节为单位，但是大部分处理器并不是按字节块来存取内存的.它一般会以双字节,四字节,8字节,16字节甚至32字节为单位来存取内存，我们将上述这些存取单位称为内存存取粒度.

现在考虑4字节存取粒度的处理器取int类型变量（32位系统），该处理器只能从地址为4的倍数的内存开始读取数据。

假如没有内存对齐机制，数据可以任意存放，现在一个int变量存放在从地址1开始的联系四个字节地址中，该处理器去取数据时，要先从0地址开始读取第一个4字节块,剔除不想要的字节（0地址）,然后从地址4开始读取下一个4字节块,同样剔除不要的数据（5，6，7地址）,最后留下的两块数据合并放入寄存器.这需要做很多工作.
```
![img](https://pic3.zhimg.com/80/v2-3f40af513a94901b36ceb5387982277e_720w.jpg)
```
现在有了内存对齐的，int类型数据只能存放在按照对齐规则的内存中，比如说0地址开始的内存。那么现在该处理器在取数据时一次性就能将数据读出来了，而且不需要做额外的操作，提高了效率。
```
![img](https://pic1.zhimg.com/80/v2-361e2d16876ce8383c9e6ea2dca34474_720w.jpg)

内存对齐的规则
1. 对于结构（或联合）的各个成员，第一个成员位于偏移为0，以后每个数据成员的偏移量必须是#pragma pack指定的数值和结构体(或联合)中最大数据成员长度 这2个数值中较小的一个的倍数。
使用伪代码表示： min(#pragma pack, 这个数据成员的自身长度) * N

2. 在数据成员完成各自对齐之后，结构(或联合)本身也要进行对齐，对齐也按照#pragma pack指定的数值和结构(或联合)最大数据成员长度这2个数值中较小的一个进行。

>*tips:如果没有使用#pragma pack指令来显式的指定内存对齐的字节数，则按照默认字节数来对齐，各个平台的默认对齐规则如下：32位CPU默认按照4字节对齐；64位CPU默认按照8字节对齐。*

#pragma pack命令

```
#pragma pack(n) // 使用自定义n字节对齐  n可以为1，2，4，8，16
#pragma pack()  // 使用缺省字节对齐
#pragma pack(show) // 在编译输出窗口以警告的形式显示出当前的内存以几个字节对齐
```

结构体内存对齐示例(64位)
```
#include <stdio.h>
#include <stdint.h>

#pragma pack(1)
typedef struct {
　　/*成员对齐*/
 　　int a;     //长度4 < 1 **按1对齐**；偏移量为0；存放位置区间[0,3]
 　　char b;    //长度1 = 1 **按1对齐**；偏移量为4；存放位置区间[4]
 　　short c;   //长度2 > 1 **按1对齐**；偏移量为5；存放位置区间[5,6]
 　　char d;    //长度1 = 1 **按1对齐**；偏移量为6；存放位置区间[7]
 　　/*整体对齐*/
 　　//整体对齐系数 = min（对齐系数1，最大成员长度4） = 1，无需对齐，整体大小为8；
}test_pack1;
#pragma pack()

 
#pragma pack(2)
typedef struct {　　 /*成员对齐*/
 　　int a;     //长度4 > 2 **按2对齐**；偏移量为0；存放位置区间[0,3]
 　　char b;    //长度1 < 2 **按1对齐**；偏移量为4；存放位置区间[4]
 　　short c;   //长度2 = 2 **按2对齐**；偏移量要提升到2的倍数6；存放位置区间[6,7]
 　　char d;    //长度1 < 2 **按1对齐**；偏移量为7；存放位置区间[8]；共九个字节
　　/*整体对齐*/
　　//整体对齐系数 = min（对齐系数2，最大成员长度4） = 2，将9提升到2的倍数10，整体大小为10；
}test_pack2;
#pragma pack()

#pragma pack(4)
typedef struct {　　 /*成员对齐*/
 　　int a;     //长度4 = 4 **按4对齐**；偏移量为0；存放位置区间[0,3]
 　　char b;    //长度1 < 4 **按1对齐**；偏移量为4；存放位置区间[4]
 　　short c;   //长度2 < 4 **按2对齐**；偏移量要提升到2的倍数6；存放位置区间[6,7]
 　　char d;    //长度1 < 4 **按1对齐**；偏移量为7；存放位置区间[8]；总大小为9
　　/*整体对齐*/
　　//整体对齐系数 = min（对齐系数4，最大成员长度4） = 4，将9提升到4的倍数12，整体大小为12；
}test_pack4;
#pragma pack()

#pragma pack(8)
typedef struct {　　 /*成员对齐*/
 　　int a;     //长度4 < 8 **按4对齐**；偏移量为0；存放位置区间[0,3]
 　　char b;    //长度1 < 8 **按1对齐**；偏移量为4；存放位置区间[4]
 　　short c;   //长度2 < 8 **按2对齐**；偏移量要提升到2的倍数6；存放位置区间[6,7]
 　　char d;    //长度1 < 8 **按1对齐**；偏移量为7；存放位置区间[8]，总大小为9
　　/*整体对齐*/
　　//整体对齐系数 = min（对齐系数8，最大成员长度4） = 4，将9提升到4的倍数12，整体大小为12；
}test_pack8;
#pragma pack()

test_pack1 pack1 = {0x11111111, 0x22, 0x3333, 0x44};
uint8_t *ptrPack1 = (uint8_t *)&pack1;
test_pack2 pack2 = {0x11111111, 0x22, 0x3333, 0x44};
uint8_t *ptrPack2 = (uint8_t *)&pack2;
test_pack4 pack4 = {0x11111111, 0x22, 0x3333, 0x44};
uint8_t *ptrPack4 = (uint8_t *)&pack4;
test_pack8 pack8 = {0x11111111, 0x22, 0x3333, 0x44};
uint8_t *ptrPack8 = (uint8_t *)&pack8;

int main(int argc, char *argv[])
{
　　int i = 0;
　　printf("#pragma pack(1) \tsize:%2d,\t", sizeof(pack1));
　　for(i=0; i<sizeof(pack1); i++)
　　{
        printf("%02x", ptrPack1[i]);
　　}
　　printf("\r\n");

　　printf("#pragma pack(2) \tsize:%2d,\t", sizeof(pack2));
　　for(i=0; i<sizeof(pack2); i++)
　　{
        printf("%02x", ptrPack2[i]);
　　}
　　printf("\r\n");

　　printf("#pragma pack(4) \tsize:%2d,\t", sizeof(pack4));
　　for(i=0; i<sizeof(pack4); i++)
　　{
        printf("%02x", ptrPack4[i]);
　　}
　　printf("\r\n");

　　printf("#pragma pack(8) \tsize:%d,\t", sizeof(pack8));
　　for(i=0; i<sizeof(pack8); i++)
　　{
        printf("%02x", ptrPack8[i]);
　　}
　　printf("\r\n"); 
　　return 0;
}
```

### <span id="1-8">1.8 类的大小</span>

首先来个总结，然后下面给出实际例子，实战！

- 空类的大小为1字节
- 一个类中，虚函数本身、成员函数（包括静态与非静态）和静态数据成员都是不占用类对象的存储空间。
- 对于包含虚函数的类，不管有多少个虚函数，只有一个虚指针,vptr的大小。
- 普通继承，派生类继承了所有基类的函数与成员，要按照字节对齐来计算大小
- 虚函数继承，不管是单继承还是多继承，都是继承了基类的vptr。(32位操作系统4字节，64位操作系统 8字节)！
- 虚继承,继承基类的vptr。

* 原则1

```
/**
 * @file blackclass.cpp
 * @brief 空类的大小为1字节
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */
#include<iostream>
using namespace std;
class A{};
int main()
{
    cout<<sizeof(A)<<endl;
    return 0;
}
```

* 原则2

```
/**
 * @file static.cpp
 * @brief 静态数据成员
 * 静态数据成员被编译器放在程序的一个global data members中，它是类的一个数据成员，但不影响类的大小。不管这个类产生了多少个实例，还是派生了多少新的类，静态数据成员只有一个实例。静态数据成员，一旦被声明，就已经存在。 
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */
#include<iostream>
using namespace std;
class A
{
    public:
        char b;
        virtual void fun() {};
        static int c;
        static int d;
        static int f;
};

int main()
{
    /**
     * @brief 16  字节对齐、静态变量不影响类的大小、vptr指针=8
     */
    cout<<sizeof(A)<<endl; 
    return 0;
}
```

* 原则3

```
/**
 * @file morevir.cpp
 * @brief 对于包含虚函数的类，不管有多少个虚函数，只有一个虚指针,vptr的大小。
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */
#include<iostream>
using namespace std;
class A{
    virtual void fun();
    virtual void fun1();
    virtual void fun2();
    virtual void fun3();
};
int main()
{
    cout<<sizeof(A)<<endl; // 8
    return 0;
}
```

* 原则4与5

```
/**
 * @file geninhe.cpp
 * @brief 1.普通单继承,继承就是基类+派生类自身的大小(注意字节对齐)
 * 注意：类的数据成员按其声明顺序加入内存，与访问权限无关，只看声明顺序。
 * 2.虚单继承，派生类继承基类vptr
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include<iostream>

using namespace std;

class A
{
    public:
        char a;
        int b;
};

/**
 * @brief 此时B按照顺序：
 * char a
 * int b
 * short a
 * long b
 * 根据字节对齐4+4=8+8+8=24
 */
class B:A
{
    public:
        short a;
        long b;
};
class C
{
    A a;
    char c;
};
class A1
{
    virtual void fun(){}
};
class C1:public A1
{
};

int main()
{
    cout<<sizeof(A)<<endl; // 8
    cout<<sizeof(B)<<endl; // 24
    cout<<sizeof(C)<<endl; // 12
    /**
     * @brief 对于虚单函数继承，派生类也继承了基类的vptr，所以是8字节
     */
    cout<<sizeof(C1)<<endl; // 8 
    return 0;
}
```

* 原则6

```
/**
 * @file virnhe.cpp
 * @brief 虚继承
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include<iostream>
using namespace std;
class A
{
    virtual void fun() {}
};
class B
{
    virtual void fun2() {}
};
class C : virtual public  A, virtual public B
{
    public:
        virtual void fun3() {}
};

int main()
{
    /**
     * @brief 8 8 16  派生类虚继承多个虚函数，会继承所有虚函数的vptr
     */
    cout<<sizeof(A)<<" "<<sizeof(B)<<" "<<sizeof(C);

    return 0;
}
```

### <span id="1-9">1.9 什么是内存泄漏</span>

**内存泄漏**：由于疏忽或错误导致的程序未能释放已经不再使用的内存。

进一步解释：

* 并非指内存从物理上消失，而是指程序在运行过程中，由于疏忽或错误而失去了对该内存的控制，从而造成了内存的浪费。

* 常指堆内存泄漏，因为堆是动态分配的，而且是用户来控制的，如果使用不当，会产生内存泄漏。

* 使用 malloc、calloc、realloc、new 等分配内存时，使用完后要调用相应的 free 或 delete 释放内存，否则这块内存就会造成内存泄漏。

* 指针重新赋值

```
char *p = (char *)malloc(10);
char *p1 = (char *)malloc(10);
p = np;
```

开始时，指针 `p` 和 `p1` 分别指向一块内存空间，但指针 `p` 被重新赋值，导致 `p` 初始时指向的那块内存空间无法找到，从而发生了内存泄漏。

### <span id="1-10">1.10 如何防止内存泄漏</span>

1. 使用智能指针：`shared_ptr` 、`unique_ptr`和`weak_ptr`。
2. 内部封装：将内存的分配和释放封装到类中，在构造的时候申请内存，析构的时候释放内存。

```
#include <iostream>
#include <cstring>

using namespace std;

class A
{
private:
    char *p;
    unsigned int p_size;

public:
    A(unsigned int n = 1) // 构造函数中分配内存空间
    {
        p = new char[n];
        p_size = n;
    };
    ~A() // 析构函数中释放内存空间
    {
        if (p != NULL)
        {
            delete[] p; // 删除字符数组
            p = NULL;   // 防止出现野指针
        }
    };
    char *GetPointer()
    {
        return p;
    };
};
void fun()
{
    A ex(100);
    char *p = ex.GetPointer();
    strcpy(p, "Test");
    cout << p << endl;
}
int main()
{
    fun();
    return 0;
}

```

* 内存泄漏检测工具->valgrind 

![](./res/1-10-1.png)

valgrind 是一套 Linux 下，开放源代码（GPL V2）的仿真调试工具的集合，包括以下工具：

* Memcheck：内存检查器（valgrind 应用最广泛的工具），能够发现开发中绝大多数内存错误的使用情况，比如：使用未初始化的内存，使用已经释放了的内存，内存访问越界等。
* Callgrind：检查程序中函数调用过程中出现的问题。
* Cachegrind：检查程序中缓存使用出现的问题。
* Helgrind：检查多线程程序中出现的竞争问题。
* Massif：检查程序中堆栈使用中出现的问题。
* Extension：可以利用 core 提供的功能，自己编写特定的内存调试工具。

Memcheck 能够检测出内存问题，关键在于其建立了两个全局表：
* Valid-Value 表：对于进程的整个地址空间中的每一个字节（byte），都有与之对应的 8 个 bits ；对于 CPU 的每个寄存器，也有一个与之对应的 bit 向量。这些 bits 负责记录该字节或者寄存器值是否具有有效的、已初始化的值。
* Valid-Address 表：对于进程整个地址空间中的每一个字节（byte），还有与之对应的 1 个 bit，负责记录该地址是否能够被读写。


检测原理：
* 当要读写内存中某个字节时，首先检查这个字节对应的 Valid-Address 表中对应的 bit。如果该 bit 显示该位置是无效位置，Memcheck 则报告读写错误。
* 内核（core）类似于一个虚拟的 CPU 环境，这样当内存中的某个字节被加载到真实的 CPU 中时，该字节在 Valid-Value 表对应的 bits 也被加载到虚拟的 CPU 环境中。一旦寄存器中的值，被用来产生内存地址，或者该值能够影响程序输出，则 Memcheck 会检查 Valid-Value 表对应的 bits，如果该值尚未初始化，则会报告使用未初始化内存错误。

### <span id="1-10">1.11 智能指针的种类</span>

C++11 中智能指针包括以下三种：

* 共享指针（shared_ptr）：资源可以被多个指针共享，使用计数机制表明资源被几个指针共享。通过 use_count() 查看资源的所有者的个数，可以通过 unique_ptr、weak_ptr 来构造，调用 release() 释放资源的所有权，计数减一，当计数减为 0 时，会自动释放内存空间，从而避免了内存泄漏。
* 独占指针（unique_ptr）：独享所有权的智能指针，资源只能被一个指针占有，该指针不能拷贝构造和赋值。但可以进行移动构造和移动赋值构造（调用 move() 函数），即一个 unique_ptr 对象赋值给另一个 unique_ptr 对象，可以通过该方法进行赋值。
* 弱指针（weak_ptr）：指向 share_ptr 指向的对象，能够解决由shared_ptr带来的循环引用问题。
>*Tips:具体请看[Effective modern C++](../Effective-Modern-C++/ReadMe.md)*


## <span id = "2">2 面向对象</span>

### <span id = "2-1">2.1 什么是面向对象？面向对象的三大特性</span>

面向对象：对象是指具体的某一个事物，这些事物的抽象就是类，类中包含数据（成员变量）和动作（成员方法）。

面向对象的三大特性：

* 封装：将具体的实现过程和数据封装成一个函数，只能通过接口进行访问，降低耦合性。
* 继承：子类继承父类的特征和行为，子类有父类的非 private 方法或成员变量，子类可以对父类的方法进行重写，增强了类之间的耦合性，但是当父类中的成员变量、成员函数或者类本身被 final 关键字修饰时，修饰的类不能继承，修饰的成员不能重写或修改。
* 多态：多态就是不同继承类的对象，对同一消息做出不同的响应，基类的指针指向或绑定到派生类的对象，使得基类指针呈现不同的表现方式。


### <span id = "2-2">2.2 重载、重写、隐藏的区别</span>

概念解释：
* 重载：是指同一可访问区内被声明几个具有不同参数列（参数的类型、个数、顺序）的同名函数，根据参数列表确定调用哪个函数，重载不关心函数返回类型。
```
class A
{
public:
    void fun(int tmp);
    void fun(float tmp);        // 重载 参数类型不同（相对于上一个函数）
    void fun(int tmp, float tmp1); // 重载 参数个数不同（相对于上一个函数）
    void fun(float tmp, int tmp1); // 重载 参数顺序不同（相对于上一个函数）
    int fun(int tmp);            // error: 'int A::fun(int)' cannot be overloaded 错误：注意重载不关心函数返回类型
};
```

* 隐藏：是指派生类的函数屏蔽了与其同名的基类函数，主要只要同名函数，不管参数列表是否相同，基类函数都会被隐藏。
```
#include <iostream>
using namespace std;

class Base
{
public:
    void fun(int tmp, float tmp1) { cout << "Base::fun(int tmp, float tmp1)" << endl; }
};

class Derive : public Base
{
public:
    void fun(int tmp) { cout << "Derive::fun(int tmp)" << endl; } // 隐藏基类中的同名函数
};

int main()
{
    Derive ex;
    ex.fun(1);       // Derive::fun(int tmp)
    ex.fun(1, 0.01); // error: candidate expects 1 argument, 2 provided
    return 0;
}

```
说明：上述代码中 `ex.fun(1, 0.01);` 出现错误，说明派生类中将基类的同名函数隐藏了。若是想调用基类中的同名函数，可以加上类型名指明 `ex.Base::fun(1, 0.01);`，这样就可以调用基类中的同名函数。

* 重写(覆盖)：是指派生类中存在重新定义的函数。函数名、参数列表、返回值类型都必须同基类中被重写的函数一致，只有函数体不同。派生类调用时会调用派生类的重写函数，不会调用被重写函数。重写的基类中被重写的函数必须有 virtual 修饰。
```
#include <iostream>
using namespace std;

class Base
{
public:
    virtual void fun(int tmp) { cout << "Base::fun(int tmp) : " << tmp << endl; }
};

class Derived : public Base
{
public:
    virtual void fun(int tmp) { cout << "Derived::fun(int tmp) : " << tmp << endl; } // 重写基类中的 fun 函数
};
int main()
{
    Base *p = new Derived();
    p->fun(3); // Derived::fun(int) : 3
    return 0;
}

```
重写和重载的区别：

* 范围区别：对于类中函数的重载或者重写而言，重载发生在同一个类的内部，重写发生在不同的类之间（子类和父类之间）。
* 参数区别：重载的函数需要与原函数有相同的函数名、不同的参数列表，不关注函数的返回值类型；重写的函数的函数名、参数列表和返回值类型都需要和原函数相同，父类中被重写的函数需要有 virtual 修饰。
* virtual 关键字：重写的函数基类中必须有 virtual关键字的修饰，重载的函数可以有 virtual 关键字的修饰也可以没有。

隐藏和重写，重载的区别：

* 范围区别：隐藏与重载范围不同，隐藏发生在不同类中。
* 参数区别：隐藏函数和被隐藏函数参数列表可以相同，也可以不同，但函数名一定相同；当参数不同时，无论基类中的函数是否被 virtual 修饰，基类函数都是被隐藏，而不是重写。


### <span id = "2-3">2.3 如何理解 C++ 是面向对象编程</span>
* 面向过程编程：一种以执行程序操作的过程或函数为中心编写软件的方法。程序的数据通常存储在变量中，与这些过程是分开的。所以必须将变量传递给需要使用它们的函数。缺点：随着程序变得越来越复杂，程序数据与运行代码的分离可能会导致问题。例如，程序的规范经常会发生变化，从而需要更改数据的格式或数据结构的设计。当数据结构发生变化时，对数据进行操作的代码也必须更改为接受新的格式。查找需要更改的所有代码会为程序员带来额外的工作，并增加了使代码出现错误的机会。

* 面向对象编程（Object-Oriented Programming, OOP）：以创建和使用对象为中心。一个对象（Object）就是一个软件实体，它将数据和程序在一个单元中组合起来。对象的数据项，也称为其属性，存储在成员变量中。对象执行的过程被称为其成员函数。将对象的数据和过程绑定在一起则被称为封装。

* 面向对象编程进一步说明：
面向对象编程将数据成员和成员函数封装到一个类中，并声明数据成员和成员函数的访问级别（public、private、protected），以便控制类对象对数据成员和函数的访问，对数据成员起到一定的保护作用。而且在类的对象调用成员函数时，只需知道成员函数的名、参数列表以及返回值类型即可，无需了解其函数的实现原理。当类内部的数据成员或者成员函数发生改变时，不影响类外部的代码。


### <span id = "2-4">2.4 什么是多态？多态如何实现？</span>
[转载](https://blog.csdn.net/qq_40840459/article/details/80195158?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522163810478316780264046504%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fblog.%2522%257D&request_id=163810478316780264046504&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~first_rank_v2~hot_rank-1-80195158.pc_v2_rank_blog_default&utm_term=c%2B%2B%E5%A4%9A%E6%80%81%E7%9A%84%E5%AE%9E%E7%8E%B0%E6%96%B9%E5%BC%8F&spm=1018.2226.3001.4450)
多态就是多种形态，C++的多态分为静态多态与动态多态。

* 静态多态就是重载，因为在编译期决议确定，所以称为静态多态。在编译时就可以确定函数地址。
* 动态多态就是通过继承重写基类的虚函数实现的多态，因为实在运行时决议确定，所以称为动态多态。运行时在虚函数表中寻找调用函数的地址。

```
class Person   
{   
public :       
    virtual void BuyTickets()      
    {           
    cout<<" 买票"<< endl;     
    }  
protected :      
    string _name ;   // 姓名   
};  
  
class Student : public Person   
{   
public :  
     virtual void BuyTickets()  
     {           
     cout<<" 买票-半价 "<<endl ;  
     }  
protected :  
     int _num ;   //学号  
};  
  
//void Fun(Person* p)   
void Fun (Person& p)  
{  
     p.BuyTickets ();  
}  
  
void Test ()  
 {  
     Person p ;  
     Student s ;  
     Fun(p );  //cout<<" 买票"<< endl;   
     Fun(s );  //cout<<" 买票-半价 "<<endl ; 
}  
```

* 多态的实现原理
>一个接口，多种方法
1. 用virtual关键字申明的函数叫做虚函数，虚函数肯定是类的成员函数。
2. 存在虚函数的类都有一个一维的虚函数表叫做虚表。当类中声明虚函数时，编译器会在类中生成一个虚函数表。

3. 类的对象有一个指向虚表开始的虚指针。虚表是和类对应的，虚表指针是和对象对应的。

4. 虚函数表是一个存储类成员函数指针的数据结构。

5. 虚函数表是由编译器自动生成与维护的。

6. virtual成员函数会被编译器放入虚函数表中。

7. 当存在虚函数时，每个对象中都有一个指向虚函数的指针（C++编译器给父类对象，子类对象提前布局vptr指针），当进行test(parent *base)函数的时候，C++编译器不需要区分子类或者父类对象，只需要再base指针中，找到vptr指针即可）。

8. vptr一般作为类对象的第一个成员。

* 探索虚表
```
class Base
 {
 public :
     virtual void func1()
    {}
     virtual void func2()
    {}
 private :
     int a ; 
};
 
void Test1 ()
 {
     Base b1;
 }
```
![](./res/2-4-1.png)

* 单继承模型

```
class Base
{
public :
     virtual void func1()
	 {
		 cout<<"Base::func1" <<endl;
	 }
     virtual void func2()
	 {
		 cout<<"Base::func2" <<endl;
	 } 
private :
     int a ;
};
 
class Derive :public Base
 { 
 public :
	virtual void func1()
    {
		cout<<"Derive::func1" <<endl;
	}
     virtual void func3()
	{ 
	cout<<"Derive::func3" <<endl;
	}
    virtual void func4()
	{
		 cout<<"Derive::func4" <<endl;
	}
private :
     int b ;
};
 
typedef void (* FUNC) ();
void PrintVTable (int* VTable)
 {
     cout<<" 虚表地址>"<< VTable<<endl ;
	 
     for (int i = 0; VTable[i ] != 0; ++i)
		{
			 printf(" 第%d个虚函数地址 :0X%x,->", i , VTable[i ]);
			 FUNC f = (FUNC) VTable[i ];
			 f();
		}
     cout<<endl ;
}
 
void Test1 ()
 {
     Base b1 ;
     Derive d1 ;
     int* VTable1 = (int*)(*( int*)&b1 );
     int* VTable2 = (int*)(*( int*)&d1 );
     PrintVTable(VTable1 );
     PrintVTable(VTable2 );
}
```
可以看到派生类Derive::func1重写基类Base::func1，覆盖了相应虚表位置上的函数。 ps:可以看到这里没有看到派生类Derive中的func3和func4，这两个函数就放在func2的后面，这里没有显示是VS的问题（bug）。
![](./res/2-4-2.png)

* 多继承模型
```
class Base1
 {
 public :
     virtual void func1()
    {
          cout<<"Base1::func1" <<endl;
    }
     virtual void func2()
    { 
         cout<<"Base1::func2" <<endl;
    }
private :
     int b1 ;
 };
class Base2
 { 
public :
     virtual void func1()
    {
          cout<<"Base2::func1" <<endl;
    }
     virtual void func2()
    {
         cout<<"Base2::func2" <<endl;
   }
private :
     int b2 ;
 };
class Derive : public Base1, public Base2
 { 
public :
virtual void func1()
    { 
         cout<<"Derive::func1" <<endl;
    }
     virtual void func3()
   { 
         cout<<"Derive::func3" <<endl;
    }
private :
     int d1 ; 
};
 
typedef void (* FUNC) ();
void PrintVTable (int* VTable)
 {
     cout<<" 虚表地址>"<< VTable<<endl ;
 
     for (int i = 0; VTable[i ] != 0; ++i)
    { 
         printf(" 第%d个虚函数地址 :0X%x,->", i , VTable[i ]); 
         FUNC f = (FUNC) VTable[i ];
          f();
    }
    cout<<endl ;
 }
 
void Test1 ()
 { 
    Derive d1 ;
    int* VTable = (int*)(*( int*)&d1 );
    PrintVTable(VTable );
     // Base2虚函数表在对象Base1后面    
    VTable = (int *)(*((int*)&d1 + sizeof (Base1)/4));
     PrintVTable(VTable );
 }
```
![](./res/2-4-3.png)

* 考题
为什么调用普通函数比调用虚函数的效率高？
>因为普通函数是静态联编的，而调用虚函数是动态联编的。
>联编的作用：程序调用函数，编译器决定使用哪个可执行代码块。

* 静态联编 ：在编译的时候就确定了函数的地址，然后call就调用了。
* 动态联编 ： 首先需要取到对象的首地址，然后再解引用取到虚函数表的首地址后，再加上偏移量才能找到要调的虚函数，然后call调用。

为什么要用虚函数表（存函数指针的数组）？
>实现多态，父类对象的指针指向父类对象调用的是父类的虚函数，指向子类调用的是子类的虚函数。
>同一个类的多个对象的虚函数表是同一个，所以这样就可以节省空间，一个类自己的虚函数和继承的虚函数还有重写父类的虚函数都会存在自己的虚函数表。

为什么要把基类的析构函数定义为虚函数？

>在用基类操作派生类时，为了防止执行基类的析构函数，不执行派生类的析构函数。因为这样的删除只能够删除基类对象, 而不能删除子类对象, 形成了删除一半形象, 会造成内存泄漏.如下代码：

```
#include<iostream>  
using namespace std;  
  
class Base  
{  
public:  
    Base() {};  
    ~Base()   
    {  
        cout << "delete Base" << endl;  
    };  
};  
  
class Derived : public Base  
{  
public:  
    Derived() {};  
    ~Derived()  
    {  
        cout << "delete Derived" << endl;  
  
    };  
};  
int main()  
{  
    //操作1  
    Base* p1 = new Derived;  
    delete p1;  
    //因为这里子类的析构函数重写了父类的析构函数，虽然子类和父类的析构函数名不一样，  
    //但是编译器对析构函数做了特殊的处理，在内部子类和父类的析构函数名是一样的。  
    //所以如果不把父类的析构函数定义成虚函数，就不构成多态，由于父类的析构函数隐藏了子类  
    //的析构函数，所以只能调到父类的析构函数。  
    //但是若把父类的析构函数定义成虚函数，那么调用时就会直接调用子类的析构函数，  
    //由于子类析构先要去析构父类，在析构子类，这样就把子类和继承的父类都析构了  
  
    system("pause");  
}  
```

### <span id="2-5">2.5 C++成员变量、构造函数的初始化顺序</span>

1. 成员变量在使用初始化列表初始化时，与构造函数中初始化成员列表的顺序无关，只与成员变量声明的顺序有关。

2. 如果不使用初始化列表初始化，在构造函数内“初始化”时，注意在构造函数运行之前，所有成员变量均进行了默认初始化，在构造函数内的操作均为成员变量`赋值操作`。

3. 类内const成员变量必须在构造函数初始化列表中初始化。（C++11以后支持类内初始化）

4. 类内static成员变量，只能在类外初始化(同一类所有实例共享静态成员变量)，静态类型成员变量的初始化顺序和声明的顺序不一致，只和初始化语句的先后顺序有关。

初始化顺序：
* 1）基类静态变量或全局变量
* 2）派生类静态变量或全局变量
* 3）基类成员变量
* 4）派生类成员变量

```
class A
{
private:
	int n1;
	int n2;
	
public:
	A():n2(0),n1(n2+2){}
 
	void Print(){
		cout << "n1:" << n1 << ", n2: " << n2 <<endl;  
	}
};
 
int main()
{
 
	A a;
	a.Print();
 	//n1:-858993458, n2: 0
	return 0;
}
```
> 在初始化列表中，首先初始化n1，但n2的值未初始化，导致n1值不符合预期。





## <span id="3">关键字库函数</span>

### <span id="3-1">3.1 sizeof和strlen的区别</span>

1. `strlen` 是头文件 <cstring> 中的函数，`sizeof` 是 C++ 中的运算符。

2. `strlen` 测量的是字符串的实际长度（其源代码如下），以 `\0` 结束。而 `sizeof` 测量的是字符数组的分配大小。
   `strlen` 源代码:

   ```
   size_t strlen(const char *str) {
       size_t length = 0;
       while (*str++)
           ++length;
       return length;
   }
   ```

   ```
   #include <iostream>
   #include <cstring>
   
   using namespace std;
   
   int main()
   {
       char arr[10] = "hello";
       cout << strlen(arr) << endl; // 5
       cout << sizeof(arr) << endl; // 10
       return 0;
   }
   

3. 若字符数组 arr 作为函数的形参，`sizeof(arr)` 中 arr 被当作字符指针来处理，`strlen(arr)` 中 arr 依然是字符数组，从下述程序的运行结果中就可以看出。

   ```
   #include <iostream>
   #include <cstring>
   
   using namespace std;
   
   void size_of(char arr[])
   {
       cout << sizeof(arr) << endl; // warning: 'sizeof' on array function parameter 'arr' will return size of 'char*' .
       cout << strlen(arr) << endl; 
   }
   
   int main()
   {
       char arr[20] = "hello";
       size_of(arr); 
       return 0;
   }
   /*
   输出结果：
   20
   5
   */



### <span id="3-2">3.2 lambda表达式</span>

lambda 表达式是一个可调度的代码单元，可以视为一个未命名的内部函数。
lambda 函数是一个函数对象，编译器在编译时会生成一个 lambda 对象的类，然后再生成一个该命令未命名的对象。

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

* return type
```
与普通函数不同的是: lambda 必须使用位尾置返回来指定返回类型。
如果忽略返回类型，lambda 表达式会根据函数体中的代码推断出返回类型。
若函数体只有一个 return 语句， 则返回类型从返回表达式的类型推断而来，否则，若未指定返回类型，返回类型为 void。
```

### <span id="3-3">3.3 explict的作用</span>

explicit关键字作用于单个参数的构造函数。

```

class test{
public:
    explicit test(int a); //单个参数
    explicit test(int a, int b = 5); //只有一个参数待确定
};
```

防止隐式类型转换

```
test ctx = 5;//编译错误
```

禁止隐式的拷贝构造

```
explicit test(test&);

test a(2);
test b = a; //相当于 test b = test(a) 隐式拷贝构造 引发编译错误
```

### <span id="3-4">3.4 static的作用</span>

1. static修饰的常规变量，从生命周期和作用域进行分析：

* 生命周期： 从定义该变量该开始，直到程序结束时
* 作用域：
	* 若是局部变量，则作用域就是定义该变量的函数块范围
	* 若是全局变量，则是定义该变量的文件范围内部，也即是 static 修饰的变量具有文件作用域
```
static 定义变量的位置在静态变量区，超过其作用域该变量并不被释放，而是在函数结束时释放
static 修饰的变量只会被初始化一次
```
```
#include <iostream> 
#include <string> 
using namespace std;

void demo()
{
	static int count = 0;
	cout << count << " ";
	count++;
}

int main()
{
	for (int i = 0; i < 5; i++)
		demo();
	//0 1 2 3 4
	return 0;
}
```

2. static 修饰类：

* static 修饰的成员变量要在类外初始化，属于类，为所有类对象共享，static 修饰的变量不占类的空间
* static 修饰的函数，静态成员函数， 属于类，为类的所有对象共享， 不能访问类的非静态成员，和外部函数， 没有this指针，因此只能访问静态成员(静态成员变量和静态函数)

2.1 类对象为静态

就像变量一样，对象也在声明为static时具有范围，直到程序的生命周期。

考虑以下程序，其中对象是非静态的。

```
#include<iostream> 
using namespace std; 

class Apple 
{ 
	int i; 
	public: 
		Apple() 
		{ 
			i = 0; 
			cout << "Inside Constructor\n"; 
		} 
		~Apple() 
		{ 
			cout << "Inside Destructor\n"; 
		} 
}; 

int main() 
{ 
	int x = 0; 
	if (x==0) 
	{ 
		Apple obj; 
	} 
	cout << "End of main\n"; 
} 
```

输出：

```
Inside Constructor
Inside Destructor
End of main
```

在上面的程序中，对象在if块内声明为非静态。因此，变量的范围仅在if块内。因此，当创建对象时，将调用构造函数，并且在if块的控制权越过析构函数的同时调用，因为对象的范围仅在声明它的if块内。 如果我们将对象声明为静态，现在让我们看看输出的变化。

```
#include<iostream> 
using namespace std; 

class Apple 
{ 
	int i; 
	public: 
		Apple() 
		{ 
			i = 0; 
			cout << "Inside Constructor\n"; 
		} 
		~Apple() 
		{ 
			cout << "Inside Destructor\n"; 
		} 
}; 

int main() 
{ 
	int x = 0; 
	if (x==0) 
	{ 
		static Apple obj; 
	} 
	cout << "End of main\n"; 
} 
```

输出：

```
Inside Constructor
End of main
Inside Destructor
```

您可以清楚地看到输出的变化。现在，在main结束后调用析构函数。这是因为静态对象的范围是贯穿程序的生命周期。

2.2 类中的静态函数

态成员函数。但建议使用类名和范围解析运算符调用静态成员。

允许静态成员函数仅访问静态数据成员或其他静态成员函数，它们无法访问类的非静态数据成员或成员函数。

```
#include<iostream> 
using namespace std; 

class Apple 
{ 
    public: 
        // static member function 
        static void printMsg() 
        {
            cout<<"Welcome to Apple!"; 
        }
}; 

// main function 
int main() 
{ 
    // invoking a static member function 
    Apple::printMsg(); 
} 
```

输出：

```
Welcome to Apple!
```

### <span id="3-5">3.5 const的作用</span>

#### 1.const含义

常类型是指使用类型修饰符**const**说明的类型，常类型的变量或对象的值是不能被更新的。

#### 2.const作用

- 可以定义常量

- define 和 const 的区别

1. 编译阶段：`define` 是在编译预处理阶段进行替换，`const` 是在编译阶段确定其值。
2. 安全性：`define` 定义的宏常量没有数据类型，只是进行简单的替换，不会进行类型安全的检查；`const` 定义的常量是有类型的，是要进行判断的，可以避免一些低级的错误。
3. 内存占用：`define` 定义的宏常量，在程序中使用多少次就会进行多少次替换，内存中有多个备份，占用的是代码段的空间；`const `定义的常量占用静态存储区的空间，程序运行过程中只有一份。
4. 调试：`define` 定义的宏常量不能调试，因为在预编译阶段就已经进行替换了；`const` 定义的常量可以进行调试。

- const的优点

1. 有数据类型，在定义式可进行安全性检查。
2. 可调式。
3. 占用较少的空间。

#### 3.const对象默认为文件局部变量

注意：非const变量默认为extern。要使const变量能够在其他文件中访问，必须在文件中显式地指定它为extern。

> 未被const修饰的变量在不同文件的访问

```
// file1.cpp
int ext
// file2.cpp
#include<iostream>

extern int ext;
int main(){
    std::cout<<(ext+10)<<std::endl;
}
```

> const常量在不同文件的访问

```
//extern_file1.cpp
extern const int ext=12;
//extern_file2.cpp
#include<iostream>
extern const int ext;
int main(){
    std::cout<<ext<<std::endl;
}
```

> 小结：
> 可以发现未被const修饰的变量不需要extern显式声明！而const常量需要显式声明extern，并且需要做初始化！因为常量在定义后就不能被修改，所以定义时必须初始化。

#### 4.定义常量

```
const int b = 10;
b = 0; // error: assignment of read-only variable ‘b’
const string s = "helloworld";
const int i,j=0 // error: uninitialized const ‘i’
```

上述有两个错误：

- b 为常量，不可更改！
- i 为常量，必须进行初始化！(因为常量在定义后就不能被修改，所以定义时必须初始化。)

#### 5.指针与const

与指针相关的const有四种：

```
const char * a; //指向const对象的指针或者说指向常量的指针。
char const * a; //同上
char * const a; //指向类型对象的const指针。或者说常指针、const指针。
const char * const a; //指向const对象的const指针。
```

> **小结：**
> 如果*const*位于`*`的左侧，则const就是用来修饰指针所指向的变量，即指针指向为常量；
> 如果const位于`*`的右侧，*const*就是修饰指针本身，即指针本身是常量。

具体使用如下：

（1） **指向常量的指针**

```
const int *ptr;
*ptr = 10; //error
```

ptr是一个指向int类型const对象的指针，const定义的是int类型，也就是ptr所指向的对象类型，而不是ptr本身，所以ptr可以不用赋初始值。但是不能通过ptr去修改所指对象的值。

除此之外，也不能使用void`*`指针保存const对象的地址，必须使用const void`*`类型的指针保存const对象的地址。

```
const int p = 10;
const void * vp = &p;
void *vp = &p; //error
```

另外一个重点是：**允许把非const对象的地址赋给指向const对象的指针**。

将非const对象的地址赋给const对象的指针:

```
const int *ptr;
int val = 3;
ptr = &val; //ok
```

我们不能通过ptr指针来修改val的值，即使它指向的是非const对象!

我们不能使用指向const对象的指针修改基础对象，然而如果该指针指向了非const对象，可用其他方式修改其所指的对象。可以修改const指针所指向的值的，但是不能通过const对象指针来进行而已！如下修改：

```
int *ptr1 = &val;
*ptr1=4;
cout<<*ptr<<endl;
```

> 小结：
> 1.对于指向常量的指针，不能通过指针来修改对象的值。
> 2.不能使用void`*`指针保存const对象的地址，必须使用const void`*`类型的指针保存const对象的地址。
> 3.允许把非const对象的地址赋值给const对象的指针，如果要修改指针所指向的对象值，必须通过其他方式修改，不能直接通过当前指针直接修改。

（2） **常指针**

const指针必须进行初始化，且const指针的值不能修改。

```
#include<iostream>
using namespace std;
int main(){

    int num=0;
    int * const ptr=&num; //const指针必须初始化！且const指针的值不能修改
    int * t = &num;
    *t = 1;
    cout<<*ptr<<endl;
}
```

上述修改ptr指针所指向的值，可以通过非const指针来修改。

最后，当把一个const常量的地址赋值给ptr时候，由于ptr指向的是一个变量，而不是const常量，所以会报错，出现：const int`*` -> int `*`错误！

```
#include<iostream>
using namespace std;
int main(){
    const int num=0;
    int * const ptr=&num; //error! const int* -> int*
    cout<<*ptr<<endl;
}
```

上述若改为 const int `*`ptr或者改为const int `*`const ptr，都可以正常！

（3）**指向常量的常指针**

理解完前两种情况，下面这个情况就比较好理解了：

```
const int p = 3;
const int * const ptr = &p; 
```

ptr是一个const指针，然后指向了一个int 类型的const对象。

#### 6.函数中使用const

> const修饰函数返回值

这个跟const修饰普通变量以及指针的含义基本相同：

（1）**const int**

```
const int func1();
```

这个本身无意义，因为参数返回本身就是赋值给其他的变量！

（2）**const int***

```
const int* func2();
```

指针指向的内容不变。

（3）int *const

```
int *const func2();
```

指针本身不可变。

> const修饰函数参数

（1）传递过来的参数及指针本身在函数内不可变，无意义！

```
void func(const int var); // 传递过来的参数不可变
void func(int *const var); // 指针本身不可变
```

表明参数在函数体内不能被修改，但此处没有任何意义，var本身就是形参，在函数内不会改变。包括传入的形参是指针也是一样。

输入参数采用“值传递”，由于函数将自动产生临时变量用于复制该参数，该输入参数本来就无需保护，所以不要加const 修饰。

（2）**参数指针所指内容为常量不可变**

```
void StringCopy(char *dst, const char *src);
```

其中src 是输入参数，dst 是输出参数。给src加上const修饰后，如果函数体内的语句试图改动src的内容，编译器将指出错误。这就是加了const的作用之一。

（3）**参数为引用，为了增加效率同时防止修改。**

```
void func(const A &a)
```

对于非内部数据类型的参数而言，象void func(A a) 这样声明的函数注定效率比较低。因为函数体内将产生A 类型的临时对象用于复制参数a，而临时对象的构造、复制、析构过程都将消耗时间。

为了提高效率，可以将函数声明改为void func(A &a)，因为“引用传递”仅借用一下参数的别名而已，不需要产生临 时对象。

> 但是函数void func(A &a) 存在一个缺点：
>
> “引用传递”有可能改变参数a，这是我们不期望的。解决这个问题很容易，加const修饰即可，因此函数最终成为 void func(const A &a)。

以此类推，是否应将void func(int x) 改写为void func(const int &x)，以便提高效率？完全没有必要，因为内部数

据类型的参数不存在构造、析构的过程，而复制也非常快，“值传递”和“引用传递”的效率几乎相当。

> 小结：
> 1.对于非内部数据类型的输入参数，应该将“值传递”的方式改为“const 引用传递”，目的是提高效率。例如将void func(A a) 改为void func(const A &a)。
>
> 2.对于内部数据类型的输入参数，不要将“值传递”的方式改为“const 引用传递”。否则既达不到提高效率的目的，又降低了函数的可理解性。例如void func(int x) 不应该改为void func(const int &x)。

以上解决了两个面试问题：

- 如果函数需要传入一个指针，是否需要为该指针加上const，把const加在指针不同的位置有什么区别；
- 如果写的函数需要传入的参数是一个复杂类型的实例，传入值参数或者引用参数有什么区别，什么时候需要为传入的引用参数加上const。

#### 7.类中使用const

在一个类中，任何不会修改数据成员的函数都应该声明为const类型。如果在编写const成员函数时，不慎修改 数据成员，或者调用了其它非const成员函数，编译器将指出错误，这无疑会提高程序的健壮性。

使用const关键字进行说明的成员函数，称为常成员函数。只有常成员函数才有资格操作常量或常对象，没有使用const关键字进行说明的成员函数不能用来操作常对象。

对于类中的const成员变量必须通过初始化列表进行初始化，如下所示：

```
class Apple{
private:
    int people[100];
public:
    Apple(int i); 
    const int apple_number;
};

Apple::Apple(int i):apple_number(i)
{

}
```

const对象只能访问const成员函数,而非const对象可以访问任意的成员函数,包括const成员函数.

例如：

```
//apple.cpp
class Apple
{
private:
    int people[100];
public:
    Apple(int i); 
    const int apple_number;
    void take(int num) const;
    int add(int num);
	int add(int num) const;
    int getCount() const;

};
//main.cpp
#include<iostream>
#include"apple.cpp"
using namespace std;

Apple::Apple(int i):apple_number(i)
{

}
int Apple::add(int num){
    take(num);
}
int Apple::add(int num) const{
    take(num);
}
void Apple::take(int num) const
{
    cout<<"take func "<<num<<endl;
}
int Apple::getCount() const
{
    take(1);
//    add(); //error
    return apple_number;
}
int main(){
    Apple a(2);
    cout<<a.getCount()<<endl;
    a.add(10);
    const Apple b(3);
    b.add(100);
    return 0;
}
```

> 编译： g++ -o main main.cpp apple.cpp

结果：

```
take func 1
2
take func 10
take func 100
```

上面getCount()方法中调用了一个add方法，而add方法并非const修饰，所以运行报错。也就是说const对象只能访问const成员函数。

而add方法又调用了const修饰的take方法，证明了非const对象可以访问任意的成员函数,包括const成员函数。

除此之外，我们也看到add的一个重载函数，也输出了两个结果，说明const对象默认调用const成员函数。

我们除了上述的初始化const常量用初始化列表方式外，也可以通过下面方法：

第一：将常量定义与static结合，也就是：

```
static const int apple_number
```

第二：在外面初始化：

```
const int Apple::apple_number=10;
```

当然，如果你使用c++11进行编译，直接可以在定义出初始化，可以直接写成：

```
static const int apple_number=10;
// 或者
const int apple_number=10;
```

这两种都在c++11中支持！

编译的时候加上`-std=c++11`即可！

这里提到了static，下面简单的说一下：

在C++中，static静态成员变量不能在类的内部初始化。在类的内部只是声明，定义必须在类定义体的外部，通常在类的实现文件中初始化。

在类中声明：

```
static int ap;
```

在类实现文件中使用：

```
int Apple::ap=666
```

对于此项，c++11不能进行声明并初始化，也就是上述使用方法。



### <span id="3-6">3.6 define和typedef的区别</span>

* 原理：`#define` 作为预处理指令，在编译预处理时进行替换操作，不作正确性检查，只有在编译已被展开的源程序时才会发现可能的错误并报错。`typedef` 是关键字，在编译时处理，有类型检查功能，用来给一个已经存在的类型一个别名，但不能在一个函数定义里面使用 `typedef `。

* 功能：`typedef` 用来定义类型的别名，方便使用。`#define` 不仅可以为类型取别名，还可以定义常量、变量、编译开关等。

* 作用域：`#define` 没有作用域的限制，只要是之前预定义过的宏，在以后的程序中都可以使用，而 `typedef `有自己的作用域。

* 指针的操作：`typedef` 和`#define` 在处理指针时不完全一样

```
#include <iostream>
#define INTPTR1 int *
typedef int * INTPTR2;

using namespace std;

int main()
{
    INTPTR1 p1, p2; // p1: int *; p2: int
    INTPTR2 p3, p4; // p3: int *; p4: int *

    int var = 1;
    const INTPTR1 p5 = &var; // 相当于 const int * p5; 常量指针，即不可以通过 p5 去修改 p5 指向的内容，但是 p5 可以指向其他内容。
    const INTPTR2 p6 = &var; // 相当于 int * const p6; 指针常量，不可使 p6 再指向其他内容。
    
    return 0;
}
```

