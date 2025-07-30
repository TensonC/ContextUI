# ContextUI 纯C语言无附加库可移植开源UI

适用于128*64 0.96寸OLED显示屏，可修改

**纯C语言编写的轻量化UI，无任何附加库，可轻松移植**

*由于目前手头资源限制，使用树莓派PICO进行开发，等有空会适配其他单片机*

中文显示添加中。。。。。。
动画添加中。。。。。。

---

## lib
| 文件 | 内容 |
| ---- | ---- |
| main | 主函数所在文件 |
| key | 按键检测，可换成任意按键检测，对应特定函数即可 |
| base| 抽象层的接口，配置该文件中的函数即可移植 |
| demo.c | UI演示demo |
| ContextUI | 核心文件，引用头文件即可编辑UI，包含界面指向，行为逻辑控制等 |
| ContextUI_draw | 图形绘制库 |
| ContextUI_build | 界面与组件构建 |
| ContextUI_library | ASCII码对照表 |

---

## 移植

### 底层驱动的移植
在 base.c 中根据具体情况完成
- void SPI_Init(); SPI通信的初始化，或I2C
- void OLED_Init(); OLED的初始化，针对不同芯片，一般情况下无需修改
- void OLED_WriteCmd(u8 cmd); 向OLED发送一字节命令
- void OLED_WriteData(u8 dat); 向OLED发送一字节数据
### 按键的移植
完成四个按钮功能的触发方式检测，包括
- Enter “确认”功能
- Exit “退出”功能
- Leftward “向左”或“向上”翻页功能
- Rightward “向右”或“向下”翻页功能
对于每个按钮功能，只要能够触发 ContextUI.h 中的
```c
void CUI_Enter();
void CUI_Exit();
void CUI_Leftward();
void CUI_Rightward();
```
四个函数即可完成对ui的控制

**至此移植完毕**

---

## 使用

### 绘图函数

| 函数 | 功能 |
| --- | --- |
| CUI_Refresh() | 画面刷新 |
| CUI_Clear() | 缓存区清空，画面不刷新 |
| CUI_Fill() | 填满缓存区 |
| CUI_PutChar_16(u8 x,u8 y,char ch) | 写一个字符(8*16) |
| CUI_PutStr_16(u8 x,u8 y,const char* str) | 写一个字符串(8*16) |
| CUI_PutNum_16(u8 x,u8 y, u16 num, u8 len) | 写一串数字(8*16)(0 - 65535) |
| CUI_PutPoint(u8 x,u8 y) | 画一个点 |
| CUI_ClearPoint(u8 x,u8 y)| 清空一个点 |
| CUI_PutBiasLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e) | 画一条斜线 |
| CUI_PutCircle(u8 x0,u8 y0,u8 r,u8 m) | 画一个圆(m=1为实心，m=0为空心) |
| CUI_PutLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e) | 画一条直线 |
| CUI_PutRect(u8 x1,u8 y1,u8 w,u8 h,u8 m) | 画一个矩形(m=1为实心，m=0为空心) |
| CUI_ClearCircle(u8 x0,u8 y0,u8 r,u8 m) | 清空一个圆形范围(m=1为实心，m=0为空心) |
| CUI_ClearLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e) | 清空一条线(m=1为实心，m=0为空心) |
| CUI_ClearRect(u8 x1,u8 y1,u8 w,u8 h,u8 m) | 清空一个矩形范围(m=1为实心，m=0为空心) |

**记得刷新画面**

### 界面构成
- 一个主菜单，多个应用
- 应用响应函数或初级列表(list)
- 列表中多个列表项(Tab),一个Tab可绑定深一级列表或一个组件(widget)
- 一个组件绑定一个功能函数

主菜单中应用的个数，一个列表中最多Tab数,以及最深列表层数可分别在 ContextUI.h 文件中的
**APP_NUM** **MAX_TAB_IN_LIST** **MAX_LAYER** 进行修改

### 代码组织

#### 应用的构建
创建 CUI_APP MainMenu[APP_NUM] ，对其中每个CUI_APP结构体元素分别传入
- const char* name 名称
- const u8* icon 应用图像(24*24像素大小,采用页寻址方式,因此实际为含72个一字节类型的数组)
- CUI_List* son_list 应用下级列表地址
- void(*respond)()  应用响应函数地址
*列表和响应函数原则上只能存在一个，优先响应函数*
例如
```c
CUI_App MainMenu[APP_NUM] = {
    {"Show",pic1,&show_list,NULL},
    {"ADC",adc_pic,NULL,into_adc},
    {"app3",pic1,NULL,NULL},
    {"app4",pic1,NULL,NULL},
    {"Setting",set_pic,&setting_list,NULL}
};
```
### 列表的构建
创建 CUI_List 结构体对象，分别传入
- const char* name 列表名称
- CUI_Tab* Tabs[] 列表中的列表项(Tab)地址的数组
例如
```c
CUI_List show_list = {
    "show_list",
    {&mult,&check_box,&dot,&str,&num,&per,&page}
};
```
### 列表项的构建
创建 CUI_Tab 结构体对象，传入
- const char* name 列表项的名字，用作Tab标题
- CUI_Widget* link_widget Tab关联的组件的地址
- CUI_List* son_list 关联子列表的地址
例如
```c
CUI_Tab check_box = {
    "Check Box",
    &check,
    NULL
};
```
### 组件的构建
目前支持的组件类型有
```c
enum WIDGET {
    WIDGET_CHECKBOX,    //复选框组件
    WIDGET_DOT,         //复选点组件
    WIDGET_PERCENTBAR,  //进度指示条组件
    WIDGET_NUMBER,      //数字组件
    WIDGET_STR,         //字符串组件
    WIDGET_PAGE         //打开新页面组件
};
```
创建 CUI_Widget 结构体对象，分别传入
- WidgetType widget_type 组件的类型
- const char* name 组件的名称
    在WIDGET_STR类组件中，该元素为要输入的字符串 
- u8 respond_value 组件响应值
    在WIDGET_CHECKBOX和WIDGET_DOT类型组件中，1为选中状态，0为未选中状态，在响应函数中进行更新
    在WIDGET_STR类组件中，该元素为字符串长度
    在WIDGET_PERCENTBAR类组件中为进度(0 - 100)，在响应函数中进行更新
    在WIDGET_NUMBER组件中为数字(0 - 255)
- void(*respond)() 组件的响应函数
    WIDGET_PAGE组件类型较为特殊，传入新页面的开启函数地址
例如
```c
CUI_Widget check = {
    WIDGET_CHECKBOX,
    "check_box",
    0,
    change_check_box
};
```
### 新页面(功能页面)的创建
**页面不可互相嵌套**
在应用或Tab的响应函数中，传入自定义的新页面函数 void new()，新页面函数应包括
```c
CUI_EnterPage(CUI_Page* new_page);
```
这个函数，
创建 CUI_Page 结构体对象，传入
- void(*fun)() 新页面的构建代码，使用ContextUI_draw中的绘制函数
- void(*control[4])() 分别对应原本用来控制ui的enter,exit,left,right四个键,用来控制新页面的行为
在按下exit键后会额外执行退出页面的功能
随后将void new()的地址放入 CUI_App 或 CUI_Widget 的 void(*respond)()中
***注意新页面的程序阻塞可能导致按键无法响应，请保证线程正常***
例如
```c
void fun() {
    CUI_PutStr_16(0,0,"A new page!");
    CUI_Refresh();
}
void enter() {
    CUI_PutStr_16(0,2,"ENTER!");
    CUI_Refresh();
}
void right() {
    CUI_PutStr_16(0,2,"RIGHT!");
    CUI_Refresh();
}
void left() {
    CUI_PutStr_16(0,2,"LEFT!");
    CUI_Refresh();
}
CUI_Page new_page = {fun,{enter,NULL,left,right}};
void open_new_page() {
    CUI_EnterPage(&new_page);   
}
CUI_Widget page_wid = {WIDGET_PAGE,"page",0,open_new_page};
CUI_Tab page = {"Open Page",&page_wid,NULL};
```
### 运行
最后，在main函数中，分别执行spi,oled,key,CUI的初始化后刷新页面即可
```c
int main() {
    SPI_Init();
    OLED_Init();
    key_Init();
    CUI_Init();
    CUI_Refresh();
    while (1) {
        
    }
}
```

**构建例子可查看demo.c文件**