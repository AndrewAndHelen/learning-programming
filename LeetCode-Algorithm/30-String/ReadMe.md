# 字符串

5. **最长回文串**

`题目描述`:给你一个字符串 s，找到 s 中最长的回文子串。
`示例:`

```
输入：s = "babad"
输出："bab"
解释："aba" 同样是符合题意的答案。
```

`题解:`中心扩展法：

```
边界情况即为子串长度为1或2的情况。我们枚举每一种边界情况，并从对应的子串开始不断地向两边扩展。如果两边的字母相同，我们就可以继续扩展，例如从 P(i+1,j-1)扩展到P(i,j)；如果两边的字母不同，我们就可以停止扩展，因为在这之后的子串都不能是回文串了。
```

`代码:`

```c++
class Solution {
public:
    pair<int,int> subPalindrome(string& s,int left,int right)
    {
        while(left>=0&&right<s.size()&&s[left]==s[right])
        {
            --left;
            ++right;
        }

        return pair<int,int>(left+1,right-1);
    }

    string longestPalindrome(string s) {
        int start=0,end=0;
        int left1,right1,left2,right2;
        for(int i=0;i<s.size()-1;i++)
        {
            tie(left1,right1)=subPalindrome(s,i,i);
            tie(left2,right2)=subPalindrome(s, i,i+1);

            if(right1-left1>end-start)
            {
                start=left1;
                end=right1;
            }
            if(right2-left2>end-start)
            {
                start=left2;
                end=right2;
            }
        }

        return s.substr(start,end-start+1);
    }
};
```









## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  