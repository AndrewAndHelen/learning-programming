# 栈

1047. **删除字符串S中所有相邻重复项**

`题解`：建立空字符串sub_str，遍历字符串S，分两种情况进行：
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

1048. **最长字符串链**

`题目描述`：从给定单词列表 `words` 中选择单词组成词链，返回词链的最长可能长度。
`例子：`给出一个单词列表，其中每个单词都由小写英文字母组成。如果在`word1`的任意地方添加一个字母使其变成`word2`，那么那么我们认为 `word1` 是 `word2` 的前身。例如，"abc" 是 "abac" 的前身。

```
输入：["a","b","ba","bca","bda","bdca"]
输出：4
解释：最长单词链之一为 "a","ba","bda","bdca"。
```



> **词链**：[word_1, word_2, ..., word_k] 组成的序列，k >= 1，其中 word_1 是 word_2 的前身，word_2 是 word_3 的前身，依此类推。

`题解`：

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