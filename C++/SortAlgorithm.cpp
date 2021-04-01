
/**
* 冒泡排序
*/
int* BubbleSort(int *arr, int len){

	if(!arr or len <= 1 )return nullptr;

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

	if(!arr or len <= 1 )return nullptr;

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

	if(!arr or len <= 1 )return nullptr;

	for (int i = 1;i < len;i++){
		int tmp = arr[i];
		for (int j = i - 1;j >= 0 && arr[j] > tmp;j--){
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

	if(!arr or len <= 1 )return nullptr;

	for (int gap = len/2; gap > 0; gap = gap/2){
		for (int i = gap;i < len;i++){
			int tmp = arr[i];
			for(int j = i - gap; j>= 0 && arr[j] > tmp; j = j - gap){
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

	if(!arr or len <=1)return;

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
	if (!arr or len <= 1)return nullptr;

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

    if (!arr or len <= 1)return nullptr;

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