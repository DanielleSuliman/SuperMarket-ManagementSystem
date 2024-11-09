#pragma once
typedef unsigned char BYTE;

int		writeStringToFile(const char* str, FILE* fp,const char* msg, int chose);
int		writeCharsToFile(const char* arr,int size, FILE* fp, const char* msg);
int		writeIntToFile(int val, FILE* fp, const char* msg);

char*	readStringFromFile(FILE* fp,const char* msg,int chose);
char*	readDynStringFromTextFile(FILE* fp);
int		readFixSizeStrFromFile(char* arr, FILE* fp, const char* msg);
int		readCharsFromFile(char* arr, int size, FILE* fp, const char* msg);
int		readIntFromFile(int* val, FILE* fp, const char* msg);
//////////////////////////////////compress///////////////////////////////////////
int	writeProductCountANDnameLenToCompress(int productCount, int superNameLength,FILE* compressFileName);
int	writeAdressNameSuperToCompress(int numberStreet,char* street,char* city, FILE* compressFileName);
int writeProductToCompress(char* pBarcode, char* pName, int type, FILE* compressFileName,int count,float price);
void ConverseFormat(char* check); 
void  reversConverseFormat(char* check);


int readProductCountANDnameLenFromCompress(int* namLen, int*countProduct, FILE* compressFileName);


