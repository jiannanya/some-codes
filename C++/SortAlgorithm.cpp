
/**
* 冒泡排序
*/
int* BubbleSort(int *arr, int len){

	if(!arr || len <= 1 )return nullptr;

	for(int i = 0;i < len - 1;i++){
		for(int j = 0;j < len - i - 1;j++){ //注意len-i-1
			if(arr[j]>arr[j+1]){
				int tmp = arr[j+1];
				arr[j+1] = arr [j];
				arr[j] = tmp;
			}
		}
	}

	return arr;
}

/**
* 简单选择排序
*/
int* SelectSort(int *arr, int len){

	if(!arr || len <= 1 )return nullptr;

	for (int i = 0;i < len - 1;i++){
		int min = i;
		for (int j = i + 1;j < len;j++){
			if (arr[min] > arr[j]){
				min = j;
			}
		}
		if (min != i){	
			int tmp = arr[i];
			arr[i] = arr[min];
			arr[min] = tmp;
		}
	}

	return arr;
}

/**
* 直接插入排序
*/
int* InsertSort(int *arr, int len){

	if(!arr || len <= 1 )return nullptr;

	for (int i = 1;i < len;i++){
		int tmp = arr[i];
		int j = i - 1;
		for (;j >= 0 && arr[j] > tmp;j--){
			arr[j + 1] = arr[j];
		}
		arr[j + 1] = tmp;
	}

	return arr;
}
/**
* 希尔排序
*/

int* ShellSort(int *arr, int len){

	if(!arr || len <= 1 )return nullptr;

	for (int gap = len/2; gap > 0; gap = gap/2){
		for (int i = gap;i < len;i++){
			int tmp = arr[i];
			int j = i - gap;
			for(; j>= 0 && arr[j] > tmp; j = j - gap){
					arr[j + gap] = arr[j];
			}
			arr[j + 1] = tmp;
		}
	}

	return arr;

}

/**
* 堆排序
*/
void AdjustHeap(int *arr, int i, int len){

	if(!arr || len <=1)return;

	int tmp = arr[i];
	for (int k = 2 * i + 1; k < len; k = 2 * k + 1)
	{
		if (k != len - 1 && arr[k] < arr[k + 1])
		{
			k++;
		}
		if (arr[k] > tmp)
		{
			arr[i] = arr[k];
			i = k;
		}
		else
		{
			break;
		}
	}
	arr[i] = tmp;
}
 
int* HeapSort(int *arr,int len)
{
	if (!arr || len <= 1)return nullptr;

	for (int i = len / 2 - 1;i >= 0 ;i--)
	{
		AdjustHeap(arr,i,len);
	}
	for (int j = len - 1;j > 0;j--)
	{
		Swap(arr,0,j);
		AdjustHeap(arr,0,j);
	}

	return arr;
}

void Swap(int *arr, int a, int b){

	if(!arr)return;

	int tmp = arr[a];
	arr[a] = arr[b];
	arr[b] = tmp;
}

/**
 * 归并排序，递归版
 */
int* MergeSort(int *arr, int len){

    if (!arr || len <= 1)return nullptr;

    int *tmp = new int[len];//在排序前，先建好一个长度等于原数组长度的临时数组，避免递归中频繁开辟空间
    sort(arr, 0, len-1, tmp);

	return arr;
}

void sort(int* arr, int sta, int end, int *tmp){
    if(sta < end){
        int mid = (sta+end)/2;
        sort(arr, sta, mid, tmp);//左边归并排序，使得左子序列有序
        sort(arr, mid+1, end, tmp);//右边归并排序，使得右子序列有序
        merge(arr, sta, mid, end, tmp);//将两个有序子数组合并操作
    }
}

void merge(int* arr, int sta, int mid, int end, int* tmp){

    int i = sta;//左序列指针
    int j = mid+1;//右序列指针
    int t = 0;//临时数组指针
    while (i<=mid && j<=end)
            tmp[t++] = arr[i] <= arr[j] ? arr[i++] : arr[j++];

    while(i<=mid)//将左边剩余元素填充进tmp中
        tmp[t++] = arr[i++];
    
    while(j<=end)//将右序列剩余元素填充进tmp中
        tmp[t++] = arr[j++];
    
    t = 0;
    //将tmp中的元素全部拷贝到原数组中
    while(sta <= end){
        arr[sta++] = tmp[t++];
    }
}


/*
 * 基数排序
 * @parameter
 * dec: 数据位数
 * order: 正在排序的位数，1代表个位，2代表十位，3代表百位
 */
#include <stdio.h>
#include <string.h>

/* 获取输入数字的索引值,dec指定数字的位数,3代表百位数,order指定需要获取哪一位的索引,1代表个位,2代表十位,3代表百位 */
int get_index(int num, int dec, int order)
{
    int i, j, n;
    int index;
    int div;

    /* 根据位数,循环减去不需要的高位数字 */
    for (i=dec; i>order; i--) {
        n = 1;
        for (j=0; j<dec-1; j++)
            n *= 10;
        div = num/n;
        num -= div * n;
        dec--;
    }

    /* 获得对应位数的整数 */
    n = 1;
    for (i=0; i<order-1; i++)
        n *= 10;

    /* 获取index */
    index = num / n;

    return index;
}

void radix_sort(int* arr, int len, int dec, int order)
{
    int i, j;
    int index;     /* 排序索引 */
    int *tmp = new int[len];  /* 临时数组,用来保存待排序的中间结果 */
    int *num = new int[10];   /* 保存索引值的数组 */
    memset(num, 0, 10*sizeof(int));  /* 数组初始清零 */
    memset(tmp, 0, len*sizeof(int)); /* 数组初始清零 */

    if (dec < order) /* 最高位排序完成后返回 */
        return;

    for (i=0; i<len; i++) {
        index = get_index(arr[i], dec, order);  /* 获取索引值 */
        num[index]++;  /* 对应位加一 */
    }

    for (i=1; i<10; i++)
        num[i] += num[i-1]; /* 调整索引数组 */

    for (i=len-1; i>=0; i--) {
        index = get_index(arr[i], dec, order);  /* 从数组尾开始依次获得各个数字的索引 */
        j = --num[index];  /* 根据索引计算该数字在按位排序之后在数组中的位置 */
        tmp[j] = arr[i]; /* 数字放入临时数组 */
    }

    for (i=0; i<len; i++)
        arr[i] = tmp[i];  /* 从临时数组复制到原数组 */

    printf("the %d time\n", order);
    for (i=0; i<30; i++)
        printf("%d  ", arr[i]);
    printf("\n");

    /* 继续按高一位的数字大小进行排序 */
    radix_sort(arr, len, dec, order+1);

    return;
}


