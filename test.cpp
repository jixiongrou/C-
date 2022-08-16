#include<iostream>
#include<unistd.h>
#include<conio.h>
using namespace std;

void SetCursorPos(int XPos, int YPos)
{
	printf("\033[%d;%dH", YPos+1, XPos+1);
}

int main()
{
	while(1)
		if(kbhit())
			char c = getch();
			cout.flush();
	return 0;
}
