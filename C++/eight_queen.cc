#include <iostream>

using namespace std;
const int N = 20;
int n;
char g[N][N]; //存放题目所需的棋盘
bool col[N],dg[N],udg[N];//记录当前列，当前正对角线，当前反对角线的状态是探测过与否


void dfs(int u)
{
    //到达递归结束的边界 输出当前放置好皇后的地图
  if(u == n) 
  {
      for(int i = 0;i < n;i++) puts(g[i]);
      puts("");
      return;//不要忘记return,否则会无休止的运行
  }
   
   //尝试每一种可能
  for(int i = 0;i < n;i++)
  {
      // 判断横行u和竖列i组成的坐标(u,i) 在这列，在这正对角线，在这反对角线中是否放置了皇后
      // 正对角线的方程为i = -1 * u + b,则b = u+i
      //  反对角线的方程为i = 1 * u + b,则b = i-u,因为要作为数组的参数，不能为负数，所以考虑加一个偏移量n，保证参数为正。
      if(!col[i] && !dg[u+i] && !udg[n-u+i])
      {
          g[u][i] = 'Q';
          col[i] = dg[u+i] = udg[n-u+i] = true;//标记这个放过皇后的点
          dfs(u+1);//继续到下一个位置
          col[i] = dg[u+i] = udg[n-u+i] = false;//回溯回来的时候，恢复现场
          g[u][i] = '.';
      }
  }
}

int main()
{
    cin >> n;
    
    for(int i = 0;i < n;i++)
        for(int j = 0; j < n;j++) g[i][j] = '.';
    
    dfs(0);
    
    return 0;
}

