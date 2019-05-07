# Report
16340164
吕雪萌
## 运行效果
![](https://github.com/lvxm0/CG_homework/blob/master/HW6/doc/hw6_1.gif)

## Phong shading
- 求顶点的法向量：在这个顶点相交的面的法向量的均值
- 对顶点应用phong lighting model, 计算顶点光亮度
- 根据顶点对内部的像素进行亮度的线性差值
- 计算量大，但更真实

## Gouraud shading
- 求顶点的法向量：在这个顶点相交的面的法向量的均值
- 根据顶点法向量对内部的点进行法向量的线性差值 
- 对每个像素应用phong lighting model, 计算顶点光亮度
- Gouruad 存在明显的对角线轮廓，因为差值不能准确模拟漫反射

## 调整参数
- ambient
环境光系数，数值越高，物体整体越亮
- diffuse
漫反射系数，数值越高，物体对比度越高
- specular
镜面反射系数，数值对物体明暗影响不太大
- shininess
数值越大，高光面积越小

## 实现思路
关键点：
- 实现phong lighting model:phong_lighting =  环境光k * l + 漫反射k * l * dot(n * l) + 镜面反射 k * l * dot(R * V)
- 最终光照值：phong_lighting * Object_color
- Phong shading: 在顶点着色器中计算法向量，在片段着色器中应用phong lighting model
- Gouruad shading: 在顶点着色器中就直接应用phong lighting model，在片段着色器中直接输出
