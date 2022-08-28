## Item 13:优先考虑const_iterator而⾮iterator

* 引言
```
1.优先考虑const_iterator而⾮iterator
2.在最⼤程度通⽤的代码中，优先考虑⾮成员函数版本的begin，end，rbegin等，而⾮同名成员函
数
```

* 13.1 基本使用方法

```
std::vector<int> values; 
…
auto it = // 使⽤cbegin
std::find(values.cbegin(),values.cend(), 1983); // 和cend
values.insert(it, 1998);
```

* 13.2 使用⾮成员函数版本的begin，end，rbegin
```
template<typename C, typename V>
void findAndInsert(C& container, // 在容器中查找第⼀次
const V& targetVal, // 出现targetVal的位置，
const V& insertVal) // 然后插⼊insertVal
{
    using std::cbegin; // there
    using std::cend;
    auto it = std::find(cbegin(container), // ⾮成员函数cbegin
    cend(container), // ⾮成员函数cend
    targetVal);
    container.insert(it, insertVal);
}
```
> *Tips:C++11没有添加cbegin，cend，rbegin，rend，crbegin，crend.*

* 13.3 使用begin实现cbegin
```
template <class C>
auto cbegin(const C& container)->decltype(std::begin(container))
{
return std::begin(container); 
}
```
>*Tips:假设C为std::vector<int>，container为 const std::vector<int>&，对const容器调⽤⾮成员函数begin（由C++11提供)将产出const_iterator。*

