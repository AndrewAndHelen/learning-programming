#include "sort.h"

int main()
{
	SqList l = {9, { 9,8,7,8,5,4,3,2,1 } } ;
	MergeSort(&l);

	print(&l);
}