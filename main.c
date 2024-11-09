#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"
#include "SuperFile.h"
#include "myMacros.h"

int menu();


int main(int  argc, char*  argv[])
{
	SuperMarket	market;
	
	int isCompressed;
	//= sscanf(argv[1], "%d", &isCompressed);
	//char* file= sscanf(argv[2], "%s", &file);

	char* file = paramsMain(argc, argv, &isCompressed);

	CHECK_MSG_RETURN_0(initSuperMarket(&market, file, CUSTOMER_FILE_NAME, isCompressed), "error init  Super Market")

	/*if (option ==0)
	{
		if (!initSuperMarket(&market, SUPER_FILE_NAME, CUSTOMER_FILE_NAME, option))
		{
			printf("error init  Super Market from SUPER_FILE_NAME ");
			return 0;
		}
	}
	else {
		if (!initSuperMarket(&market, SUPERMARKET_COMPRESS_check, CUSTOMER_FILE_NAME, 1))
		{
			printf("error init  Super Market SUPERMARKET_COMPRESS_check");
			return 0;
		}

	}*/
	int stop = 0;
	int option;
	do
	{
		 option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;


		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if (!doShopping(&market))
				printf("Error in shopping\n");
			break;

		case ePrintCart:
			doPrintCart(&market);
			break;

		case eCustomerPay:
			if (!doPayment(&market))
				printf("Error in payment\n");
			break;

		case eSortCustomer:
			sortCustomers(&market);
			break;

		case eSearchCustomer:
			findCustomer(&market);
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case EXIT:
		//	printf("DO YOU WHANT SAVE THE  DATA TO COMPRESS FILE OR TO REAGILER BIN  FILE\n ?\nENTER 1-->COMPRESS\nENTER 0-->REAGILER/n");
		//	int res;
		//	scanf("%d", &res);
			
		 if(!saveSuperMarketToFile(&market, file, CUSTOMER_FILE_NAME,isCompressed))
			printf("Error saving supermarket to file\n");
			
			printMessage("Thank", "You", "For", "Shpping", "With", "Us", NULL);
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	handleCustomerStillShoppingAtExit(&market);
	freeMarket(&market);

	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}
char* paramsMain(int argc, char* argv[], int* isCompressed)
{
	int numOfVars = 0;
	*isCompressed = 1;
	numOfVars += sscanf(argv[1], "%d", &(*isCompressed));
	char* file = getDynStr(argv[2]);
	if (file != NULL) {
		numOfVars++;
	}
	if (numOfVars != 2) {
		return NULL;
	}
	return file;
}

