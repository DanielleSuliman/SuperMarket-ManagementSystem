#pragma once
#include "ShoppingCart.h"

typedef struct
{
	char*			name;
	ShoppingCart*	pCart;
	int		shopTimes;
	float totalSpend;
}Customer;

int		initCustomer(Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);

int		isCustomer(const Customer* pCust, const char* name);

void	pay(Customer* pCustomer);

void	freeCustomer(Customer* pCust);



