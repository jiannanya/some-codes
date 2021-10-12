#include<iostream>
#include<vector>
#include<algorithm>

std::vector<int> quicksort(std::vector<int> &nums, int left, int right){
    if(nums.size()<=1 || left<0 || right>=nums.size() || left >= right)return nums;//应对边界条件

    int pivot = nums[left];
    int i = left, j = right;
    while (i<j)
    {
        while(i<j && nums[j]>=pivot)--j;//需加等于号，应对重复元素
        std::swap(nums[i],nums[j]);

        while(i<j && nums[i]<pivot)++i;
        std::swap(nums[i],nums[j]);
    }
    quicksort(nums, left, i-1);
    quicksort(nums, i+1, right);

    return nums;
}

int main(int argc, char** argv){
    std::vector<int> nums{1,5,6,3,7,4,9,0,5,2};
    for(auto &e: nums){
        std::cout<<e<<",";
    }
    quicksort(nums, 0, nums.size()-1);
    std::cout<<std::endl;
    for(auto &e: nums){
        std::cout<<e<<",";
    }
    std::cout<<std::endl;
    return 0;
}