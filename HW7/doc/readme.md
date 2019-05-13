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

## shadowing Mapping

![](https://learnopengl.com/img/advanced-lighting/shadow_mapping_theory_spaces.png)

### Shadow Mapping 思路
- 以光的位置为视角进行渲染，看到的东西将被照亮，看不见在阴影之中。

- 我们希望得到射线第一次击中的那个物体，然后用这个最近点和射线上其他点进行对比。然后我们将测试一下看看射线上的其他点是否比最近点更远，如果是的话，这个点就在阴影中。

- 我们使用深度缓冲来代替射线，从光源的透视图来渲染场景，并把深度值的结果储存到纹理中，对光源的透视图所见的最近的深度值进行采样，最后通过跟深度纹理中的深度值进行比较，可以迅速在渲染物体表面时确定该片元是否在阴影中。

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
  把我们把生成的深度纹理作为帧缓冲的深度缓冲：(深度缓冲中的深度值都是离观察点“最近”的深度)
  ```
    // texture binds with FBO
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  ```
- 渲染到深度贴图
  

  变换到光源视角需要两个变换矩阵，一个是光源坐标系下的View矩阵，通过lookAt来生成,另一个是光源视角下的投影矩阵，与普通的投影矩阵相同，可以使用正交和透视两种。
  ```
  if (radio == 0) {
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
      }
      else {
        lightProjection = glm::perspective(glm::radians(45.0f), 1.0f, near_plane, far_plane);
      }
  lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
  ```
  变换矩阵通过两个矩阵相乘得到，传入渲染深度贴图的着色器中用于顶点坐标变换：
  ```
  lightSpaceMatrix = lightProjection * lightView;
  ```
  深度贴图的渲染着色器depthShader实现把顶点变换到光空间
  ```
  #version 330 core
  layout (location = 0) in vec3 aPos;

  uniform mat4 model;
  uniform mat4 lightSpaceMatrix;

  void main() { gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0); }
  ```
  空像素着色器
  ```
  #version 330 core
  void main(){}
  ```
  
- 渲染阴影

  生成阴影在像素着色器中执行，用来检验一个片元是否在阴影之中，不过我们在顶点着色器中进行光空间的变换：
  ```
  #version 330 core
  layout (location = 0) in vec3 aPos;
  layout (location = 1) in vec3 aNormal;
  layout (location = 2) in vec2 aTexCoords;

  out vec2 TexCoords;

  out VS_OUT {
      vec3 FragPos;
      vec3 Normal;
      vec2 TexCoords;
      vec4 FragPosLightSpace;
  } vs_out;

  uniform mat4 projection;
  uniform mat4 view;
  uniform mat4 model;
  uniform mat4 lightSpaceMatrix;

  void main()
  {
      vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
      vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
      vs_out.TexCoords = aTexCoords;
      vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
      gl_Position = projection * view * model * vec4(aPos, 1.0);
  }
  ```
  像素着色器使用Blinn-Phong光照模型渲染场景。我们接着计算出一个shadow值:
  ```
  #version 330 core
  out vec4 FragColor;

  in VS_OUT {
      vec3 FragPos;
      vec3 Normal;
      vec2 TexCoords;
      vec4 FragPosLightSpace;
  } vs_out;

  uniform vec3 lightPos;
  uniform vec3 viewPos;
  uniform vec3 objectColor;

  uniform sampler2D diffuseTexture;
  uniform sampler2D shadowMap;

  float ShadowCalculation(vec4 fragPosLightSpace) {
      vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
      projCoords = projCoords * 0.5 + 0.5;
      float closestDepth = texture(shadowMap, projCoords.xy).r; 
      float currentDepth = projCoords.z;
      vec3 normal = normalize(vs_out.Normal);
      vec3 lightDir = normalize(lightPos - vs_out.FragPos);
      float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
      float shadow = 0.0;
      vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
      for(int x = -1; x <= 1; ++x) {
          for(int y = -1; y <= 1; ++y) {
              float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
              shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
          }    
      }
      shadow /= 9.0;
      if(projCoords.z > 1.0)
          shadow = 0.0;

      return shadow;
  }
  void main()
  {           
      vec3 color = objectColor;
      vec3 normal = normalize(vs_out.Normal);
      vec3 lightColor = vec3(0.3);
      // ambient
      vec3 ambient = 0.6 * color;
      // diffuse
      vec3 lightDir = normalize(lightPos - vs_out.FragPos);
      float diff = max(dot(lightDir, normal), 0.0);
      vec3 diffuse = diff * lightColor;
      // specular
      vec3 viewDir = normalize(viewPos - vs_out.FragPos);
      vec3 reflectDir = reflect(-lightDir, normal);
      float spec = 0.0;
      vec3 halfwayDir = normalize(lightDir + viewDir);  
      spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
      vec3 specular = spec * lightColor;    
      // calculate shadow
      float shadow = ShadowCalculation(vs_out.FragPosLightSpace);                      
      vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
      FragColor = vec4(lighting, 1.0);
  }
  ```
