# 字典树
```
Trie [traɪ] 读音和 try 相同，它的另一些名字有：字典树，前缀树，单词查找树等
```
## 介绍Trie🌳
```
Trie 是一颗非典型的多叉树模型，多叉好理解，即每个结点的分支数量可能为多个
为什么说非典型呢？因为它和一般的多叉树不一样，尤其在结点的数据结构设计上，比如一般的多叉树的结点是这样的：
```

```cpp

struct TreeNode {
    VALUETYPE value;    //结点值
    TreeNode* children[NUM];    //指向孩子结点
};
```

```
而 Trie 的结点是这样的(假设只包含'a'~'z'中的字符)：
```

```cpp
struct TrieNode {
    bool isEnd; //该结点是否是一个串的结束
    TrieNode* next[26]; //字母映射表
};
```

```
要想学会 Trie 就得先明白它的结点设计。我们可以看到TrieNode结点中并没有直接保存字符值的数据成员，那它是怎么保存字符的呢？
这时字母映射表next 的妙用就体现了，TrieNode* next[26]中保存了对当前结点而言下一个可能出现的所有字符的链接，因此我们可以通过一个父结点来预知它所有子结点的值：
```

```cpp
for (int i = 0; i < 26; i++) {
    char ch = 'a' + i;
    if (parentNode->next[i] == NULL) {
        说明父结点的后一个字母不可为 ch
    } else {
        说明父结点的后一个字母可以是 ch
    }
}
```
## 图示
```
我们来看个例子吧。
想象以下，包含三个单词 "sea","sells","she" 的 Trie 会长啥样呢？
```
![](https://pic.leetcode-cn.com/e3c98484881bd654daa8419bcb0791a2b6f8288b58ef50df70ddaeefc4084f48-file_1575215107950)
![](https://pic.leetcode-cn.com/3a0be6938b0a5945695fcddd29c74aacc7ac30f040f5078feefab65339176058-file_1575215106942)



## 总结
```
通过以上介绍和代码实现我们可以总结出 Trie 的几点性质：
Trie 的形状和单词的插入或删除顺序无关，也就是说对于任意给定的一组单词，Trie 的形状都是唯一的。
查找或插入一个长度为 L 的单词，访问 next 数组的次数最多为 L+1，和 Trie 中包含多少个单词无关。
Trie 的每个结点中都保留着一个字母表，这是很耗费空间的。如果 Trie 的高度为 n，字母表的大小为 m，最坏的情况是 Trie 中还不存在前缀相同的单词，那空间复杂度就为 O(m^n).
最后，关于 Trie 的应用场景，希望你能记住 8 个字：一次建树，多次查询
```
# 208. 实现 Trie (前缀树)
## 题目

```
Trie（发音类似 "try"）或者说 前缀树 是一种树形数据结构，用于高效地存储和检索字符串数据集中的键。这一数据结构有相当多的应用情景，例如自动补完和拼写检查.
请你实现 Trie 类：
Trie() 初始化前缀树对象.
void insert(String word) 向前缀树中插入字符串 word.
boolean search(String word) 如果字符串 word 在前缀树中，返回 true（即，在检索之前已经插入）；否则，返回 false.
boolean startsWith(String prefix) 如果之前已经插入的字符串 word 的前缀之一为 prefix ，返回 true ；否则，返回 false.
```

## 示例1：
```cpp
输入
["Trie", "insert", "search", "search", "startsWith", "insert", "search"]
[[], ["apple"], ["apple"], ["app"], ["app"], ["app"], ["app"]]
输出
[null, null, true, false, true, null, true]

解释
Trie trie = new Trie();
trie.insert("apple");
trie.search("apple");   // 返回 True
trie.search("app");     // 返回 False
trie.startsWith("app"); // 返回 True
trie.insert("app");
trie.search("app");     // 返回 True
```
## Code
```cpp
class Trie {
private:
    bool isEnd;
    Trie *child[26];
public:
    /** Initialize your data structure here. */
    Trie() {
        isEnd=false;
        for(int i=0;i<26;i++){
            child[i]=nullptr;
        }
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        auto temp=this;
        for(auto iter:word){
            if(temp->child[iter-'a']==nullptr){
                temp->child[iter-'a']=new Trie();
            }
            temp=temp->child[iter-'a'];
        }
        temp->isEnd=true;
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        auto temp=this;
        for(auto iter:word){
            if(temp->child[iter-'a']==nullptr){
                return false;
            }
            temp=temp->child[iter-'a'];
        }
        return temp->isEnd;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        auto temp=this;
        for(auto iter:prefix){
            if(temp->child[iter-'a']==nullptr){
                return false;
            }
            temp=temp->child[iter-'a'];
        }
        return true;
    }
};

/**
 * Your Trie object will be instantiated and called as such:
 * Trie* obj = new Trie();
 * obj->insert(word);
 * bool param_2 = obj->search(word);
 * bool param_3 = obj->startsWith(prefix);
 */
 ```
