//汉诺塔问题
void hanoi(int n, String sourceTower, String tempTower, String targetTower) {
        if (n == 1) {
            //如果只有一个盘子1，那么直接将其从sourceTower移动到targetTower
            move(n, sourceTower, targetTower);
        } else {
            //将1号到n-1号盘子由sourceTower经过targetTower移动到tempTower
            hanoi(n - 1, sourceTower, targetTower, tempTower);
            //移动（输出）盘子n由sourceTower移动到targetTower
            move(n, sourceTower, targetTower);
            //把之前移动到tempTower的盘子1号到n-1号，由tempTower经过sourceTower移动到targetTower
            hanoi(n - 1, tempTower, sourceTower, targetTower);
        }
    }

    //盘子n的从sourceTower->targetTower的移动
void move(int n, String sourceTower, String targetTower) {
        std::cout<<"第"<< n <<"号盘子移动:"<<sourceTower<<"--->"<<targetTower<<std::endl;
}
