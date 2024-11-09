#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "General.h"

int	initCustomer(Customer* pCustomer)
{

	do {
		pCustomer->name = getStrExactLength("Enter customer name\n");
		if (!pCustomer->name)
			return 0;
	} while (checkEmptyString(pCustomer->name));
	
	pCustomer->pCart = NULL;
	pCustomer->shopTimes = 0;
	pCustomer->totalSpend = 0;
	return 1;
}

void	printCustomer(const Customer* pCustomer)
{
	printf("Name: %s\t", pCustomer->name);
	printf("shop Times: %d\t", pCustomer->shopTimes);
	printf("total Spend: %f\t", pCustomer->totalSpend);
	
	if (pCustomer->pCart == NULL)
		printf("Shopping cart is empty!\n");
	else 
	{
		printf("Doing shopping now!!!\n");
		
	}
}


void	pay(Customer* pCustomer)
{
	pCustomer->shopTimes += 1;
	if (!pCustomer->pCart)
		return;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	printShoppingCart(pCustomer->pCart);
	printf("the Customer  shop in the store %d\n", pCustomer->shopTimes);
	printf("the Customer totel spend in the store %f\n", pCustomer->totalSpend);
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}




int isCustomer(const Customer* pCust, const char* name)
{
	if (strcmp(pCust->name, name) == 0)
		return 1;
	return 0;
}

void	freeCustomer(Customer* pCust)
{
	if (pCust->pCart)
		pay(pCust); //will free every thing
	free(pCust->name);
}