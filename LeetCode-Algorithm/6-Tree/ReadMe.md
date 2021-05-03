## 树

### 114 二叉树的前序遍历

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


### 94 二叉树的中序遍历

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

### 102 二叉树的层序遍历
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

### 236 二叉树的最近公共祖先
* 给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。
>*Tips: 中最近公共祖先的定义为：“对于有根树 T 的两个节点 p、q，最近公共祖先表示为一个节点 x，满足 x 是 p、q 的祖先且 x 的深度尽可能大（一个节点也可以是它自己的祖先）。”*

* 示例
![img](https://assets.leetcode.com/uploads/2018/12/14/binarytree.png)
```
输入：root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
输出：3
解释：节点 5 和节点 1 的最近公共祖先是节点 3 。
```

* 题解
```
递归解法：
给定两个节点p和q:
	如果p和q都存在，则返回它们的公共祖先；
	如果只存在一个，则返回存在的一个；
	如果p和q都不存在，则返回NULL
具体思路：
（1）如果当前结点root等于NULL，则直接返回 NULL
（2）如果root等于p或者q ，那这棵树一定返回p或者q
（3）然后递归左右子树，因为是递归，使用函数后可认为左右子树已经算出结果，用left和right表示
（4）此时若left为空，那最终结果只要看right；若right 为空，那最终结果只要看left
（5）如果left和right都非空，因为只给了p和q两个结点，都非空，说明一边一个，因此 root是他们的最近公共祖先
（6）如果left和right都为空，则返回空（其实已经包含在前面的情况中了）

非递归解法：
使用后序遍历可以获取某一节点的所有祖先节点，在获取俩个节点的所有祖先节点后，从后往前遍历取最后一个相同节点作为最近公共祖先节点。
```

* 代码
```

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        std::vector<TreeNode*> road_p,road_q;
        getPath(root,road_p,p);
        getPath(root,road_q,q);

        int length = min(road_p.size(),road_q.size());

        for(int i=length-1;i>=0;--i)
        {
           if(road_p[i]==road_q[i])
                return road_p[i];
        }

        return nullptr;
    }

    bool getPath(TreeNode* root,std::vector<TreeNode*>& road,TreeNode* end)
    {
        TreeNode* node=root;
        TreeNode* pre_visit=nullptr;

        while(node!=nullptr||!road.empty())
        {
            while(node!=nullptr)
            {
                road.push_back(node);
                if(node == end) return true;
                node=node->left;
            }

            node=road.back();
            if(node->right==nullptr||node->right==pre_visit)
            {
                pre_visit = node;
                node = nullptr;
                road.pop_back();
            }
            else{
                node = node->right;
            }
        }
        return true;
    }
};

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if(root == NULL)
            return NULL;
        if(root == p || root == q) 
            return root;
            
        TreeNode* left =  lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
       
        if(left == NULL)
            return right;
        if(right == NULL)
            return left;      
        if(left && right) // p和q在两侧
            return root;
        
        return NULL; // 必须有返回值
    }
};
```
## Contack

If you found bugs or have new ideas,please pull requests😄   
If you have trouble compiling or using this software,email to [15313326374@163.com](mailto:15313326374@163.com)  