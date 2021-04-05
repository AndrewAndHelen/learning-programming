# 递归

**395** 至少有K个重复字符的最长子串

* 给你一个字符串 s 和一个整数 k ，请你找出 s 中的最长子串， 要求该子串中的每一字符出现次数都不少于 k 。返回这一子串的长度。

* 示例
```
输入：s = "ababbc", k = 2
输出：5
解释：最长子串为 "ababb" ，其中 'a' 重复了 2 次， 'b' 重复了 3 次。
```

* 题解
```
统计字符串s中每个字符出现的频率counter
for(auto c:s)
	if counter[c]<k次
		以c为分隔符分割s获取vector strs
		遍历strs，寻找最长子串长度
```

* 代码
```
class Solution {
public:
    int longestSubstring(string s, int k) {

        unordered_map<char,int> counter;
        for(auto c:s)
            counter[c]++;
        
        for(auto c:s)
        {
            int res = 0;
            if(counter[c]<k)
            {
                vector<string> strs;
                split(s, strs, c);
                
                for(auto tn:strs)
                    res = max(res,longestSubstring(tn,k));
                return res;
            }
        }
        
        return s.size();
    }

    void split(string s,vector<string>& strs,const char c)
    {
        stringstream ss(s);
        string temp;

        while(getline(ss,temp,c))
        {
            if(!temp.empty())
                strs.push_back(temp);
        }
    }
};
```


## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  