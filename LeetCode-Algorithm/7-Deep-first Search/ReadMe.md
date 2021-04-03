# 深度优先搜索
** 22**. 括号生成
* 数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。

* 示例 
```
输入：n = 3
输出：["((()))","(()())","(())()","()(())","()()()"]
```
* 代码
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

**110**. 平衡二叉树
* 给定一个二叉树，判断它是否是高度平衡的二叉树。本题中，一棵高度平衡二叉树定义为：一个二叉树每个节点 的左右两个子树的高度差的绝对值不超过 1 。

* 示例
```
输入：root = [3,9,20,null,null,15,7]
输出：true
```

* 代码
```
自顶向下
class Solution {
public:
    int height(TreeNode* root)
    {
        if(root==nullptr)
            return 0;
        else
            return max(height(root->left),height(root->right))+1;
    }
    bool isBalanced(TreeNode* root) {
        if(root==nullptr)
            return true;
        else if(abs(height(root->left)-height(root->right))<=1&&isBalanced(root->left)&&isBalanced(root->right))
            return true;
        else
            return false;
    }
};
自底向上
class Solution
{
public:
    int height(TreeNode* root)
    {
        if(root==nullptr)
            return 0;
        
        int leftheight = height(root->left);
        int rightheight = height(root->right);

        if(leftheight==-1||rightheight==-1||abs(leftheight-rightheight)>1)
            return -1;
        else
            return max(leftheight,rightheight)+1;
    }

    bool isBalanced(TreeNode* root) {
       return height(root)>=0;
    }
};
```