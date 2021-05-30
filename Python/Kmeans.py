#!/usr/bin/env python
#-*-coding:UTF-8 -*-
import random

#假设数据维度为2
def cal_of_kmeans(k,dataset,n=100):
    kinds = [[] for i in range(k)] #聚类表,共有k个类
    particle = [None for i in range(k)] #质点表
    particle[0] = dataset[0] #第一个质点为第一个数据点
    for i in range(1,k): #初始化k-1个质点
        particle[i]=dataset[random.randint(i,len(dataset)-1)]

    for times in range(n):  #迭代计算质点，用欧几里得距离,最大迭代次数为n
        distance = [] #每次迭代维护一个矩阵，记录每个数据点到每个已选择质点的距离
        for p in range(k):
            d = [] #每个数据点 到质点p的距离向量
            for one_data in dataset:
                dist = round(pow(pow(one_data[0]-particle[p][0],2)+pow(one_data[1]-particle[p][1],2),0.5),2)#每个数据点对质点p的欧式距离
                d.append(dist)    
            distance.append(d)

        for i in range(len(dataset)):#计算数据点i属于哪个类j
            min = distance[0][i]
            dex = 0
            for j in range(k): 
                if distance[j][i] < min:
                    min = distance[j][i]
                    dex = j #得到该数据点与某个质点距离最小时的质点下标（该质点表示的类下标)
            row_of_kinds = kinds[dex]
            row_of_kinds.append(i) #将该数据点序号加入该类
            #row_of_kinds = list(set(row_of_kinds))   


        for ki in range(k): #重新计算质点
            same = 0 #记录质点是否不变，不变则加1
            sum_xx = 0 
            sum_yy = 0
            for kind in kinds[ki]:
                sum_xx += dataset[kind][0]
                sum_yy += dataset[kind][1]
            xx = round(sum_xx/len(kinds[ki]),2) #质点在坐标系x轴的值
            yy = round(sum_yy/len(kinds[ki]),2)
            if particle[ki][0] == xx:
                if particle[ki][1] == yy:
                    same += 1 #若第k个质点坐标不变，same加1
                else:         #否则更新该质点坐标
                    particle[ki][1] = yy
            else:
                particle[ki][0] = xx
                particle[ki][1] = yy

            if same == k:break #若质点都不变，聚类完成，退出循环，否则继续，直到迭代n次结束
    return kinds

if __name__ == "__main__":
    dataset = [[1,1],
                [2,1],
                [1,2],
                [2,2],
                [4,3],
                [5,3],
                [4,4],
                [5,4]]

    kinds = cal_of_kmeans(3,dataset)
    kinds = [list(set(kin)) for kin in kinds ]

    print(kinds)
    for i in range(len(kinds)):
        print("kind no %d contains:"%(i+1))
        for j in range(len(kinds[i])):
            print(kinds[i][j]+1)