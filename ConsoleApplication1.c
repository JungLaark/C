#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <HKMC_AdvancedSeedKey_Client.h>
#include <direct.h>
#include <setjmp.h>

//외부 라이브러리 참조
__declspec(dllexport) ASK_KeyGenerate(const BYTE* seedBuffer, const BYTE* keyBuffer);


int main()
{

	BYTE seedValue[8] = { 0 };
	BYTE keyValue[8] = { 0 };
	int i = 0, j = 0;

	int len1 = 0;
	char* str = " ";
	char* str2 = "0x";
	long value = 0;

	TCHAR szPath[1024] = {0}; // 이 실행파일의 PATH 
	TCHAR szPathBuffer[1024] = {0}; // PATH 의 버퍼 

	char* azPathBufferToChar[1024] = { 0 };
	//txt파일 읽어오기 
	FILE* pFile = NULL;
	unsigned char buffer[20] = { 0 };
	unsigned char changedBuffer[] = { 0 };
	char hex[2] ={"0x"};
	char tempFour[4] = {0};


	GetModuleFileName(NULL, szPath, 1024);//현재 모듈의 절대경로 가지고 오기 //애초의 경로 크기를 알 수 있다면 더 좋을텐데 
	int pathIndex = 0; // "\\"가 마지막으로 있는 인덱스 번호 가지고 옴 

	for (i = 0; i < wcslen(szPath); i++) //버퍼의 전체길이 구해서 for문 돌기 
	{
		if (szPath[i] != '\\')
		{
			szPathBuffer[i+j] = szPath[i];
		}
		else if (szPath[i] == '\\')
		{
			szPathBuffer[i+j] = '\\';
			szPathBuffer[i+1+j] = '\\';
			j++;
			pathIndex = i + 1; //마지막 값이 박힐 것이여 
		}
	}

	szPathBuffer[pathIndex] = 's';
	szPathBuffer[pathIndex+1] = 'e';
	szPathBuffer[pathIndex+2] = 'e';
	szPathBuffer[pathIndex+3] = 'd';
	szPathBuffer[pathIndex+4] = 'k';
	szPathBuffer[pathIndex+5] = 'e';
	szPathBuffer[pathIndex+6] = 'y';
	szPathBuffer[pathIndex+7] = '.';
	szPathBuffer[pathIndex+8] = 't';
	szPathBuffer[pathIndex+9] = 'x';
	szPathBuffer[pathIndex+10] = 't';
	szPathBuffer[pathIndex + 11] = '\0';

	WideCharToMultiByte(CP_ACP, 0, szPathBuffer, 1024, azPathBufferToChar, 1024, NULL, NULL);

	//pFile = fopen(azPathBufferToChar, "r");
	pFile = fopen("seedkey.txt", "r");

	if (pFile != NULL)//한 줄 읽어오기 
	{
		i = 0;
		while (!feof(pFile))
		{
			//한문자씩 
			str = fgetc(pFile);
			if (str != ' ')
			{
				buffer[i] = str;
			}
			i++;
		}
		
		fclose(pFile);
	}

	for (i = 0; i < 8; i++)
	{
		strncpy(tempFour, hex, 2);// 16진수 표기 넣어줌 
		strncpy(tempFour + 2, buffer + i*2, 1); //buffer에 있는 값 가지고 옴 02468
		strncpy(tempFour + 3, buffer + i*2 + 1, 1); //1357

		seedValue[i] = strtol(tempFour, NULL, 16);

		memset(tempFour, 0, sizeof(tempFour));
	}

	ASK_KeyGenerate(seedValue, keyValue);

	pFile = fopen("keyvalue.txt", "w");

	if (pFile != NULL)
	{
		fprintf(pFile, "%X%X%X%X%X%X%X%X", keyValue[0], keyValue[1], keyValue[2], keyValue[3], keyValue[4], keyValue[5], keyValue[6], keyValue[7]);
		fclose(pFile);
		exit;
	}
	return 0;
}






