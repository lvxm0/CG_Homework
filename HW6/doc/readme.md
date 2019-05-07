# Report

## 运行效果
![](https://github.com/lvxm0/CG_homework/blob/master/HW6/doc/hw6_1.gif)

## Phong shading
- 求顶点的法向量：在这个顶点相交的面的法向量的均值
- 对顶点应用phong lighting model, 计算顶点光亮度
- 根据顶点对内部的像素进行亮度的线性差值

## Gouraud shading
- 求顶点的法向量：在这个顶点相交的面的法向量的均值
- 根据顶点法向量对内部的点进行法向量的线性差值 
- 对每个像素应用phong lighting model, 计算顶点光亮度

## 改变参数

- ambient
环境光系数，数值越高，物体整体越亮
- diffuse
漫反射系数，数值越高，物体对比度越高
- specular
镜面反射系数，数值对物体明暗影响不太大
- shininess
数值越大，高光面积越小
