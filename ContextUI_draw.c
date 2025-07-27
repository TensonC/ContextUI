#include "oled.h"
#include "ContextUI_draw.h"
#include "oled_library.h"
#include "ContextUI.h"


//画面缓冲区
static u8 GRAM[PAGE][SCREEN_W];
//选择框与进度指示缓冲区
static u8 GRAM2[PAGE][SCREEN_W];
static u16 Pow10[] = {
 1,10,100,1000,10000
};

/**
 * @brief 设置光标位置
 * @param x 横坐标
 * @param y 纵坐标
 */
static void OLED_SetCursor(u8 x,u8 y)
{
    OLED_WriteCmd(0xB0 + y);
	OLED_WriteCmd(((x & 0xF0) >> 4) | 0x10);
	OLED_WriteCmd((x & 0x0F) | 0x00);
}

/**
 * @brief 刷新oled屏幕
 */
void CUI_Refresh()
{
    u8 i,j;
    for(i = 0; i < PAGE; i++)
    {
        OLED_SetCursor(0,i);
        for(j = 0; j < SCREEN_W; j++)
        {
            OLED_WriteData(GRAM[i][j]^GRAM2[i][j]);
        }
    }
}

/**
 * @brief 清空oled屏幕
 */
void CUI_Clear()
{
    u8 i,j;
    for(i = 0; i < PAGE; i++)
    {
        for(j = 0; j < SCREEN_W; j++)
        {
            GRAM[i][j] = 0x00;
            GRAM2[i][j] = 0x00;
        }
    }
}


/**
 * @brief 填满oled屏幕
 */
void CUI_Fill()
{
    u8 i,j;
    for(i = 0; i < PAGE; i++)
    {
        for(j = 0; j < SCREEN_W; j++)
        {
            GRAM[i][j] = 0xFF;
        }
    }
}

/**
 * @brief 放置一个8*16的字符
 * @param x 横坐标
 * @param y 纵坐标
 * @param ch 字符
 */
void CUI_PutChar_16(u8 x,u8 y,char ch)
{
    u8 i;
    if (x > SCREEN_W - 8 || y > PAGE - 2) return;
    for(i = 0;i < 8;i++)
    {
        GRAM[y][x] = OLED_F8x16[ch-' '][i];
        GRAM[y+1][x] = OLED_F8x16[ch-' '][i + 8];
        x++;
    }
}

/**
 * @brief 放置一个8*16的字符串
 * @param x 横坐标
 * @param y 纵坐标
 * @param str 字符串
 */
void CUI_PutStr_16(u8 x,u8 y,const char* str)
{
    while (*str)
    {
        CUI_PutChar_16(x,y,*str++);
        x += 8;
    }
}

/**
 * @brief 放置一个8*16的数字(0-65535)
 * @param x 横坐标
 * @param y 纵坐标
 * @param num 数字
 * @param len 数字长度
 */
void CUI_PutNum_16(u8 x,u8 y, u16 num, u8 len)
{
    while(len--)
    {
        CUI_PutChar_16(x,y,num / Pow10[len] % 10 + '0');
        x += 8;
    }
}

/**
 * @brief 绘制一张24*24的图片
 * @param x 横坐标
 * @param y 纵坐标
 * @param pic 图片数组
 */
void CUI_PutPic(u8 x,u8 y,const u8* pic)
{
    u8 i, j;
    u8 k = 0;
    for (i = 0; i < 3; i++)
    {
        if (y + i >= 8) break;
        for (j = 0; j < 24; j++) {
            if (x + j >= 128) break; 
            GRAM[y + i][x + j] = pic[k++];
        }
    }
}

/**
 * @brief 画一个点
 * @param x 横坐标(0-127)
 * @param y 纵坐标(0-63)
 */
void CUI_PutPoint(u8 x,u8 y)
{
    GRAM[y / 8][x] |= 0x01 << (y % 8);
}

/**
 * @brief 清除一个点
 * @param x 横坐标(0-127)
 * @param y 纵坐标(0-63)
 */
void CUI_ClearPoint(u8 x,u8 y)
{
    GRAM[y / 8][x] &= ~(0x01 << (y % 8));
}

/**
 * @brief 画一条直线
 * @param x_s 起始横坐标(0-127)
 * @param y_s 起始纵坐标(0-63)
 * @param x_e 终止横坐标(0-127)
 * @param y_e 终止纵坐标(0-63)
 */
void CUI_PutLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e)
{
    u8 i;
    if(x_s == x_e) {
        for( i = y_s; i <= y_e; i++) {
            CUI_PutPoint(x_s,i);
        }
    }
    else {
        for( i = x_s; i <= x_e; i++) {
            CUI_PutPoint(i,y_s);
        }
    }
}

/**
 * @brief 擦除一条直线
 * @param x_s 起始横坐标(0-127)
 * @param y_s 起始纵坐标(0-63)
 * @param x_e 终止横坐标(0-127)
 * @param y_e 终止纵坐标(0-63)
 */
void CUI_ClearLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e)
{
    u8 i;
    if(x_s == x_e) {
        for( i = y_s; i <= y_e; i++) {
            CUI_ClearPoint(x_s,i);
        }
    }
    else {
        for( i = x_s; i <= x_e; i++) {
            CUI_ClearPoint(i,y_s);
        }
    }
}

/**
 * @brief 画一个条斜线
 * @param x_s 起始横坐标(0-127)
 * @param y_s 起始纵坐标(0-63)
 * @param x_e 终止横坐标(0-127)
 * @param y_e 终止纵坐标(0-63)
 */
void CUI_PutBiasLine(u8 x_s,u8 y_s,u8 x_e,u8 y_e)
{
    u8 lx,ly,dm,i;
    float dx,dy,x,y;
    lx = (x_s > x_e) ? (x_s -x_e) : (x_e - x_s); 
    ly = (y_s > y_e) ? (y_s -y_e) : (y_e - y_s);
    dm = lx > ly ? lx : ly;//取距离中大的那一个做基准
    dx  = (float)(x_e-x_s) / dm;
    dy  = (float)(y_e-y_s) / dm;
    x = (float)x_s + 0.5;
    y = (float)y_s + 0.5;
    for(i = 0; i <= dm; i++)
    {
        CUI_PutPoint(x,y);
        x+=dx;
        y+=dy;
    }
}

/**
 * @brief 画一个矩形
 * @param x1 起始横坐标(0-127)
 * @param y1 起始纵坐标(0-63)
 * @param w 宽
 * @param h 高
 * @param m 空心0 实心1
 */
void CUI_PutRect(u8 x1,u8 y1,u8 w,u8 h,u8 m)
{
    u8 i;
    switch (m)
    {
    case 0:
        CUI_PutLine(x1,y1,x1+w,y1);
        CUI_PutLine(x1,y1,x1,y1+h);
        CUI_PutLine(x1,y1+h,x1+w,y1+h);
        CUI_PutLine(x1+w,y1,x1+w,y1+h);
        break;
    case 1:
        for(i = 0;i <= h;i++) {
            CUI_PutLine(x1,y1+i,x1+w,y1+i);
        }
        break;
    default:
        break;
    }
    
}

/**
 * @brief 擦除一个矩形
 * @param x1 起始横坐标(0-127)
 * @param y1 起始纵坐标(0-63)
 * @param w 宽
 * @param h 高
 * @param m 空心0 实心1
 */
void CUI_ClearRect(u8 x1,u8 y1,u8 w,u8 h,u8 m)
{
    u8 i;
    switch (m)
    {
    case 0:
        CUI_ClearLine(x1,y1,x1+w,y1);
        CUI_ClearLine(x1,y1,x1,y1+h);
        CUI_ClearLine(x1,y1+h,x1+w,y1+h);
        CUI_ClearLine(x1+w,y1,x1+w,y1+h);
        break;
    case 1:
        for(i = 0;i <= h;i++) {
            CUI_ClearLine(x1,y1+i,x1+w,y1+i);
        }
        break;
    default:
        break;
    }
    
}

/**
 * @brief 画一个圆(Bresenham法)
 * @param x0 圆心横坐标(0-127)
 * @param y0 圆心纵坐标(0-63)
 * @param r 半径
 * @param m 空心0 实心1
 */
void CUI_PutCircle(u8 x0,u8 y0,u8 r,u8 m)
{
    u16 x = 0,y = r;
    short d = 3 - 2 * r;
    switch (m)
    {
    case 0:
        while (x <= y) {
            CUI_PutPoint(x0 + x,y0 + y);
            CUI_PutPoint(x0 - x,y0 + y);
            CUI_PutPoint(x0 - x,y0 - y);
            CUI_PutPoint(x0 + x,y0 - y);
            CUI_PutPoint(x0 + y,y0 + x);
            CUI_PutPoint(x0 - y,y0 + x);
            CUI_PutPoint(x0 - y,y0 - x);
            CUI_PutPoint(x0 + y,y0 - x);
            if(d < 0)
                d += 4 * x + 6;
            else{
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
        break;
    case 1:
        while (x <= y) {
            CUI_PutLine(x0 - x,y0 + y,x0 + x,y0 + y);
            CUI_PutLine(x0 - x,y0 - y,x0 + x,y0 - y);
            CUI_PutLine(x0 - y,y0 + x,x0 + y,y0 + x);
            CUI_PutLine(x0 - y,y0 - x,x0 + y,y0 - x);
            if(d < 0)
                d += 4 * x + 6;
            else{
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    default:
        break;
    }
    
}

/**
 * @brief 擦除一个圆(Bresenham法)
 * @param x0 圆心横坐标(0-127)
 * @param y0 圆心纵坐标(0-63)
 * @param r 半径
 * @param m 空心0 实心1
 */
void CUI_ClearCircle(u8 x0,u8 y0,u8 r,u8 m)
{
    u16 x = 0,y = r;
    short d = 3 - 2 * r;
    switch (m)
    {
    case 0:
        while (x <= y) {
            CUI_ClearPoint(x0 + x,y0 + y);
            CUI_ClearPoint(x0 - x,y0 + y);
            CUI_ClearPoint(x0 - x,y0 - y);
            CUI_ClearPoint(x0 + x,y0 - y);
            CUI_ClearPoint(x0 + y,y0 + x);
            CUI_ClearPoint(x0 - y,y0 + x);
            CUI_ClearPoint(x0 - y,y0 - x);
            CUI_ClearPoint(x0 + y,y0 - x);
            if(d < 0)
                d += 4 * x + 6;
            else{
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
        break;
    case 1:
        while (x <= y) {
            CUI_ClearLine(x0 - x,y0 + y,x0 + x,y0 + y);
            CUI_ClearLine(x0 - x,y0 - y,x0 + x,y0 - y);
            CUI_ClearLine(x0 - y,y0 + x,x0 + y,y0 + x);
            CUI_ClearLine(x0 - y,y0 - x,x0 + y,y0 - x);
            if(d < 0)
                d += 4 * x + 6;
            else{
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    default:
        break;
    }
    
}

static void CUI_SelectLine(u8 x,u8 y,u8 len)
{
    u8 i;
    for(i = 0;i <= len;i++)
    {
        if (x + i >= 128) break;
        GRAM2[y / 8][x + i] |= 0x01 << (y % 8);
    }
}

static void CUI_SelectDot(u8 y)
{
    CUI_SelectLine(1,y+7,2);
    CUI_SelectLine(0,y+8,4);
    CUI_SelectLine(0,y+9,4);
    CUI_SelectLine(1,y+10,2);
}

static void CUI_SelectRect(u8 y,u8 len)
{
    u8 i;
    CUI_SelectLine(1,y,len-2);
    CUI_SelectLine(1,y + 15,len-2);
    for(i = 1;i < 15;i++) {
        CUI_SelectLine(0,y + i,len);
    }
}

/**
 * @brief 绘制选择框
 * @param tab 选择框选中的tab
 * @param position 该Tab所在位置 (0-3)
 */
void CUI_DrawSelect(u8 len,u8 position,u8 select_style)
{
    switch (select_style)
    {
    case SELECT_LINE:
        CUI_SelectLine(0,(position * 2 + 1) * PAGE, (len+2) * 8);
        break;
    case SELECT_POINT:
        CUI_SelectDot(position * 2 * PAGE);
        break;
    case SELECT_RECT:
        CUI_SelectRect(position * PAGE * 2, (len + 2) * 8);
        break;
    default:
        break;
    }
}