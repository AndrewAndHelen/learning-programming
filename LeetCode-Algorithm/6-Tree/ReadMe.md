# 树

**114** 二叉树的前序遍历

* 给定一个二叉树的根节点 `root` ，返回它的 **前序** 遍历。

* 示例
```
输入：root = [1,null,2,3]
输出：[1,2,3]
```

* 题解
```
根节点存入栈中打印根节点，然后访问这个根节点的左子树，左子树也是将左子树的根存入栈中打印根节点，依次往下直到左子树为空，再取出栈顶元素，栈顶元素(访问完左子树的根节点)作为新的根节点去访问右子树。
```

* 代码
```
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
//recursion
class Solution {
public:
    void traverse(TreeNode* T,vector<int>& result)
    {
    	result.push_back(T->val);
        if(T->left) traverse(T->left,result);
        if(T->right) traverse(T->right,result);
    }

    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        if(root)
        {
            traverse(root,result);
            return result;  
        }

        return {};
    }
};

no recursion
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        TreeNode* head = root;

        stack<TreeNode*> sta;

        while(head!=nullptr||!sta.empty())
        {
            while(head!=nullptr)
            {
                result.push_back(head->val);
                sta.push(head);
                head=head->left;
            }

            head = sta.top();
            head=head->right;
            sta.pop();
        }
        return result;
    }
};
```


**94** 二叉树的中序遍历

* 给定一个二叉树的根节点 `root` ，返回它的 **中序** 遍历。

* 示例
```
输入：root = [1,null,2,3]
输出：[1,3,2]
```

* 题解
```
中序遍历（LDR）是二叉树遍历的一种，也叫做中根遍历、中序周游。可记做左根右
中序遍历首先遍历左子树，然后访问根结点，最后遍历右子树。
若二叉树为空则结束返回，否则：
1.遍历左子树
2.访问根结点
3.遍历右子树
```

* 代码
```
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
//recursion
class Solution {
public:
    void traverse(TreeNode* T,vector<int>& result)
    {
        if(T->left) traverse(T->left,result);
        result.push_back(T->val);
        if(T->right) traverse(T->right,result);
    }

    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        if(root)
        {
            traverse(root,result);
            return result;  
        }

        return {};
    }
};

// no recursion
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> sta;

        TreeNode* head=root;

        while(head!=nullptr||!sta.empty())
        {
            while(head!=nullptr)
            {
                sta.push(head);
                head=head->left;
            }

            head = sta.top();
            result.push_back(head->val);
            sta.pop();
            head = head->right;
        }
        return result;
    }
};
```

** **

**145** 二叉树的后序遍历

* 给定一个二叉树，返回它的后序遍历
* 示例
```
输入: [1,null,2,3]  
   1
    \
     2
    /
   3 

输出: [3,2,1]
```

* 题解
```
常规遍历左右根，和前序、中序方法一类似。
```

* 代码
```
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> sta;

        TreeNode* head=root;
        TreeNode* pre_visit=nullptr;

        while(head!=nullptr||!sta.empty())
        {
            while(head!=nullptr)
            {
                sta.push(head);
                head=head->left;
            }

            head=sta.top();
            if(head->right==nullptr||head->right==pre_visit)
            {
                result.push_back(head->val);
                pre_visit=head;

                head=nullptr;
                sta.pop();
            }
            else
            {
                head=head->right;
            }
        }

        return result;
    }
};
```

**102** 二叉树的层序遍历
* 给你一个二叉树，请你返回其按 层序遍历 得到的节点值。 （即逐层地，从左到右访问所有节点）。
* 示例
```
二叉树：[3,9,20,null,null,15,7],

    3
   / \
  9  20
    /  \
   15   7
返回其层序遍历结果：

[
  [3],
  [9,20],
  [15,7]
]
```

* 题解
```
1. 初始化：i=1 的时候，队列里面只有root，是唯一的层数为1的元素，因为只有一个元素，所以也显然满足「从左向右排列」。
2. 当队列不空时，确定当前队列长度，循环将当前队列长度的结点出队列，将结点对应的值加入当前层的vector中，然后判断该结点左右子节点是否存在，存在，则依次加入队列。

```

* 代码
```
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        queue<TreeNode*> que;
        int current_num=0;

        if(root!=nullptr)
            que.push(root);

        while(!que.empty())
        {
            current_num=que.size();

            result.push_back(vector<int>());
            for(int i=1;i<=current_num;++i)
            {
                TreeNode* node=que.front();
                que.pop();
                result.back().push_back(node->val);

                if(node->left!=nullptr)
                    que.push(node->left);
                if(node->right!=nullptr)
                    que.push(node->right);
            }
        }

        return result;
    }
};
```


## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  