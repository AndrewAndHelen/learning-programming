## Item34:考虑lambda表达式而非std::bind
转载自：https://blog.csdn.net/zhangyifei216/article/details/66975444
* 引言
```
1. 与使⽤ std::bind 相⽐，Lambda更易读，更具表达⼒并且可能更⾼效。
2. 只有在C++11中， std::bind 可能对实现移动捕获或使⽤模板化函数调⽤运算符来绑定对象时会很有⽤。
```
C++11中的std::bind是对C++98中的std::bind1st 和std::bind2nd的改进和继承。而std::bind是在2005年的时候 作为标准库的一部分，起初是放在TR1规范中，拥有自己独立的命名空间std::tr1。因此很多C++程序员在很早就开始使用std::bind，所以如果要让这些C++程序员放弃对std::bind的使用，会多少有些不情愿。但是在C++11放弃对std::bind的使用是个不错的选择，因为在C++11中的lambda要比std::bind更好。而C++14对lambda进行了加强，加强后的lambda可以完全替换std::bind了。而lambda之所以要比std::bind要好主要有几个方面，第一就是可读性好，例如下面这个例子:
```
// typedef for a point in time (see Item 9 for syntax)
using Time = std::chrono::steady_clock::time_point;

// see Item 10 for "enum class"
enum class Sound { Beep, Siren, Whistle };
// typedef for a length of time

// at time t, make sound s for duration d 
void setAlarm(Time t, Sound s, Duration d);

// setSoundL ("L" for "lambda") is a function object allowing a
// sound to be specified for a 30-sec alarm to go off an hour
// after it's set
auto setSoundL =
[](Sound s) {
       // make std::chrono components available w/o qualification
       using namespace std::chrono;
      setAlarm(steady_clock::now() + hours(1), // alarm to go off s, // in an hour for 
               seconds(30)); // 30 seconds
};
```
上面的lambda代码中，可以清晰的看到lambda函数setSoundL调用了setAlarm函数，并传入了两个参数。而到了C++14后借助于字面值类型可以使得上面的lambda表达式更加清晰。
```
auto setSoundL = [](Sound s)
{
    using namespace std::chrono;
    using namespace std::literals;        // for C++14 suffixes
    setAlarm(steady_clock::now() + 1h, 30s);  // C++14, but
                                            // same meaning
                                            // as above
};
```
通过字面值类型可以将hours(1)替换成1h，seconds(30)替换成30s，使得代码更加清晰。但是如果要使用std::bind的话代码会变成如下形式:
```
using namespace std::chrono;
using namespace std::literals;
using namespace std::placeholders;    // needed for use of "_1"
auto setSoundB = std::bind(setAlarm,  // "B" for "bind"
                           steady_clock::now() + 1h, // incorrect! see below
                           _1,
                           30s);
```
首先上面的代码不易读，看不出来调用关系和传参情况，还有_1这样的magic number存在。此外上面这个std::bind还存在另外一个问题就 是steady_clock::now() + 1h的调用时机。理论上是在setAlarm被调用的时候才会执行steady_clock::now() + 1h，但是如果使用std::bind会导致先计算steady_clock::now() + 1h的值，然后保存在std::bind对象的内部，当setAlarm被调用的时候再把这个计算好的值传递过去，这就存在了时间差导致结果并不符合预期。修复这个问题的方法也很简单，只要让这个表达式延迟计算，在被调用的时候再执行即可，因此可以通过std::bind封装成一个std::function对象，然后在调用的时候执行这个function对象即可，代码如下:
```
auto setSoundB =
     std::bind(setAlarm,
     std::bind(std::plus<>(), steady_clock::now(), 1h), _1,
     30s);
```
上面的代码中std::plus是一个模版但是省略了类型，这是C++14中引入的新特性，可以忽略类型部分，但是C++11中不支持这种形式，需要显示的给出要实例化的类型如下:
```
using namespace std::chrono;                   // as above
using namespace std::placeholders;
auto setSoundB =
     std::bind(setAlarm,
              std::bind(std::plus<steady_clock::time_point>(), steady_clock::now(), hours(1)),
               seconds(30));
```
除了上文中提及到的不易读，容易造成错误等问题外，std::bind在遇到函数重载等时候也会出现问题。当上文中的setAlarm出现了重载函数，lambda版本的调用无需做任何修改就可以正常使用，但是std::bind却不行，因为std::bind并不能推断出调用的是哪个重载版本，因此必须显示的通过函数指针来指明，代码如下：
```
enum class Volume { Normal, Loud, LoudPlusPlus };
void setAlarm(Time t, Sound s, Duration d, Volume v);   // 出现重载了

using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);  // 显示指定是哪个重载版本
auto setSoundB = // now 
  std::bind(static_cast<SetAlarm3ParamType>(setAlarm), // okay
            std::bind(std::plus<>(),
                      steady_clock::now(),
                      1h), _1,
                      30s);
```
用lambda替换std::bind的第二个好处就是可以提升性能，lambda是直接调用目标函数，所以编译器可以根据目标函数的函数体大小进行有选择的内联，但是std::bind不可以，std::bind本质上是通过函数指针指向目标函数，然后在调用的函数通过函数指针来调用，所以无法被内联(因为内联是发生在编译期，但是指针的指向在运行期会被改变，因此函数指针指向的函数无法被内联)。

用lambda替换std::bind的第三个好处就是可以代码更容易被人理解，不会被误用，我们知道std::bind可以绑定参数，绑定的参数会被传递到std::bind对象的内部，以便在调用函数的时候可以传递过去，那么这些函数是如何传递到std::bind对象的内部呢？ 默认是通过传值的方式，但是从写法上来无法一眼看出，但是lambda不一样，它可以将引用的外部参数显示的指出来，表明是值传递还是引用传递，例如下面这个例子:
```
auto compressRateL = [w](CompLevel lev)
{ return compress(w, lev); };
```
上面的代码中显示的列出了w，表明这是一个需要值传递到lambda内部的变量，但是如果使用std::bind的话是看不出那些变量会进行值传递，代码如下:
```
Widget w;
using namespace std::placeholders;
auto compressRateB = std::bind(compress, w, _1);
```
综上所述，std::bind相比于lambda来说，易读性差、不易于表达、容易错误、性能差。所以C++14中没有理由是用std::bind，建议使用lambda将其替换，但是在C++11中std::bind有两个用法是合理的，需要其存在。第一个就是使用std::bind模拟lambda的移动捕获，在`C++11`中只有值捕获和引用捕获，到了`C++14`才支持移动捕获。将参数移动到std::bind对象中，然后在lambda表达式中直接调用std::bind绑定的对象即可。第二个则是实现多态，`C++11`中的lambda的参数是不能使用auto的，也就是不支持通用lambda，只能接受特定类型，但是std::bind可以不受这个限制，例如下面这段代码:
```
class PolyWidget {
 public:
    template<typename T>
    void operator()(const T& param); ...
};
PolyWidget pw;
auto boundPW = std::bind(pw, _1);   // boundPW可以接受任意类型的对象了
```
