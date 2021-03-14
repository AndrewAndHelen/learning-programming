# 栈

71. 简化路径
* 题目描述：给你一个字符串 path ，表示指向某一文件或目录的 Unix 风格 绝对路径 （以 `/` 开头），请你将其转化为更加简洁的规范路径。

在 Unix 风格的文件系统中，一个点`.`表示当前目录本身；此外，两个点 `..` 表示将目录切换到上一级（指向父目录）；两者都可以是复杂相对路径的组成部分。任意多个连续的斜杠（即，`//`）都被视为单个斜杠 `/` 。 对于此问题，任何其他格式的点（例如，`...`）均被视为文件/目录名称。

* 请注意，返回的 规范路径 必须遵循下述格式：

  * 始终以斜杠 `'/'` 开头。

  * 两个目录名之间必须只有一个斜杠`'/'` 。
  * 最后一个目录名（如果存在）不能 以`'/'`结尾。
  * 此外，路径仅包含从根目录到目标文件或目录的路径上的目录（即，不含 `.` 或 `..`）。
  * 返回简化后得到的 规范路径 。

* 示例
```
输入：path = "/home/"
输出："/home"
解释：注意，最后一个目录名后面没有斜杠。 
```

* 题解
```
用/作为分隔，每次读取/之前的字符串内容，分情况谈论：
1. 如果 ""或者"."，就跳过
2. 否则，
（1）如果".."且保存分割内容的vector不空，则将栈顶字符串出栈
（2）否则push_back到vector
```

* 代码

```c++
class Solution {
public:
    string simplifyPath(string path) {
       stringstream ss(path);
       vector<string> strs;
       string str;

       while(getline(ss,str,'/'))
       {
           if(str!="."&&str!="")
           {
               if(str!="..")
                strs.push_back(str);
               else
                if(!strs.empty())
                    strs.pop_back();
           }
       }

       if(!strs.empty())
       {
            string clean_path;
            for(auto s:strs)
            {
                clean_path.append("/");
                clean_path.append(s);
            }
           return clean_path;
       }
       else
            return "/";
    }
};
```


227. 基本计算器二

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



 772. 计算器三
* 实现一个基本的计算器来计算简单的表达式字符串。表达式字符串只包含非负整数，算符 +、-、*、/ ，左括号 ( 和右括号 ) 。整数除法需要 向下截断 。

* 示例
```
输入：s = "2 * (5 + 5 * 2) / 3 + (6 / 2 + 8)"
输出：21
```

* 题解
```
1.用string存储整行输入的中缀表达式；
2.接着从string的0位置开始判断字符，如果是数字，那就要判断后面是否是数字，如果是就不断扫描组成一个整数（暂不考虑负数和小数），最终组成一个整数，然后输出这个数（因为不用计算，所以直接输出即可）；
3.如果是左括号，直接进符号栈；
4.如果是操作运算符，与符号栈的栈顶元素比较优先级：如果高就压入栈；低，就取出符号栈顶的元素输出；
接着，再判断符号栈顶的元素和当前的运算符号继续比较优先级，重复前面步骤，直到栈空或者当前的符号优先级高；
5.如果是右括号，把符号栈栈顶的元素取出，如果不是左括号，把取出的运算符输出，接着取符号栈栈顶的元素，直到符号栈中取出的符号是左括号；
6.当扫描完字符数组时，判断符号栈是否为空：不为空，把符号栈栈顶的元素取出，输出到窗口，直到符号栈为空。
```

* 代码
```
void reduceSpace(std::string& str)
{
	std::string substr;
	for (auto c : str)
	{
		if (c != ' ')
			substr.push_back(c);
	}

	str.assign(substr.begin(), substr.end());
}

bool isOperator(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/')
		return true;
	else
		return false;
}

int getPriority(char ch)
{
	int level = 0;
	
	switch (ch)
	{
	case '(': level = 1; break;
	case '+': level = 2; break;
	case  '-': level = 2; break;
	case  '*': level = 3; break;
	case '/':level = 3; break;

	default:break;
	}
	
	return level;
}

void infix2suffix(std::string infix, std::vector<std::string>& suffix)
{
	reduceSpace(infix);
	std::string op_stack;			//操作符栈

	int i = 0;
	int len = infix.size();
	
	while (i<len)
	{
		int num = 0;

		if (isdigit(infix[i]))
		{
			do {
				num = num * 10 + (infix[i] - '0');
				i++;
			} while (isdigit(infix[i]));

			suffix.push_back(std::to_string(num));
		}
		else if (infix[i] == '(')
		{
			op_stack.push_back(infix[i]);
			i++;
		}
		else if (isOperator(infix[i]))
		{
			
			while (op_stack.size() > 0)
			{
				if (getPriority(infix[i]) > getPriority(op_stack.back()))  //当前元素优先级高于栈顶元素，高，入栈，低，栈顶元素输出
				{
					op_stack.push_back(infix[i]);
					break;
				}
				else
				{
					char tmp = op_stack.back();
					suffix.push_back(std::string(1,tmp));

					op_stack.pop_back();
				}
			}

			if (op_stack.size() == 0)
				op_stack.push_back(infix[i]);

			i++;
		}
		else if (infix[i] == ')')
		{
			while (op_stack.back() != '(')
			{
				char tmp = op_stack.back();
				op_stack.pop_back();
				suffix.push_back(std::string(1,tmp));
			}
			op_stack.pop_back();					//左括号不要
			i++;

		}

	}

	while (op_stack.size()>0)
	{
		char tmp = op_stack.back();
		op_stack.pop_back();
		suffix.push_back(std::string(1, tmp));
	}
	
}

int calculate(std::string str)
{
	std::vector<std::string> suffix;
	infix2suffix(str, suffix);

	int num1, num2,result;
	std::vector<int> num;

	for (auto str : suffix)
	{
		if (str == "+" || str == "-" || str == "*" || str == "/")
		{
			num1 = num.back();
			num.pop_back();
			num2 = num.back();
			num.pop_back();

			if (str == "+")
				result = num2 + num1;
			if (str == "-")
				result = num2 - num1;
			if (str == "*")
				result = num2 * num1;
			if (str == "/")
				result = num2 / num1;

			num.push_back(result);
		}
		else
			num.push_back(atoi(str.c_str()));
	}

	return num.back();

}
```



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




## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  