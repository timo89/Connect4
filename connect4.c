#include <avr/io.h>  
#include <avr/delay.h>
#define F_CPU 1000000UL
#include <avr/interrupt.h>

typedef unsigned char u8;
volatile int i;
volatile int r[8],g[8];


SIGNAL(SIG_OVERFLOW0)
{
	int j;
			
		PORTD = 0;
		PORTA = 0;
		PORTB=1<<(i & 0x07);
		PORTD=r[i & 0x07];
		PORTA=g[i & 0x07];
	i++;
}

/*
unsigned short int m[8][8]={
    {1,2,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,2,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,0},
	{0,0,2,2,1,2,0,0},
	{0,0,0,0,0,1,0,0},
	{0,0,0,0,0,2,1,0}
	
},cx,cy,lx,ly,ch=1;*/


unsigned short int m[8][8]={
    {0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
	
},cx,cy,lx,ly,ch=1;


int checkWin(int x,int y)
{
	short int i,j,count=0,n,ti,tj;
	
	//start l to r
	
	i=x-3;
	n=x+3;
	
	count=0;
	
	if(i<0) i=0;
	if(n>7) n=7;
	
	for(;i<=n;i++)
	{
		if(m[y][i]==ch)
			count++;
		else
			count=0;

		if(count==4)
			return 1;

	}
	//end l to r
	
	
	//start d to u
	j=y-3;
	n=y+3;
	count=0;
	
	if(j<0) j=0;
	if(n>7) n=7;
	
	for(;j<=n;j++)
	{
		if(m[j][x]==ch)
			count++;
		else count=0;
		if(count==4)
			return 1;
	}	
	//end d to u



	//start ld to ru

	i=x;
	j=y;
	
	if(x<3||y<3)
	{
		while(i!=0&&j!=0)
		{
			i--;j--;	
		}
	}
	else
	{
		i-=3;
		j-=3;
	}


	n=i;
	ti=i;
	tj=j;
	if(x>4||y>4)
	{
		while(ti!=7&&tj!=7)
		{
			n++;
			ti++;
			tj++;
		}
	}
	else
	{
		n+=3;
	}
	count=0;
//	printf("\n\n\n");
	for(;i<=n;i++)
	{
	//	printf("<%c>",m[j][i]);
		if(m[j++][i]==ch)
			count++;
		else count=0;
		if(count==4)
			return 1;
	}	

	//end ld to ru
	
	
	
	
	//start lu to rd
	
	i=x;
	j=y;
	
	if(x<3||y>4)
	{
		while(i!=0&&j!=7)
		{
			i--;j++;	
		}
	}
	else
	{
		i-=3;
		j+=3;
	}


	n=i;
	ti=i;
	tj=j;
	if(x>4||y<3)
	{
		while(ti!=7&&tj!=0)
		{
			n++;
			ti++;
			tj--;
		}
	}
	else
	{
		n+=3;
	}
	count=0;
//	printf("\n\n\n%d %d %d \n",i,j,n);
	for(;i<=n;i++)
	{
	//	printf("<%c>",m[j][i]);
		if(m[j--][i]==ch)
			count++;
		else count=0;
		if(count==4)
			return 1;
	}	
	
	//end lu to rd
	
		
//getch();
	return 0;
}





void prelucreaza()
{

	m[ly][lx]=0;
	m[cy][cx]=ch;
	
	lx=cx;
	ly=cy;


}

void reset()
{
	unsigned short int i,j;
	cx=0;
	cy=0;
	ly=cy;
	lx=cx;

	for(i=0;i<8;i++)
	{
		r[i]=0;
		g[i]=0;
		for(j=0;j<8;j++)
			m[i][j]=0;
	}
			
	ch=1;			
	m[ly][cx]=ch;
}

void afiseaza()
{
	int i,j;

	for(i=0;i<8;i++)
	{
		//r[i]=0;
		//g[i]=0;
		for(j=0;j<8;j++)
		{
			if(m[i][j]==1)
				g[i]|=1<<(7-j);
			else if(m[i][j]==2)
				r[i]|=1<<(7-j);
			else
			{
				g[i]&=~(1<<(7-j));
				r[i]&=~(1<<(7-j));
			}
		}
	}	
		
}

void showWin()
{
/*
	unsigned short int i,j;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
			m[i][j]=0;
*/
	g[0]=r[0]=0;
	g[1]=r[1]=0;
	g[2]=r[2]=0;
	g[3]=r[3]=0;
	g[4]=r[4]=0;
	g[5]=r[5]=0;
	g[6]=r[6]=0;
	g[7]=r[7]=0;

	if(ch==1)
	{
		g[2]=36;
		g[4]=126;
		g[5]=66;
		g[6]=60;
	}
	else
	{
		r[2]=36;
		r[4]=126;
		r[5]=66;
		r[6]=60;
	}
	


}





int main()
{

		int i,j;
	int p=0;
	u8 buttonLeft=0,buttonRight=0,buttonDrop=0,buttonReset=0,pressed=0,drop=0,count,win=0;


	
	
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0x00;
	DDRD = 0xFF;

	PORTC= 0XFF;


	TCCR0 |= _BV(CS00);
	TIMSK |= _BV(TOIE0);
	sei();
	
	while(1)
	{

		buttonLeft=(~PINC)&(1<<1);
		buttonRight=(~PINC)&(1<<7);
		buttonDrop=(~PINC)&(1<<6);
		buttonReset=(~PINC)&(1<<0);
	
	//	_delay_ms(100);
		if(!pressed&&!drop&&!win)
		{
			if(buttonLeft)
			{
				if(cx!=0)
					cx--;
			}
			if(buttonRight)
			{
				if(cx!=7)
					cx++;
			}
			if(buttonDrop)
			{
				drop=1;
				count=0;
			}
			if(buttonReset)
			{
				reset();
			}
			prelucreaza();
		}

		if(drop)
		{
	
			count++;
			if(count==10)
			{
				if(cy<7&&m[cy+1][cx]==0)
					cy++;
				else
				{
					if((checkWin(cx,cy))==1)
					{
						showWin();
						win=1;
					}
					else
					{		
						if(ch==1) ch=2; else ch=1;
						lx=ly=cx=cy=0;
					}
					drop=0;
				}
				if(!win)
					prelucreaza();
				count=0;
			}
		//	_delay_ms(100);
		}


		if(!buttonLeft&&!buttonRight&&!buttonDrop&&!buttonReset)
			pressed=0;
		else
			pressed=1;


		
		
		if(win==1)
		{
			_delay_ms(2000);
			win=0;
			count=0;
			reset();
		}
		else
			afiseaza();	

	}
	return 0;
}
