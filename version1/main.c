#include <STC15F2K60S2.h>
#include <stdlib.h>
#include "oled.h"

#define Width 11 //128   //游戏画面尺寸，11是极限值
#define High 	11//64	//由于updateWithoutInput函数中新建了一个同等大小的数组，导致占用大量空间，无法再增加了。除非优化该函数


char cells[Width][High]={0};//全局变量

char array_checkout_flag=0;//数组高度检查标志位

void show(unsigned char begin_x,unsigned char begin_y);
void startup(void);
void updateWithoutInput(void);
void change_array(void);
char checkout_array_high(unsigned char high);
void print_test(void);

int main(void){
	array_checkout_flag=checkout_array_high(High);	//标志位检查并赋检查返回值
	
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	startup();
	while (1)//这是个死循环，所有的细胞动态繁衍或死亡
	{
		
		
		show(0,0);
		delay_ms(10);
		
			updateWithoutInput();

	}
}


void print_test(void){
	unsigned char i,z;
	OLED_Clear();
	
	OLED_Set_Pos(0,7);
	
		for(z=0;z<120;z++){
			//OLED_Set_Pos(z,0);
			//------------像素扫描方向测试
			//OLED_Set_Pos(z,0);
			
			IIC_Start();							//协议格式
			Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
			IIC_Wait_Ack();	
			Write_IIC_Byte(0x40);			//write data
			IIC_Wait_Ack();	
			
			OLED_SCLK_Clr();
			for(i=0;i<8;i++){
				OLED_SDIN_Set();
				OLED_SCLK_Set();
				OLED_SCLK_Clr();
			}
			
			IIC_Wait_Ack();	
			IIC_Stop();	
//---------------------------------------------------
			
			//Write_IIC_Data(0Xff);
			delay_ms(1);
		}
	
}



void change_array(void){
	int i,j;
	/*
	for(i=0;i<Width;i++){
		for(j=0;j<High;j++){
			cells[i][j] = 0;//rand() % 2;
		}
	}*/
	
	for(i=0;i<Width;i++){
		for(j=0;j<High;j++){
			if(j==0 || i==0 || i==Width-1 || j==High-1)
				cells[i][j] = 0;//rand() % 2;
			else
				cells[i][j] = 1;//rand() % 2;
		}
	}
	
	
}

char checkout_array_high(unsigned char high){
//高度处理，高度等于0（不考虑）、小于8、等于8、大于8但小于8的倍数、等于8的倍数
	if(high%8==0){	//是8的整倍数的情况
		return 0;
	}
	else{	//不是8的整数倍情况
		return 1;	
	}
}

void show(unsigned char begin_x,unsigned char begin_y){
	int i,j,k;

	for(k=0;k<(High/8+array_checkout_flag);k++){	//如果是8的整数倍，则不用管；如果不是8的整数倍情况，得多向下扫描一行，以处理余数部分
		
		OLED_Set_Pos(begin_x,begin_y+k);
		
		for(j=0;j<Width;j++){
			
			IIC_Start();							//协议格式
			Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
			IIC_Wait_Ack();	
			Write_IIC_Byte(0x40);			//write data
			IIC_Wait_Ack();	
			OLED_SCLK_Clr();
			
			for(i=0;i<8;i++){	//由于排线在下的状态下，每列像素的扫描方向为从下到上，所以作此修改
			
				if((k*8+7-i)<(High)){	//用于避免越界	//当k=0,i=0时所得值，会与当k=1,i=7时所得值相同，即存在重复打印
					if(cells[j][k*8+7-i]==0){OLED_SDIN_Set();}	//用于始终判断最左边位上的数据，与运算，相同则说明该位为1，设置高电平（亮），否则设置低电平（灭）
					else OLED_SDIN_Clr();
					}
				else OLED_SDIN_Clr();
				
				
				OLED_SCLK_Set();
				OLED_SCLK_Clr();
			}


			IIC_Wait_Ack();	
			IIC_Stop();
			//delay_ms(100);
			
		}
	}
}





void startup(void)//数据初始化
{
    int i, j;
    for (i = 0; i < High; i++)
        for (j = 0; j < Width; j++)
        {
            cells[i][j] = rand() % 2;
        }
}



void updateWithoutInput(void)
{
    unsigned char NewCells[High][Width];
    int NeibourNumber;
    int i, j;
    for (i = 1; i <= High - 1; i++)
    {
        for (j = 0; j < Width; j++)
        {
            NeibourNumber = cells[i - 1][j - 1] + cells[i - 1][j] + cells[i - 1][j + 1] + cells[i][j - 1] + cells[i][j + 1] + cells[i + 1][j - 1] + cells[i + 1][j] + cells[i + 1][j + 1];
            if (NeibourNumber == 3)
                NewCells[i][j] = 1;
            else if (NeibourNumber == 2)
                NewCells[i][j] = cells[i][j];
            else
                NewCells[i][j] = 0;
        }
    }
    for (i = 1; i <= High - 1; i++)
        for (j = 0; j < Width; j++)
            cells[i][j] = NewCells[i][j];
}


