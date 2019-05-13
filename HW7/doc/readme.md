# Week7 Report
16340164 吕雪萌
## 实现内容

- 实现方向光源在正交投影下的Shadowing Mapping:
- 场景中有一个object和一个plane
- GUI改变投影方式，光源位置

## 实现效果
- 正交投影
![](https://github.com/lvxm0/CG_homework/blob/master/HW7/doc/1.PNG)
- 更改光源位置
![](https://github.com/lvxm0/CG_homework/blob/master/HW7/doc/2.PNG)
- 更改正交投影的远近
![](https://github.com/lvxm0/CG_homework/blob/master/HW7/doc/3.PNG)
- 透视投影

## shadowing Mapping

![](https://learnopengl.com/img/advanced-lighting/shadow_mapping_theory_spaces.png)

- **Shadow Mapping的主要思路**
以光的位置为视角进行渲染，看到的东西将被照亮，看不见在阴影之中。

我们希望得到射线第一次击中的那个物体，然后用这个最近点和射线上其他点进行对比。然后我们将测试一下看看射线上的其他点是否比最近点更远，如果是的话，这个点就在阴影中。

我们使用深度缓冲来代替射线，从光源的透视图来渲染场景，并把深度值的结果储存到纹理中，对光源的透视图所见的最近的深度值进行采样，最后通过跟深度纹理中的深度值进行比较，可以迅速在渲染物体表面时确定该片元是否在阴影中。

- **Shadow Mapping实现**
