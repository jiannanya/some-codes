public class SortAlgorithm{


/**
* 冒泡排序
*/
public static int[] bubbleSort(int[] arr) {
    if(arr==null || arr.length <=1 ){
        return;
    }
    for (int i = 0; i < arr.length - 1; i++) {
        for (int j = 0; j < arr.length - i -1; j++) {   // 这里需要-1
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

/**
 * 希尔排序
 */
public static int[]  insertionSort(int[] arr){
    if(arr == null || arr.length <= 1){
        return arr;
    }
    //希尔排序  升序
    for (int d = arr.length / 2;d>0;d /= 2){ //d：增量  7   3   1
        for (int i = d; i < arr.length; i++){ 
            //i:代表即将插入的元素角标，作为每一组比较数据的最后一个元素角标 
            //j:代表与i同一组的数组元素角标
            for (int j = i-d; j>=0; j-=d){ //在此处-d 为了避免下面数组角标越界
                if (arr[j] > arr[j + d]) {// j+d 代表即将插入的元素所在的角标
                    //符合条件，插入元素（交换位置）
                    int temp = arr[j];
                    arr[j] = arr[j + d];
                    arr[j + d] = temp;
                }
            }
        } 
    }
    return arr;
}



/**
 * 归并排序，递归版
 */
public static int[] MergeSort(int []arr){

    if(arr == null || arr.length <= 1){
        return arr;
    }

    int []temp = new int[arr.length];//在排序前，先建好一个长度等于原数组长度的临时数组，避免递归中频繁开辟空间
    sort(arr,0,arr.length-1,temp);
}
private static void sort(int[] arr,int sta,int end,int []temp){
    if(sta<end){
        int mid = (sta+end)/2;
        sort(arr,sta,mid,temp);//左边归并排序，使得左子序列有序
        sort(arr,mid+1,end,temp);//右边归并排序，使得右子序列有序
        merge(arr,sta,mid,end,temp);//将两个有序子数组合并操作
    }
}
private static void merge(int[] arr,int sta,int mid,int end,int[] temp){

    int i = sta;//左序列指针
    int j = mid+1;//右序列指针
    int t = 0;//临时数组指针
    while (i<=mid && j<=end)
            temp[t++] = arr[i] <= arr[j] ? arr[i++] : arr[j++];

    while(i<=mid)//将左边剩余元素填充进temp中
        temp[t++] = arr[i++];
    
    while(j<=end)//将右序列剩余元素填充进temp中
        temp[t++] = arr[j++];
    
    t = 0;
    //将temp中的元素全部拷贝到原数组中
    while(sta <= end){
        arr[sta++] = temp[t++];
    }
}



}






