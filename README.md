# ContextUI 纯C语言无附加库可移植开源UI

适用于128*64 0.96寸OLED显示屏，稍微修改后可适用于所有尺寸OLED

**纯C语言编写的轻量化UI，无任何附加库，可轻松移植**

*由于目前手头资源限制，使用树莓派PICO进行开发，等完成后会抽象出来适配其他单片机*

---

## lib
| 文件 | 内容 |
| ---- | ---- |
| ContextUI | 核心文件，引用头文件即可编辑UI，包含界面指向，行为逻辑控制等 |
| ContextUI_draw | 图形绘制库 |
| ContextUI_build | 界面与组件构建 |
| ContextUI_library(待完成) | ASCII码对照表，之后会引入中文输入 |
| ContextUI_communication(待完成) | 抽象层的接口，配置该文件中的函数即可适配其他机型 |

## API
(待完成)

