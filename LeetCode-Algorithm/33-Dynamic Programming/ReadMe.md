# 动态规划
**62**. 最小路径和

* 一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为 “Start” ）。机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为 “Finish” ）。问总共有多少条不同的路径？

* 示例
```
输入：m = 3, n = 2
输出：3
解释：
从左上角开始，总共有 3 条路径可以到达右下角。
1. 向右 -> 向下 -> 向下
2. 向下 -> 向下 -> 向右
3. 向下 -> 向右 -> 向下
```

* 题解

```
初始状态：f[0][0]=1;
转移方程：

当前位置到达路径和为上方路径和与左方路径和，即有 f[i][j] = f[i][j-1]+f[i-1][j]

返回值：f[rows-1][cols-1]
```

* 代码
```
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> f(m,vector<int>(n,0));
        f[0][0]=1;
        for(int i=1;i<m;++i)
            f[i][0]=1;
        for(int j=1;j<n;++j)
            f[0][j]=1;
        
        for(int i=1;i<m;++i)
            for(int j=1;j<n;++j)
                f[i][j] = f[i-1][j]+f[i][j-1];
        
        return f[m-1][n-1];
    }
};
```



**64**. 最小路径和

* 给定一个包含非负整数的 m x n 网格 grid ，请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。

* 说明：每次只能向下或者向右移动一步。

* 示例
```
输入：grid = [[1,3,1],[1,5,1],[4,2,1]]
输出：7
解释：因为路径 1→3→1→1→1 的总和最小。
```

* 题解

```
初始状态：f[0][0]=grid[0][0];
转移方程：
1. 当前位置只能通过 往下 移动而来，即有 f[i][j] = f[i-1][j] + grid[i][j]
2. 当前位置只能通过 往右 移动而来，即有 f[i][j] = f[i][j-1] + grid[i][j]
3. 当前位置既能通过 往下 也能 往右 移动，即有 f[i][j] = min(f[i][j-1],f[i-1][j]) + grid[i][j]

返回值：f[rows-1][cols-1]
```

* 代码
```
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();

        vector<vector<int>> f(rows,vector<int>(cols,0));
        f[0][0]=grid[0][0];

        for(int i=1;i<rows;++i)
            f[i][0]=f[i-1][0]+grid[i][0];
        
        for(int j=1;j<cols;++j)
            f[0][j]=f[0][j-1]+grid[0][j];
        
        for(int i=1;i<rows;++i)
            for(int j=1;j<cols;++j)
            {
                f[i][j]=min(f[i-1][j]+grid[i][j],f[i][j-1]+grid[i][j]);
            }
       
        return f[rows-1][cols-1];
    }
};

```



**139**.单词拆分

* 给定一个**非空**字符串 *s* 和一个包含**非空**单词的列表 *wordDict*，判定 *s* 是否可以被空格拆分为一个或多个在字典中出现的单词。
* 说明
  * 拆分时可以重复使用字典中的单词。
  * 你可以假设字典中没有重复的单词。

* 示例
```
输入: s = "applepenapple", wordDict = ["apple", "pen"]
输出: true
解释: 返回 true 因为 "applepenapple" 可以被拆分成 "apple pen apple"。注意你可以重复使用字典中的单词。

输入: s = "catsandog", wordDict = ["cats", "dog", "sand", "and", "cat"]
输出: false
```

* 题解
```
给出一本词典以及一个字符串，能否切分这个字符串使得每个字串都在字典里面存在。
假设dp(i)表示长度为i的字串能否别切分，dp方程如下:
dp(i) = dp(j) && s[j, i) in dict, 0
```

* 代码
```
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> word_set;

        for(auto str:wordDict)
            word_set.insert(str);
        
        vector<bool> dp(s.size()+1);

        dp[0]=true;

        for(int i=1;i<=s.size();++i)
            for(int j=0;j<i;++j)
            {
                if(dp[j] && word_set.find(s.substr(j,i-j))!=word_set.end())
                {
                    dp[i]=true;
                    break;
                }  
            }
        return dp[s.size()];
    }
};
```