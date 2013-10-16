HGE MFC Demo
============

之前在某游戏公司兼职的时候，需要将其棋牌大厅改为HGE引擎渲染的。该Demo就是这样一个用处。其本身没有任何意义，只是做正规项目之前的一个热身。

> **另：** 该项目中包含一个地图编辑器Demo。

目录说明
--------

> 该Demo的主要参考价值在于 **OUMapMakerEx** 以及 **OUMapDemo**。

### Bin

用于生成最初的测试。

### DemoBin

棋牌大厅 **inner** 窗口的一个Demo。有任务走动，棋牌桌摆放，地图格式，人物简陋 A* 算法。只不过地图的相关贴图算法还有一些纰漏。

### MapMakerBin

地图编辑器。该编辑器地图格式也是Demo状态，只是用于最初的展示，并不能非常好的直接用于项目之中。

### OUAPacker

动画图片打包器代码。格式自定，也是一种比较简陋的格式，仅作Demo用。

### OUMapDemo

棋牌大厅 **inner** 窗口的一个Demo的代码。

### OUMapMaker

几乎可以忽略该项目。

### OUMapMakerEx

地图编辑器代码。

### OUPlazaGUISystem

GUI层封装。比较基础的封装。

### OUPlazaRender

渲染层封装。也是比较基础。

### OUWorld

最初测试代码，非常简陋。

### packbin

动画图片打包可执行文件。

### Res

测试用的一些资源文件。
