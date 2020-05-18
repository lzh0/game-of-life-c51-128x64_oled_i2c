#include <STC15F2K60S2.h>
#include <stdlib.h>
#include "oled.h"

#define Width 11 //128   //��Ϸ����ߴ磬11�Ǽ���ֵ
#define High 	11//64	//����updateWithoutInput�������½���һ��ͬ�ȴ�С�����飬����ռ�ô����ռ䣬�޷��������ˡ������Ż��ú���


char cells[Width][High]={0};//ȫ�ֱ���

char array_checkout_flag=0;//����߶ȼ���־λ

void show(unsigned char begin_x,unsigned char begin_y);
void startup(void);
void updateWithoutInput(void);
void change_array(void);
char checkout_array_high(unsigned char high);
void print_test(void);

int main(void){
	array_checkout_flag=checkout_array_high(High);	//��־λ��鲢����鷵��ֵ
	
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear(); 
	startup();
	while (1)//���Ǹ���ѭ�������е�ϸ����̬���ܻ�����
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
			//------------����ɨ�跽�����
			//OLED_Set_Pos(z,0);
			
			IIC_Start();							//Э���ʽ
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


