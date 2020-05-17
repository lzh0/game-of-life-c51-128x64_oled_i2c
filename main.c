#include <STC15F2K60S2.h>
#include <stdlib.h>
#include "oled.h"
//#include "bmp.h"
#define Width 3 //128   //游戏画面尺寸
#define High 3	//64


char cells[Width][High];//全局变量0,0,0,0,1,1,1,1,2
//char cell[Width+9][High+9]={0};
char array_checkout_flag=0;//数组高度检查标志位

void show(unsigned char begin_x,unsigned char begin_y);
void startup(void);
void updateWithoutInput(void);
void change_array(void);
char checkout_array_high(unsigned char high);


int main(void){
	unsigned char z;


	array_checkout_flag=checkout_array_high(High);	//标志位检查并赋检查返回值
	
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	change_array();
	
	
	
	show(0,0);//delay_ms(100);
	
	
	
	//	------------像素扫描方向测试
			OLED_Set_Pos(Width,0);
			IIC_Start();							//协议格式
			Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
			IIC_Wait_Ack();	
			Write_IIC_Byte(0x40);			//write data
			IIC_Wait_Ack();	
			OLED_SCLK_Clr();
			
			for(z=0;z<8;z++){OLED_SDIN_Set();OLED_SCLK_Set();OLED_SCLK_Clr();}
			IIC_Wait_Ack();	
			IIC_Stop();	

//---------------------------------------------------
	
	//startup();
	while (1)//这是个死循环，所有的细胞动态繁衍或死亡
	{
		
		//show(0,0);delay_ms(100);
		
		//OLED_Set_Pos(0,3);
		Write_IIC_Byte(0Xff);
		delay_ms(100);
			OLED_Clear();
		//
		//OLED_Clear(); 
		
		
			//updateWithoutInput();

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
			
				/*OLED_SDIN_Set();*/
				
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






























	/*
	
unsigned int j=0;
unsigned char x,y;
unsigned char x0,y0,x1,y1;
	
	
void main(void){
	OLED_Init();			//初始化OLED  
	OLED_Clear(); 
	OLED_Set_Pos(0,0);

	//OLED_DrawBMP(0,0,128,8,BMP1);
	

	for(i=0;i<=16;i++){
		OLED_WR_Byte(t++,1);
		//delay_ms(50);
	}
	
	OLED_Set_Pos(0,1);
	for(i=0;i<=256;i++){
		OLED_WR_Byte(i,1);
	}	
	
  OLED_Set_Pos(0,2);
	OLED_WR_Byte(0,1);
	OLED_WR_Byte(0,1);
	OLED_WR_Byte(0xff,1);

	
	
	for(t=0;t<=255;t++){

		
		
		//OLED_WR_Byte(t,1);
		//delay_ms(25);
		
		
		IIC_Start();							//协议格式
		Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		IIC_Wait_Ack();	
		Write_IIC_Byte(0x40);			//write data
		IIC_Wait_Ack();	
		
		//Write_IIC_Byte(IIC_Data);	//所写数据传入，内容函数为：Write_IIC_Byte(unsigned char IIC_Byte)，以下为Write_IIC_Byte该函数内容:
		da=0xff;	//数据传入，复制备份
		OLED_SCLK_Clr();
		for(i=0;i<8;i++){
			
			//OLED_SDIN_Set();	//用于控制像素点的亮
			//OLED_SDIN_Clr();	//用于控制像素点的灭
			
			m=da;
			//	OLED_SCLK_Clr();
			m=m&0x80;
			if(m==0x80){OLED_SDIN_Set();}	//用于始终判断最左边位上的数据，与运算，相同则说明该位为1，设置高电平（亮），否则设置低电平（灭）
			else OLED_SDIN_Clr();
			
			da=da<<1;	 //左移，更新最左侧位上的数据，以用于下次判断，并以此设置亮灭。	
			
			OLED_SCLK_Set();
			OLED_SCLK_Clr();
			
		}
		
		IIC_Wait_Ack();	
		IIC_Stop();
		
		
}
		
	
		

	/*

 
//由函数//OLED_ShowCHinese(0,0,0);逆推得到：某汉字的上半部分显示所需语句
//以下放于main()函数外
unsigned char x=0,y=0;
unsigned int t=0;
unsigned int i;
unsigned char m,da;
unsigned char code test[][32]={	//"中",0
	{0xff,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00},
	{0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00},
};
//以上放于main()函数外






	OLED_Set_Pos(x,y);	//显示汉字的上半部分
	
	for(t=0;t<16;t++){
		//OLED_WR_Byte(test[2*no][t],1);//第二个参数1表示写数据，0为写命令，内容为：Write_IIC_Data(dat);
		//Write_IIC_Data(dat);	//以下为Write_IIC_Data(dat);的函数内容
		
		IIC_Start();							//协议格式
		Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		IIC_Wait_Ack();	
		Write_IIC_Byte(0x40);			//write data
		IIC_Wait_Ack();	
		
		//Write_IIC_Byte(IIC_Data);	//所写数据传入，内容函数为：Write_IIC_Byte(unsigned char IIC_Byte)，以下为Write_IIC_Byte该函数内容:
		da=test[2*no][t];	//数据传入，复制备份
		OLED_SCLK_Clr();
		for(i=0;i<8;i++){
			m=da;
			//	OLED_SCLK_Clr();
			m=m&0x80;
			if(m==0x80){OLED_SDIN_Set();}
			else OLED_SDIN_Clr();
			da=da<<1;
			OLED_SCLK_Set();
			OLED_SCLK_Clr();
		}
		
		
		
		
		IIC_Wait_Ack();	
		IIC_Stop();

	}	
	//--------------------------------------------------------------------------
	OLED_Set_Pos(x,y+1);	//显示汉字的下半部分
	for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(test[2*no+1][t],1);
	}

	while(1){}
	
}

*/




















/*
 int main(void)
 {
		OLED_Init();			//初始化OLED  
		OLED_Clear(); OLED_ShowCHinese(0,0,0);//中
		//Delay_1ms(1000);
	 

	 OLED_Init();			//初始化OLED  
		OLED_Clear(); 
		x=64;y=4;
		OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
			//OLED_WR_Byte(test[2*no][t],1);//1为写数据，0为写命令
			//Write_IIC_Data(test[2*no][t]);
			//Write_IIC_Byte(test[2*no][t]);
			
			IIC_Start();
			Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
			IIC_Wait_Ack();
			Write_IIC_Byte(0x40);			//write data
			IIC_Wait_Ack();
			
			da=(test[2*no][t]);
			OLED_SCL=0;
			for(i=0;i<8;i++)
			{
				m=da;
				//	OLED_SCL=0;
				m=m&0x80;
				if(m==0x80){OLED_SDIN=1;}
				else OLED_SDIN=0;
				da=da<<1;
				OLED_SCL=1;
				OLED_SCL=0;
			}
			IIC_Wait_Ack();
			IIC_Stop();
			
				//adder+=1;
     }
		//汉字下半部分显示所需语句
		OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
			{
				OLED_WR_Byte(test[2*no+1][t],OLED_DATA);
				adder+=1;
      }
	 


	//由函数//OLED_DrawBMP(0,0,128,8,BMP1);逆推得到
	x0=y0=0;	//起点位置
	x1=128;y1=64;	//屏幕大小128*64
	
	if(y1%8==0) y=y1/8;
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		if(j>0xfd){j=0;}
    for(x=x0;x<x1;x++){
			
		//OLED_WR_Byte(BMP[j++],OLED_DATA);
		//OLED_WR_Byte(unsigned dat,1);
		//Write_IIC_Data(dat);
		IIC_Start();
    Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40);			//write data
    IIC_Wait_Ack();
			
    //Write_IIC_Byte(IIC_Data);
		unsigned char i;
		unsigned char m,da;
		da=IIC_Byte;
		OLED_SCL=0;
		for(i=0;i<8;i++)
		{
			m=da;
			//	OLED_SCL=0;
			m=m&0x80;
			
			if(m==0x80){OLED_SDIN=1;}
			else OLED_SDIN=0;
			
			da=da<<1;
			OLED_SCL=1;
			OLED_SCL=0;
		}
			
    IIC_Wait_Ack();
    IIC_Stop();
			
			
		}
	}
}
	*/