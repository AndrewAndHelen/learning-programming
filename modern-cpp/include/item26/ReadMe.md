## Item26: 避免在通用引用上重载
转载自：https://blog.csdn.net/zhangyifei216/article/details/53899914
* 引言
```
1. 在通用引用上进行重载，几乎总是会导致通用引用重载的版本比预期被调用的次数更频繁。
2. 完美转发构造函数是非常有问题的，因为它们通常比非const左值的拷贝构造函数更好地匹配，并且可以劫持派生类对基类拷贝构造和移动构造函数的调用。
```
在正式进入本文主题之前先来看一段代码，这段代码的目的是记录下当前的日期和时间，并记录一下倒全局的数据结构中，代码如下:
```
std::multiset<std::string> names;
void logAndAdd(const std::string& name) {

  auto now = std::chrono::system_lock::now();
  log(now, "logAndAdd");
  names.emplace(name); // C++11提供的方法，对于左值是拷贝构造，对于右值是移动
}
```
面的代码，尽管可以正常work，但是效率并不高，考虑下面三种调用场景。
```
std::string petName("Darla");
logAndAdd(petName);     // 第一种
logAndAdd(std::string("Persephone"));   //第二种
logAndAdd("patty Dog"); // 第三种
```
第一种方式会导致在调用names.emplace的时候产生了一次拷贝操作。第二种方式首先会有一次临时对象的创建，然后在调用names.emplace的时候仍然会有一次拷贝操作，因为传入的name总是一个左值。而第三种方式首先会隐式的构造一个临时的 std::string，然后在调用names.emplace时进行一次拷贝。可见上面三种调用方式总是会带来拷贝的开销。通过对item25和Item24的学习可以借助于通用引用来重写上面的logAndAdd方法让它更加高效，重写后的代码如下:
```
template<typename T>
void logAndAdd(T&& name) {
  auto now = std::chrono::system_clock::now();
  log(now, "logAndAdd");
  names.emplace(std::forward<T>(name));
}
```
上面的代码可以在传入的是右值的情况下，避免names.emplace导致的一次拷贝动作。可见通用引用的确很强大。logAndAdd会把用户传入的name纪录到全局的数据结构中，但是有的时候用户想传入一个id，然后通过id查找到对应的name最后再添加到全局的数据结构中，于是就有了下面这个重载版本的logAndAdd，如下:
```
std::string nameFromIdx(int idx);

void logAndAdd(int idx) {
  auto now = std::chrono::system_clock::now();
  log(now, "logAndAdd");
  names.emplace(nameFromIdx(idx));
}
```
然后用户开始调用下面的代码来使用logAndAdd方法。
```
std::string petName("Darla");
logAndAdd(petName);
logAndAdd(std::string("persephone"));
logAndAdd("petty dog");
logAndAdd(22);  //调用void logAndAdd(int idx)重载
```
一切都跟预期一样正常运行，直到有有一天，出现了下面这段调用代码。
```
short nameIdx;
logAndAdd(nameIdx);     // error，调用的是通用引用版本的logAndAdd
```
上面的代码不符合预期，编译失败了，说到这里才是本文的开始，本文要讲的就是`避免给通用引用进行重载`，因为通用引用一般会结合模版，在上面的代码中因为nameIdx是short类型会导致实例化一个接收short类型参数的logAndAdd函数，根据重载函数的匹配原则优先会匹配函数签名完全一致的重载版本，因此这里选择了通用引用的版本，而names.emplace接收的是string，但是传入的却是short类型所以出现了编译失败。

​接着上面的话题，接下来我们来看看另外一个误给通用引用进行重载导致编译失败的例子。
```
class Person {
public:
    template<typename T>
    explicit Person(T&& n)
        : name(std::forward<T>(n)) {}
    explicit Person(int idx);
    Person(const Person& rhs);      //编译器自动生成的
    Person(Person&& rhs);           //编译器自动生成的
private:
  std::string name;
};

Person p("test");
auto cloneOfp(p);   //编译失败
```
上面的例子编译失败了，因为cloneOfp(p)调用的是通用引用版本的构造函数，而不是编译器生成的拷贝构造函数，因为拷贝构造函数的参数是const Person类型，而传入的p是Person类型，当通用版本的构造函数将p传后赋值给name的时候会因为两者的类型不同而导致编译失败。可以将上面的Person p(“test”)，改成const Person p(“test”)后就编译通过了并且是复合预期行为的。

除了上面这个例子外，通用引用在类继承的场景下也存在一些问题，例如下面这段代码
```
class SpecialPerson: public Person {
 public:
  SpecialPerson(const SpecialPerson& rhs)  
                : Person(rhs) {... }
  SpecialPerson(SpecialPerson&& rhs) 
                : Person(std::move(rhs)) {....}
};
```
上面的代码中SpecialPerson继承自Person，在拷贝构造和移动构造的时候，间接的调用父类的拷贝构造和移动构造， 但是实际上调用的却是父类Person的完美转发的函数，这是因为传递到父类的参数是SpecialPerson这会导致父类的完美转发函数对SpecialPerson类型进行实例化。

