#include <iostream.h>
#include <conio.h>
#include <stdio.h>

int persian=0;
int pt[11][11];
int alefba[32][5];
int ins=0;
int x=1,y=1;//x and y of cursor
int a=1,b=21;

void dostate(int stnum);
int getevent();
char coe='f';//Character Of Event
char Getkey(int &c);
void dostate(int state);
void loadpt();
void loadalefba();
void Show();
void Search(int &i,int &j,int m);
void CheckPersianChar(char &C,char Left,char Rigth);

struct stack{
char c[550];
int top;

void init ();
void push (char pushchar);
void Allpush(stack s1);
char pop();
int isfull();
int isempty();
}L,R;



int main(){
clrscr();
loadpt();
loadalefba();
L.init();
R.init();
int cs=0;
int e=getevent();
while( pt[cs][e] >= 0 && pt[cs][e]<= 10  ){
	cs=pt[cs][e];
	dostate(cs);
	Show();
	e=getevent();
}
getch();
return 0;
}

void stack::init(){
top=-1;
}

void stack::push(char pushchar){
if(! isfull() ){
	top++;
	c[top]=pushchar;
}
}

char stack::pop(){
if(! isempty() ){
	top--;
	return c[top+1];
}
else
	return NULL;
}

int stack::isfull(){
	if(top==499)return 1;
	else	return 0;
}

int stack::isempty(){
	if(top==-1)return 1;
	else	return 0;
}

void stack::Allpush(stack s){
    while(!s.isempty())
    push(s.pop());

}

int getevent(){
int sign;// sign of funkey!
coe=Getkey(sign);
	if(!sign){
		 switch(coe){
		 case 8://backspace case
			if(!persian)
				return 1;
			return 10;
		 case 27:// ESC case
			return 6;
		 default:
			if(!persian)
				return 0;
			return 9;
		 }
	}
	else{
		 switch(coe){
		 case 83:		//delete
			return 2;
		 case 77:		//rigth key
			return 3;
		 case 75:		//left  key
			return 4;
		 case 79:               //end
			return 5;
		 case 71:		//home
			return 7;
		 case 59:		//f1 for persian/english
			persian=!persian;
			return 8;
		 }
	}

}

char Getkey(int& c)
{
char I=getch();
if(I==0)
	{
	I=getch();
	c=1;
	}
else
	c=0;
return I;
}

void dostate(int state){
switch(state){
	case 1:
		L.push(coe);		//input Eng state
		CheckPersianChar(R.c[R.top-1],R.c[R.top],R.c[R.top-2]);
		if(L.c[L.top] <0)
		CheckPersianChar(L.c[L.top],L.c[L.top-1],R.c[R.top]);
		if(R.c[R.top] <0)
		CheckPersianChar(R.c[R.top],L.c[L.top],R.c[R.top-1]);
		if(x<22)
		x++;
		break;
	case 2:
		L.pop();		//backspace Eng state
		if(x>a)
		x--;
		break;
	case 3:
		R.pop();
		break;
	case 4:	if(!R.isempty()){	 //rigth state
			L.push(R.pop());
			if(x<b)
			x++;
		}
		break;
	case 5:				//left state
		if(!L.isempty()){
			R.push(L.pop());
			if(x>a)
			x--;
		}
		break;
	case 6:				//end
		while(!R.isempty())
			L.push(R.pop());
		x=b;
		break;
	case 7:				//home
		while(!L.isempty())
			R.push(L.pop());
		x=a;
		break;
	case 8:				//Change language state
		break;
	case 9:				//input farsi state

		CheckPersianChar(coe,L.c[L.top],R.c[R.top]);
		R.push(coe);
		CheckPersianChar(R.c[R.top],L.c[L.top],R.c[R.top-1]);
		if(R.c[R.top-1] <0)
		CheckPersianChar(R.c[R.top-1],R.c[R.top],R.c[R.top-2]);
		if(L.c[L.top] <0)
		CheckPersianChar(L.c[L.top],L.c[L.top-1],R.c[R.top]);
		break;
	case 10:			 //backspace state in farsi mode
		R.pop();
		if(R.c[R.top-1]<0)
		CheckPersianChar(R.c[R.top-1],R.c[R.top],R.c[R.top-2]);
		if(L.c[L.top] <0)
		CheckPersianChar(L.c[L.top],L.c[L.top-1],R.c[R.top]);
		if(R.c[R.top] <0)
		CheckPersianChar(R.c[R.top],L.c[L.top],R.c[R.top-1]);


}
}

void loadpt(){
FILE *F=fopen("Editor.txt","r");
for(int i=0;i<11;i++)
	for(int j=0;j<11;j++)
		fscanf(F,"%d",&pt[i][j]);
fclose(F);
}

void loadalefba(){
FILE *F=fopen("Alefba.txt","r");
for(int i=0;i<32;i++)
	for(int j=0;j<5;j++)
		fscanf(F,"%d",&alefba[i][j]);
fclose(F);
}

void Show(){
clrscr();
int far* scr = (int far *) 0xB8000000;
scr=scr+80;
*scr=0x0f*256+17;
int p=L.top-(x)+1;
int q=R.top-(20-x);
getch();
if(p<0)p=0;
if(q<0)q=0;
int j=1;
for(int i=p;i<=L.top;i++){
	*(scr+j)=0x0f*256+L.c[i];
	j++;
}
for(i=R.top;i>=q;i--){
	*(scr+j)=0x0f*256+R.c[i];
	j++;
}
*(scr+22)=0x0f*256+16;
gotoxy(x+1,2);
//*(scr+x)=0x82db;
/*gotoxy(a,10);
cout<<(char)(17);
int p=L.top-(x-a)+1;
int q=R.top-(b-x)+1;
if(p<0)p=0;
if(q<0)q=0;
for(int i=p;i<=L.top;i++){
	cout<<L.c[i];
}
for(i=R.top;i>=q;i--){
	cout<<R.c[i];
}
gotoxy(b,10);
cout<<" "<<(char)16;
gotoxy(x+1,10);
*/
return ;
}
void Search(int &i,int &j,int m){
for(i=0;i<32;i++)
	for(j=0;j<5;j++)
		if(alefba[i][j]==m)
			return;

}
void CheckPersianChar(char &C,char Left,char Rigth){
int i=0;
int j=0;
int ci=(int)C;
if(ci<0)
	ci=ci+256;


/*for(i=0;i<32;i++){
	for(j=0;j<5;j++){
		if(alefba[i][j]==(int)C){
		break;
		}
	   }}   */
Search(i,j,ci);
	if(i==32){
		return ;
		}
	int LJ=0;
	int RJ=0;
	if(Left>'ç' && Left <= '˛')
		LJ=1;
	int m,n;
	int c=(int)Rigth;
	if(c<0)
		c=c+256;
	Search(m,n,c);
	char p=alefba[m][0];
	switch(p){
			case 'a':
			case 's':
			case 'd':
			case 'f':
			case 'g':
			case 'j':
			case 'k':
			case 'l':
			case ';':
			case 39:
			case 'q':
			case 'w':
			case 'e':
			case 'r':
			case 't':
			case 'y':
			case 'u':
			case 'i':
			case 'o':
			case 'p':
			case '[':
			case ']':
			case 'z':
			case 'x':
				RJ=1;
				break;
			default:
				RJ=0;

		}
		if(RJ==1 && LJ==1)
			C=(char)alefba[i][2];
		if(RJ==1 && LJ==0)
			C=(char)alefba[i][3];
		if(RJ==0 && LJ==1)
			C=(char)alefba[i][1];
		if(RJ==0 && LJ==0)
			C=(char)alefba[i][4];

}
