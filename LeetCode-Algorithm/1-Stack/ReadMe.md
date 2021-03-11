# 栈

1047. **删除字符串S中所有相邻重复项**

* 题解

```
建立空字符串sub_str，遍历字符串S，分两种情况进行：
(1)如果sub_str尾部字符与S当前字符c不相同，将c压入到sub_str尾部。
(2)如果sub_str尾部字符与S当前字符c相同，sub_str尾部字符出栈。
```
* 代码
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

1048. **最长字符串链**

* 题目描述：从给定单词列表 `words` 中选择单词组成词链，返回词链的最长可能长度。
* 示例

```
输入：["a","b","ba","bca","bda","bdca"]
输出：4
解释：最长单词链之一为 "a","ba","bda","bdca"。
```



> **词链**：[word_1, word_2, ..., word_k] 组成的序列，k >= 1，其中 word_1 是 word_2 的前身，word_2 是 word_3 的前身，依此类推。

* 题解

```
伪代码：
 int longestStrChain(vector<string>& words)
 {
 	sort(words);		//按照字符串大小升序排列words
 	map<string, int> d;	//建立map存储词链中最长的字符串与词链的长度
 	
 	int max_list_size=0;//最大词链长度
 	for i:= 0 To words.size()
 	{
 		将words[i]作为d的一个键；
 		if(words[i]长度为1)
 			d[words[i]]=1;
 		else
 		{
 			for j:=0:words[i].size()
 			{
 				提取words[i]除去位置j的子字符串sub_str;
 				
 				if(d中存在sub_str)
 					d[words[i]]=max(d[words[i]],d[sub_str]+1);
 				else
 					d[words[i]]=max(d[words[i]],1);
 			}
 		}
 		
 		max_list_size = max(max_list_size,d[words[i]]);
 	}
 	return max_list_size;
 }
```

* 代码

```c++
class Solution {
public:
    int longestStrChain(vector<string>& words) {
        sort(words.begin(),words.end(),[&](string a,string b){return a.size()<b.size();});
        int ans=0;
        map<string, int> d;


        for(int i=0;i<words.size();i++)
        {
            int& word_len=d[words[i]];
            if(words[i].size()==1)
                i_len=1;
            else
            {
                for(int j=0;j<words[i].size();j++)
                {
                    string sub_str;
                    if(j==0)
                        sub_str = words[i].substr(1);
                    else if(j==words[i].size()-1)
                        sub_str = words[i].substr(0,j);
                    else
                    {
                        sub_str += words[i].substr(0,j);
                        sub_str += words[i].substr(j+1);
                    }

                    if(d.count(sub_str))
                        word_len = max(word_len,d[sub_str]+1);
                    else
                        word_len = max(word_len,1);
                }
            }
            ans =max(ans,word_len);
           
        }

        return ans;
    }
};
```

227 基本计算器二

* 题目描述:给你一个字符串表达式 `s` ，请你实现一个基本计算器来计算并返回它的值。整数除法仅保留整数部分。

* 示例

```
输入：s = " 3+5 / 2 "
输出：5
```

* 题解

```
由于乘除优先于加减计算，因此不妨考虑先进行所有乘除运算，并将这些乘除运算后的整数值放回原表达式的相应位置，则随后整个表达式的值，就等于一系列整数加减后的值。

基于此，我们可以用一个栈，保存这些（进行乘除运算后的）整数的值。对于加减号后的数字，将其直接压入栈中；对于乘除号后的数字，可以直接与栈顶元素计算，并替换栈顶元素为计算后的结果。

具体来说，遍历字符串 ss，并用变量preSign记录每个数字之前的运算符，对于第一个数字，其之前的运算符视为加号。每次遍历到数字末尾时，根据preSign来决定计算方式：

加号：将数字压入栈；
减号：将数字的相反数压入栈；
乘除号：计算数字与栈顶元素，并将栈顶元素替换为计算结果。
代码实现中，若读到一个运算符，或者遍历到字符串末尾，即认为是遍历到了数字末尾。处理完该数字后，更新preSign为当前遍历的字符。

遍历完字符串 ss 后，将栈中元素累加，即为该字符串表达式的值。
```

* 代码
```c++
class Solution {
public:
    int calculate(string s) {
        int num = 0;
        std::vector<int> stk;
        char pre_sign='+';

        int n = s.size();
        for (int i = 0; i < n; i++)
        {
            if (!isdigit(s[i]) && i == 0 && s[i] != ' ')
				pre_sign = s[i];			//如果第一个字符为运算符，如‘-’，需要修改默认值
            if (isdigit(s[i]))
                num = num * 10 + static_cast<int>(s[i] - '0');
            if ((!isdigit(s[i]) && s[i] != ' ')||i==n-1)
            {
                switch (pre_sign)
                {
                case '+':
                    stk.push_back(num); break;
                case  '-':
                    stk.push_back(-num); break;
                case  '*':
                    stk.back() *= num; break;
                case '/':
                    stk.back() /= num; break;
                }

                pre_sign = s[i];
                num = 0;
            }
        }

        return std::accumulate(stk.begin(), stk.end(),0);

    }
};
```
>Tips:#include<string> #include <ctype.h> #include <vector> #include<numeric> #include<iostream>

## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  