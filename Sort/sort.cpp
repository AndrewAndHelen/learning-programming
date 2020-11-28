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

//��������������
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
//s,mΪ�����±�
static void HeapAdjust(SqList* l, int s, int m)
{
	int tmp, j;
	tmp = l->r[s];
	for (j = 2 * s+1; j < m; j = 2 * s + 1)//�Ӹ��ڵ�����ӽڵ㿪ʼ
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
//ѡ������������
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
//�������SR[i..m]��SR[m+1..n]�鲢Ϊ�����TR[i..n]
static void merge(int arr[] , int start, int mid, int end) {
	int*  temp = new int[end - start + 1];    //�����������ݵ���ʱ����
	int k = 0;        //��ʱ�����ָ��
	int i = start;    //��һ����������ָ��
	int j = mid + 1;    //�ڶ�����������ָ��

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

//�鲢����
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
		while (i < j&& l->r[j] >= index) { // ����Ѱ�ҵ�һ��С��index����
			j--;
		}

		if (i < j)
			l->r[i++] = l->r[j];// ��array[j]����array[i]������i�����ƶ�

		while (i < j&&l->r[i] < index) {// ����Ѱ�ҵ�һ������index����
			i++;
		}
		if (i < j)
			l->r[j--] = l->r[i];// ��array[i]����array[j]������j�����ƶ�
	}

	l->r[i] = index;
	quicksort(l, low, i - 1);
	quicksort(l, i + 1, high);

}
//ð������������
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