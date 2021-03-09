# 栈

1047. **删除字符串S中所有相邻重复项**

题解：建立空字符串sub_str，遍历字符串S，分两种情况进行：
(1)如果sub_str尾部字符与S当前字符c不相同，将c压入到sub_str尾部。
(2)如果sub_str尾部字符与S当前字符c相同，sub_str尾部字符出栈。
```c++
class Solution {
public:
    string removeDuplicates(string S) {
        string sub_str;
        for(auto c:S)
        {
            if(!sub_str.empty()&&new_S.back()==c)
                sub_str.pop_back();
            else
                sub_str.push_back(c);
        }

        return sub_str;
    }
};
```


## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  
