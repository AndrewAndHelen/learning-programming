#include"sort.h"
void swap(SqList* p, int i, int j)
{
	int tmp = p->r[j];
	p->r[j] = p->r[i];
	p->r[i] = tmp;
}

void BubbleSort(SqList* l)
{
	int length = l->length;
	for (int i=0;i<length;i++)
	{
		for (int j= length-1;j>=i+1;j--)
		{
			if (l->r[j] < l->r[j-1])
				swap(l, j, j-1);
		}
	}
}

void BubbleSort2(SqList* l)
{
	int length = l->length;
	bool flag = true;
	for (int i = 0; i < length&& flag; i++)
	{
		flag = false;
		for (int j = length - 1; j >= i + 1; j--)
		{
			if (l->r[j] < l->r[j - 1]) {
				swap(l, j, j - 1);
				flag = true;
			}
		}
	}
}

void SelectSort(SqList* l)
{
	int length = l->length;

	int min = 0;

	for (int i = 0; i < length; i++)
	{
		min = i;
		for (int j = i + 1; j < length; j++)
		{
			if (l->r[j] < l->r[min])
				min = j;
		}

		if(i!=min)
			swap(l, i, min);
	}
}

void InsertSort(SqList* l)
{
	int tmp;
	for (int i = 1; i < l->length; i++)
	{
		int j = i;
		int tmp = l->r[i];
		while (j-1>=0&& l->r[j-1] > tmp)
		{
			l->r[j] = l->r[j - 1];
			j = j--;
		}
		l->r[j] = tmp;
	}

}

//插入排序升级版
void ShellSort(SqList* l)
{
	int N = l->length;

	for (int gap = N / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < N; i++)
		{
			int j = i;
			int tmp = l->r[j];

			while (j-gap>=0&& l->r[j-gap]>tmp)
			{
				l->r[j] = l->r[j - gap];
				j = j - gap;
			}
			l->r[j] = tmp;

		}			
	}
}
//s,m为数组下标
static void HeapAdjust(SqList* l, int s, int m)
{
	int tmp, j;
	tmp = l->r[s];
	for (j = 2 * s+1; j < m; j = 2 * s + 1)//从父节点的左子节点开始
	{
		if (j < m&&l->r[j] < l->r[j + 1])
			j++;
		if(tmp>=l->r[j])
			break;
		l->r[s] = l->r[j];
		s = j;
	}
	l->r[s] = tmp;
}
//选择排序升级版
void HeapSort(SqList* l)
{
	int i;
	for (i = l->length / 2-1; i >=0; i--)
		HeapAdjust(l, i, l->length-1);

	for (int i = l->length-1; i > 1; i--)
	{
		swap(l, 0, i);
		HeapAdjust(l, 0, i-1);
	}
	
}
//将有序的SR[i..m]与SR[m+1..n]归并为有序的TR[i..n]
static void merge(int arr[] , int start, int mid, int end) {
	int*  temp = new int[end - start + 1];    //存放排序号数据的临时区域
	int k = 0;        //临时区域的指针
	int i = start;    //第一个有序区的指针
	int j = mid + 1;    //第二个有序区的指针

	while (i <= mid && j <= end) {
		if (arr[i] <= arr[j])
			temp[k++] = arr[i++];
		else
			temp[k++] = arr[j++];
	}
	while (i <= mid)
		temp[k++] = arr[i++];
	while (j <= end)
		temp[k++] = arr[j++];

	for (k = 0; k <= end - start; k++)
		arr[k + start] = temp[k];

	delete[] temp;
}

static void mSort(int arr[] , int start, int end) {
	if (start == end)
		return;
	int mid = (start + end) / 2;
	mSort(arr, start, mid);
	mSort(arr, mid + 1, end);
	merge(arr, start, mid, end);
}

//归并排序
void MergeSort(SqList *l)
{
	mSort(l->r,0, l->length-1);
}
static void quicksort(SqList* l, int low, int high)
{
	if (low >= high)
		return;
	int i = low, j = high;
	float index = l->r[i];

	while (i < j)
	{
		while (i < j&& l->r[j] >= index) { // 向左寻找第一个小于index的数
			j--;
		}

		if (i < j)
			l->r[i++] = l->r[j];// 将array[j]填入array[i]，并将i向右移动

		while (i < j&&l->r[i] < index) {// 向右寻找第一个大于index的数
			i++;
		}
		if (i < j)
			l->r[j--] = l->r[i];// 将array[i]填入array[j]，并将j向左移动
	}

	l->r[i] = index;
	quicksort(l, low, i - 1);
	quicksort(l, i + 1, high);

}
//冒泡排序升级版
void QuickSort(SqList* l)
{
	int len = l->length;
	quicksort(l, 0, len - 1);
}

void print(SqList* l)
{
	for (int i = 0; i < l->length; i++)
		std::cout << l->r[i] << "    ";

}