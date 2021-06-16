#经典青蛙跳台阶
#递归
def fibo(n):
	if n==1 or n==2:
		return n
	return fibo(n-1) + fibo(n-2)
	
if __name__ == "__main__":
	print(fibo(8))  # 输出34

#非递归
def fibo(n):
	if n==1 or n==2:
		return n
	a, b = 1, 2
	for i in range(3,n+1):
		a, b = b, a+b  # 组合进行循环（如：2,3 = 3,5；3,5 = 5,8； ...）
	return b
	
if __name__ == "__main__":
	print(fibo(8))  # 输出34

#高级青蛙跳台阶../javascript/Frog.js