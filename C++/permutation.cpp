//dfs实现全排列
void dfs(char* perm, int from, int to)
{
	if (to <= 1)
	{
		return;
	}

	if (from == to)
	{
		for (int i = 0; i <= to; i++)
			cout << perm[i];
		cout << endl;
	}
	else
	{
		for (int j = from; j <= to; j++)
		{
			swap(perm[j], perm[from]); //交换
			dfs(perm, from + 1, to);
			swap(perm[j], perm[from]);  //交换回来
		}
	}
}


