#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Address.h"
#include "General.h"
#include "fileHelper.h"
#include "SuperFile.h"
#include "myMacros.h"
#include "SuperFile.h"



int	saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
	const char* customersFileName, int isCompress)
{
	if (isCompress == 1) {
		saveSuperMarketToFileCompressed(pMarket, fileName, customersFileName);
		return 1;
	}
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp) {
		printf("Error open supermarket file to write\n");
		return 0;
	}

	if (!writeStringToFile(pMarket->name, fp, "Error write supermarket name\n",0))
	{
		CLOSE_RETURN_0(fp);
	}
		

	if (!saveAddressToFile(&pMarket->location, fp))
	{
		fclose(fp);
		return 0;
	}
	int count = getNumOfProductsInList(pMarket);

	if (!writeIntToFile(count, fp, "Error write product count\n"))
	{
		CLOSE_RETURN_0(fp);
	}

	Product* pTemp;
	NODE* pN = pMarket->productList.head.next; //first Node
	while (pN != NULL)
	{
		pTemp = (Product*)pN->key;
		if (!saveProductToFile(pTemp, fp))
		{
			CLOSE_RETURN_0(fp);
		}
		pN = pN->next;
	}

	fclose(fp);

	saveCustomerToTextFile(pMarket->customerArr,pMarket->customerCount, customersFileName);

	return 1;
}
/////////////////////////////compress////////////////////////////////////////////////////////////////////////
int	saveSuperMarketToFileCompressed(const SuperMarket* pMarket, const char* fileName, const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");
	CHECK_MSG_RETURN_0(fp, "failed opening file.")

		int count = getNumOfProductsInList(pMarket);
	int len = (int)strlen(pMarket->name);
	if (!writeProductCountANDnameLenToCompress(count, len, fp)) {

		printf("Error  supermarket compress file to write name len and product count\n");
		fclose(fp);
		return 0;
	}

	if (!writeCharsToFile(pMarket->name, len, fp, "Error supermarket compress file to write name sp  \n")) {
		CLOSE_RETURN_0(fp);
	}

	if (!writeAdressNameSuperToCompress(pMarket->location.num, pMarket->location.street, pMarket->location.city, fp)) {
		printf("Error  supermarket compress file to write name  \n");
		CLOSE_RETURN_0(fp);
	}
	int NumOfProductsInList = getNumOfProductsInList(pMarket);
	NODE* pN = pMarket->productList.head.next; //first Node
	for (pN; pN != NULL; pN = pN->next)
	{
		Product* product = (Product*)pN->key;
		int type = (int)product->type;
		if (!writeProductToCompress(product->barcode, product->name, type, fp, product->count, product->price)) {
			printf("Error  supermarket compress file to write product  \n");
			fclose(fp);
			return 0;
		}

	}
	fclose(fp);
	saveCustomerToTextFile(pMarket->customerArr, pMarket->customerCount, customersFileName);
	printf("succsessfuly saved compressed file!");
	return 1;
}

#define BIN
#ifdef BIN
int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}

	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n",0);
	if (!pMarket->name)
	{
		fclose(fp);
		return 0;
	}

	if (!loadAddressFromFile(&pMarket->location, fp))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);
		
	}

	int count;
	if (!readIntFromFile(&count, fp, "Error reading product count\n"))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
	}

	if (count > 0)
	{
		Product* pTemp;
		for (int i = 0; i < count; i++)
		{
			pTemp = (Product*)calloc(1, sizeof(Product));
			if (!pTemp)
			{
				printf("Allocation error\n");
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			if (!loadProductFromFile(pTemp, fp))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			if (!insertNewProductToList(&pMarket->productList, pTemp))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
		}
	}


	fclose(fp);

	pMarket->customerArr = loadCustomerFromTextFile(customersFileName,&pMarket->customerCount);
	if (!pMarket->customerArr)
		return 0;

	return	1;

}
int	loadSuperMarketFromCompressFile(SuperMarket* pMarket, const char* fileName,const char* customersFileName) {

	FILE* fp;
	fp = fopen(fileName, "rb");
	CHECK_MSG_RETURN_0(fp, "Falied to open file.");
	int countProduct;
	int namLen;
	if (!readProductCountANDnameLenFromCompress(&namLen, &countProduct, fp)) {
		printf("Error read company Count AND nameLen from Compress file\n");
		fclose(fp);
		return 0;
		}
	pMarket->name = (char*)malloc(sizeof(char)*(namLen + 1));

	if (!readCharsFromFile(pMarket->name, namLen, fp, "Error read company name from Compress file\n")) {
		FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);
	}
	if (!loadAddressFromCompressFile(&pMarket->location, fp))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
	}
	if (countProduct > 0)
	{
		Product* pTemp;
		L_init(&pMarket->productList);
		for (int i = 0; i < countProduct; i++)
		{
			pTemp = (Product*)calloc(1, sizeof(Product));
			if (!pTemp)
			{
				printf("Allocation error\n");
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;

			}
			
			if (!loadProductFromCopressFile(pTemp,pTemp->barcode, fp))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			printProduct(pTemp);
			
			if (!insertNewProductToList(&pMarket->productList, pTemp))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
		}

	}

	
	fclose(fp);
	pMarket->customerArr = loadCustomerFromTextFile(customersFileName, &pMarket->customerCount);
	if (!pMarket->customerArr)
		return 0;

		return 1;
	}


#else
int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}

	//L_init(&pMarket->productList);


	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n");
	if (!pMarket->name)
	{
		fclose(fp);
		return 0;
	}

	if (!loadAddressFromFile(&pMarket->location, fp))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name,fp);
		
	}

	fclose(fp);

	loadProductFromTextFile(pMarket, "Products.txt");


	pMarket->customerArr = loadCustomerFromTextFile(customersFileName, &pMarket->customerCount);
	if (!pMarket->customerArr)
		return 0;

	return	1;

}
#endif

int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName)
{
	FILE* fp;
	//L_init(&pMarket->productList);
	fp = fopen(fileName, "r");
	int count;
	fscanf(fp, "%d\n", &count);


	//Product p;
	Product* pTemp;
	for (int i = 0; i < count; i++)
	{
		pTemp = (Product*)calloc(1, sizeof(Product));
		myGets(pTemp->name, sizeof(pTemp->name), fp);
		myGets(pTemp->barcode, sizeof(pTemp->barcode), fp);
		fscanf(fp, "%d %f %d\n", &pTemp->type, &pTemp->price, &pTemp->count);
		insertNewProductToList(&pMarket->productList, pTemp);
	}

	fclose(fp);
	return 1;
}