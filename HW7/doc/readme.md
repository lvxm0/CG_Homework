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

### Shadow Mapping 思路
以光的位置为视角进行渲染，看到的东西将被照亮，看不见在阴影之中。

我们希望得到射线第一次击中的那个物体，然后用这个最近点和射线上其他点进行对比。然后我们将测试一下看看射线上的其他点是否比最近点更远，如果是的话，这个点就在阴影中。

我们使用深度缓冲来代替射线，从光源的透视图来渲染场景，并把深度值的结果储存到纹理中，对光源的透视图所见的最近的深度值进行采样，最后通过跟深度纹理中的深度值进行比较，可以迅速在渲染物体表面时确定该片元是否在阴影中。

### Shadow Mapping 实现

- 创建深度贴图
  深度贴图是从光的透视图里渲染的深度纹理，用它计算阴影。因为我们需要将场景的渲染结果储存到一个纹理中，我们将再次需要帧缓冲。

  首先，我们要为渲染的深度贴图创建一个帧缓冲对象：
  ```
  //create depth map FBO
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
  ```
  然后，创建一个2D纹理，提供给帧缓冲的深度缓冲使用：
  ```
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 8192, 8192, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  ```
  把我们把生成的深度纹理作为帧缓冲的深度缓冲：
  ```
    // texture binds with FBO
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  ```
- 光源空间的变换
