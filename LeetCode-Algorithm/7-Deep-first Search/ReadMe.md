# 22. 括号生成
## 题目
```
数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。
```
## 示例 ：
```
输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
```
## Code
```cpp
class Solution {
//方法一：递归
/*
private:
    vector<string> result;
    void getParenthesis(string temp,int left,int right){
        if(left==0&&right==0){
            result.push_back(temp);
            return;
        }

        if(left==right){
            getParenthesis(temp+"(",left-1,right);
        }
        else{
            if(left>0){
                getParenthesis(temp+"(",left-1,right);
            }
            getParenthesis(temp+")",left,right-1);
        }
    }
public:
    vector<string> generateParenthesis(int n) {
        if(n<=0) return{};
        string temp;
        getParenthesis(temp,n,n);
        return result;

    }
*/
//方法二:回溯
private:
    vector<string> result;
    void backTracking(string& temp,int left,int right,int n){
        if(temp.size()==2*n){
            result.push_back(temp);
            return;
        }

        if(left<n){
            temp.push_back('(');
            backTracking(temp,left+1,right,n);
            temp.pop_back();
        }

        if(left>right){
            temp.push_back(')');
            backTracking(temp,left,right+1,n);
            temp.pop_back();
        }
    }
public:
    vector<string> generateParenthesis(int n) {
        if(n<=0) return{};
        string temp;
        backTracking(temp,0,0,n);
        return result;

    }
};
```
