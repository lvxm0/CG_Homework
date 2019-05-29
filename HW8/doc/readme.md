# HW8 Report
16340164 吕雪萌
## 实现效果

## Beizer 曲线定义

贝塞尔曲线 Bézier curve 是计算机图形学中相当重要的参数曲线。 更高维度的广泛化贝塞尔曲线就称作贝兹曲面，其中贝兹三角是一种特殊的实例。

## Beizer 公式
![](https://github.com/lvxm0/CG_homework/blob/master/HW8/doc/01.png)

- 代码实现：
```
//bezier curve 公式
float* Bezier(vector<glm::vec2> point) {
	//beizer curve 点个数
	int num = 0;
	//beizer curve 点坐标
	float* bezierCurve = new float[10000];
	for (float t = 0; t < 1; t += 0.001, num++) {
		// x,y 坐标 = 0
		bezierCurve[num * 2] = 0;
		bezierCurve[num * 2 + 1] = 0;
		for (int i = 0, n = point.size() - 1; i <= n; i++) {
			// 常数部分 C=n!/i!(n-i)!
			float C = jiecheng(n) / (jiecheng(i) * jiecheng(n - i));
			//X
			bezierCurve[num * 2] += C * point[i].x  * pow(1 - t, n - i) * pow(t, i);
			//Y
			bezierCurve[num * 2 + 1] += C * point[i].y  * pow(1 - t, n - i) * pow(t, i);
		}

	}

	return bezierCurve;
}
```
- 自己写阶乘函数
```
// 阶乘
long int jiecheng(int x) {
	if (x == 0) return 1;
	int result = 1;
	for (int i = 1; i <= x; i++) {
		result *= i;
	}
	return result;
}
```

## 
