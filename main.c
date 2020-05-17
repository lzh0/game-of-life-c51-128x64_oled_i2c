#include <STC15F2K60S2.h>
#include <stdlib.h>
#include "oled.h"
//#include "bmp.h"
#define Width 3 //128   //��Ϸ����ߴ�
#define High 3	//64


char cells[Width][High];//ȫ�ֱ���0,0,0,0,1,1,1,1,2
//char cell[Width+9][High+9]={0};
char array_checkout_flag=0;//����߶ȼ���־λ

void show(unsigned char begin_x,unsigned char begin_y);
void startup(void);
void updateWithoutInput(void);
void change_array(void);
char checkout_array_high(unsigned char high);


int main(void){
	unsigned char z;


	array_checkout_flag=checkout_array_high(High);	//��־λ��鲢����鷵��ֵ
	
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear(); 
	change_array();
	
	
	
	show(0,0);//delay_ms(100);
	
	
	
	//	------------����ɨ�跽�����
			OLED_Set_Pos(Width,0);
			IIC_Start();							//Э���ʽ
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
	while (1)//���Ǹ���ѭ�������е�ϸ����̬���ܻ�����
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
//�߶ȴ����߶ȵ���0�������ǣ���С��8������8������8��С��8�ı���������8�ı���
	if(high%8==0){	//��8�������������
		return 0;
	}
	else{	//����8�����������
		return 1;	
	}
}

void show(unsigned char begin_x,unsigned char begin_y){
	int i,j,k;
	
	
	for(k=0;k<(High/8+array_checkout_flag);k++){	//�����8�������������ùܣ��������8��������������ö�����ɨ��һ�У��Դ�����������
		
		OLED_Set_Pos(begin_x,begin_y+k);
		
		for(j=0;j<Width;j++){
			
			IIC_Start();							//Э���ʽ
			Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
			IIC_Wait_Ack();	
			Write_IIC_Byte(0x40);			//write data
			IIC_Wait_Ack();	
			OLED_SCLK_Clr();
			
			for(i=0;i<8;i++){	//�����������µ�״̬�£�ÿ�����ص�ɨ�跽��Ϊ���µ��ϣ����������޸�
			
				/*OLED_SDIN_Set();*/
				
				if((k*8+7-i)<(High)){	//���ڱ���Խ��	//��k=0,i=0ʱ����ֵ�����뵱k=1,i=7ʱ����ֵ��ͬ���������ظ���ӡ
					if(cells[j][k*8+7-i]==0){OLED_SDIN_Set();}	//����ʼ���ж������λ�ϵ����ݣ������㣬��ͬ��˵����λΪ1�����øߵ�ƽ���������������õ͵�ƽ����
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





void startup(void)//���ݳ�ʼ��
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
	OLED_Init();			//��ʼ��OLED  
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
		
		
		IIC_Start();							//Э���ʽ
		Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		IIC_Wait_Ack();	
		Write_IIC_Byte(0x40);			//write data
		IIC_Wait_Ack();	
		
		//Write_IIC_Byte(IIC_Data);	//��д���ݴ��룬���ݺ���Ϊ��Write_IIC_Byte(unsigned char IIC_Byte)������ΪWrite_IIC_Byte�ú�������:
		da=0xff;	//���ݴ��룬���Ʊ���
		OLED_SCLK_Clr();
		for(i=0;i<8;i++){
			
			//OLED_SDIN_Set();	//���ڿ������ص����
			//OLED_SDIN_Clr();	//���ڿ������ص����
			
			m=da;
			//	OLED_SCLK_Clr();
			m=m&0x80;
			if(m==0x80){OLED_SDIN_Set();}	//����ʼ���ж������λ�ϵ����ݣ������㣬��ͬ��˵����λΪ1�����øߵ�ƽ���������������õ͵�ƽ����
			else OLED_SDIN_Clr();
			
			da=da<<1;	 //���ƣ����������λ�ϵ����ݣ��������´��жϣ����Դ���������	
			
			OLED_SCLK_Set();
			OLED_SCLK_Clr();
			
		}
		
		IIC_Wait_Ack();	
		IIC_Stop();
		
		
}
		
	
		

	/*

 
//�ɺ���//OLED_ShowCHinese(0,0,0);���Ƶõ���ĳ���ֵ��ϰ벿����ʾ�������
//���·���main()������
unsigned char x=0,y=0;
unsigned int t=0;
unsigned int i;
unsigned char m,da;
unsigned char code test[][32]={	//"��",0
	{0xff,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00},
	{0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00},
};
//���Ϸ���main()������






	OLED_Set_Pos(x,y);	//��ʾ���ֵ��ϰ벿��
	
	for(t=0;t<16;t++){
		//OLED_WR_Byte(test[2*no][t],1);//�ڶ�������1��ʾд���ݣ�0Ϊд�������Ϊ��Write_IIC_Data(dat);
		//Write_IIC_Data(dat);	//����ΪWrite_IIC_Data(dat);�ĺ�������
		
		IIC_Start();							//Э���ʽ
		Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
		IIC_Wait_Ack();	
		Write_IIC_Byte(0x40);			//write data
		IIC_Wait_Ack();	
		
		//Write_IIC_Byte(IIC_Data);	//��д���ݴ��룬���ݺ���Ϊ��Write_IIC_Byte(unsigned char IIC_Byte)������ΪWrite_IIC_Byte�ú�������:
		da=test[2*no][t];	//���ݴ��룬���Ʊ���
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
	OLED_Set_Pos(x,y+1);	//��ʾ���ֵ��°벿��
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
		OLED_Init();			//��ʼ��OLED  
		OLED_Clear(); OLED_ShowCHinese(0,0,0);//��
		//Delay_1ms(1000);
	 

	 OLED_Init();			//��ʼ��OLED  
		OLED_Clear(); 
		x=64;y=4;
		OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
		{
			//OLED_WR_Byte(test[2*no][t],1);//1Ϊд���ݣ�0Ϊд����
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
		//�����°벿����ʾ�������
		OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
			{
				OLED_WR_Byte(test[2*no+1][t],OLED_DATA);
				adder+=1;
      }
	 


	//�ɺ���//OLED_DrawBMP(0,0,128,8,BMP1);���Ƶõ�
	x0=y0=0;	//���λ��
	x1=128;y1=64;	//��Ļ��С128*64
	
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