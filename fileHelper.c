#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileHelper.h"
#include "General.h"
#include <math.h>



int	 writeStringToFile(const char* str, FILE* fp,const char* msg,int chose)
{
	int length = (int)strlen(str);

	if (!chose )//Not Compress
		length+= 1;
	
	if (!writeIntToFile(length, fp,msg))
		return 0;

	if (!writeCharsToFile(str, length, fp, msg))
		return 0;


	return 1;
}

int	 writeCharsToFile(const char* arr, int size, FILE* fp, const char* msg)
{
	if (fwrite(arr, sizeof(char), size, fp) != size)
	{
		puts(msg);
		return 0;
	}
	return 1;

}

int	 writeIntToFile(int val, FILE* fp, const char* msg)
{
	if (fwrite(&val, sizeof(int), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

char* readStringFromFile(FILE* fp, const char* msg,int chose)
{
	char* str;
	int length;
	


	if (!readIntFromFile(&length, fp,msg))
		return NULL;


	if(chose)
		length+= 1;

	str = (char*)malloc((length) * sizeof(char));
	if (!str)
	{
		puts(msg);
		return NULL;
	}
	if (chose)
		length-= 1;

	if (fread(str, sizeof(char), length, fp) != length)
	{
		free(str);
		puts(msg);
		return NULL;
	}
	return str;
}

int readFixSizeStrFromFile(char* arr,FILE* fp, const char* msg)
{
	int len;
	if (!readIntFromFile(&len, fp, msg))
		return 0;
	
	if (!readCharsFromFile(arr, len, fp, msg))
		return 0;

	return 1;
}

int	 readCharsFromFile(char* arr, int length, FILE* fp, const char* msg)
{
	if (fread(arr, sizeof(char), length, fp) != length)
	{
		puts(msg);
		return 0;
	}
	return 1;
}

int	  readIntFromFile(int* pVal, FILE* fp, const char* msg)
{
	if (fread(pVal, sizeof(int), 1, fp) != 1)
	{
		puts(msg);
		return 0;
	}
	return 1;

}
char* readDynStringFromTextFile(FILE* fp)
{
	char temp[MAX_STR_LEN];
	myGets(temp, sizeof(temp), fp);
	return getDynStr(temp);
}
//////////////////////////////////////////comprss///////////////////////////////////
int writeProductCountANDnameLenToCompress(int productCount, int superNameLength, FILE* compressFileName)
{
	BYTE bytes[2];
	bytes[0] = productCount >> 2;
	bytes[1] = (productCount & 0xF003)<<6;
	bytes[1] = bytes[1] | superNameLength;
	if (fwrite(bytes, sizeof(BYTE), 2, compressFileName) != 2)
		return 0;


	return 1;
}


int	writeAdressNameSuperToCompress(int numberStreet, char* street, char* city, FILE* compressFileName) {
	BYTE NumbeS = numberStreet;

	if (!fwrite(&NumbeS, sizeof(BYTE), 1, compressFileName))
		return 0;
	if (!writeStringToFile(street, compressFileName, "erro write Street",1))
		return 0;
	if (!writeStringToFile(city, compressFileName, "erro write City",1))
		return 0;
	

	return 1;

}
int writeProductToCompress(char* pBarcode, char* pName, int type, FILE* compressFileName,int count ,float price) {
	BYTE part[6];

	char* barcodeConverse = _strdup(pBarcode);
	char* run = barcodeConverse;
	while (*run != '\0')
	{
		ConverseFormat(run);
		run++;
	}
	part[0] = (barcodeConverse[0]<<2)| (barcodeConverse[1] >> 4);
	part[1] = ((barcodeConverse[1] & 0xF0F) << 4) | (barcodeConverse[2] >> 2);
	part[2] = (barcodeConverse[2] << 6) | (barcodeConverse[3] );
	part[3]= (barcodeConverse[4] << 2) | (barcodeConverse[5] >> 4);
	part[4]= ((barcodeConverse[5] & 0xF0F) << 4) | (barcodeConverse[6] >> 2);
	int nameLen = (int)strlen(pName);
	part[5] = (barcodeConverse[6] << 6) | (nameLen<<2) | (type);

	free(barcodeConverse);
	if (!fwrite(part, sizeof(BYTE) * 6, 1, compressFileName)) 
		return 0;

	
	if(!writeCharsToFile(pName, nameLen, compressFileName, "Error  supermarket compress file to write product name  \n"))
		return 0;
	
	BYTE payment[3];
	payment[0] = count;
	int  fullprice = (int)price;
	int leftprice =(int)((price-fullprice)*100) ;
	
	payment[1] = (leftprice <<1) | (fullprice >> 8);
	payment[2] = (fullprice&0xff);
	if (!fwrite(payment, sizeof(BYTE)*3, 1, compressFileName))
		return 0;

	return 1;
}

void   ConverseFormat(char* check) {
	char ch = *check;

	if (ch >= 'A' && ch <= 'Z') {
		ch = ch - '7';
		*check = ch;
	}

	if (ch >= '0' && ch <= '9') {
		ch = ch - '0';
		*check = ch;

	}
}
	void   reversConverseFormat(char* check) {
		int ch = (int)(*check);

		if (ch >= 10 && ch <= 35) {//ALPA
			ch = ch +55;
			*check = (char)ch;
		}

		if (ch >= 0 && ch <= 9) {//DIGIT
			ch = ch + 48;
			*check = (char)ch;

		}
	
	}

int readProductCountANDnameLenFromCompress(int* namLen, int* countProduct, FILE* compressFileName) {
	BYTE data[2];
	if (fread(data, sizeof(BYTE), 2, compressFileName) != 2)
		return 0;

	 *countProduct = (data[0] << 2) | (data[1] >> 6);
	 *namLen = (data[1] & 0x3F);


	return 1;
}
