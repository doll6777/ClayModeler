#include <stdio.h>


int main()
{
	int T = 0;
	char calChar;
	char *userString[2];
	int resultNum = 0;

	scanf("%d", &T);
	
	for(int j=0; j<T; j++)
	{
		userString[j] = new char[4];
		scanf("%s", userString[j]);
	}


	for(int i=0; i<T; i++)
	{
	

	//Cal
	switch(userString[i][0])
	{
	case '+':
		{
			resultNum = (userString[i][1]-48)+(userString[i][2]-48);
			printf("%d\n", resultNum);
			break;
		}

	case '-':
		{
			resultNum = ((userString[i][1]-48)-(userString[i][2]-48));
			printf("%d\n", resultNum);
						break;			
		}

	case '*':
		{
			resultNum = ((userString[i][1]-48)*(userString[i][2]-48));	
			printf("%d\n", resultNum);

						break;
		}
	case '/':
		{
			resultNum = (userString[i][1]-48)/(userString[i][2]-48);
			printf("%d\n", resultNum);
			if(userString==0) printf("Divide by zero");
						break;
		}
		
	}
	}


	return 0;
}

