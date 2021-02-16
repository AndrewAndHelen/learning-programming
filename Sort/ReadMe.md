# 十大经典排序算法

> *本系列算法整理自：https://github.com/hustcc/JS-Sorting-Algorithm*



- **概述**

排序算法可以分为内部排序和外部排序，内部排序是数据记录在内存中进行排序，而外部排序是因排序的数据很大，一次不能容纳全部的排序记录，在排序过程中需要访问外存。常见的内部排序算法有：**插入排序、希尔排序、选择排序、冒泡排序、归并排序、快速排序、堆排序、基数排序**等。用一张图概括：

![十大经典排序算法 概览截图](res/sort.png)



- **关于稳定性**：

> 稳定的排序算法：冒泡排序、插入排序、归并排序、计数排序、桶排序和基数排序。
>
> 不是稳定的排序算法：选择排序、快速排序、希尔排序、堆排序。



- **名词解释**：

> **n**：数据规模
>
> **k**：“桶”的个数
>
> **In-place**：占用常数内存，不占用额外内存
>
> **Out-place**：占用额外内存
>
> **稳定性**：排序后 2 个相等键值的顺序和排序之前它们的顺序相同



- **内容大纲**

1. [冒泡排序](1.bubbleSort.md)
2. [选择排序](2.selectionSort.md)
3. [插入排序](3.insertionSort.md)
4. [希尔排序](4.shellSort.md)
5. [归并排序](5.mergeSort.md)
6. [快速排序](6.quickSort.md)
7. [堆排序](7.heapSort.md)
8. [计数排序](8.countingSort.md)
9. [桶排序](9.bucketSort.md)
10. [基数排序](10.radixSort.md)



- **排序算法关系表**

<table style="text-align:center;">
<tr>
    <td colspan="7">比较排序</td>
    <td colspan="3">非比较排序</td>
</tr>
<tr>
    <td colspan="2">插入类排序</td>
    <td colspan="2">选择类排序</td>
    <td colspan="2">交换类排序</td>
    <td colspan="1">合并类排序</td>
    <td colspan="3">非比较类排序</td>
</tr>
<tr>
    <td>插入排序</td>
    <td>希尔排序</td>
    <td>选择排序</td>
    <td>堆排序</td>
    <td>冒泡排序</td>
    <td>快递排序</td>
    <td>归并排序</td>
    <td>计数排序</td>
    <td>桶排序</td>
    <td>基数排序</td>
</tr>
</table></div>

