#pragma once
#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
#include"listGen.h"


typedef enum { eNoSort, eSortByNane, eSortByShopTimes, eSortByTotalSpend, eNumSortType } eCustomersSortType;
static const char* CustomersSortType[eNumSortType] = { "The Customers No Sorted", "Sort By Name", "Sort By ShopTimes", "Sort By TotalSpend" };
typedef struct
{
	char*		name;
	Address		location;
	Customer*	 customerArr;
	eCustomersSortType SortType;
	int			customerCount;
	LIST		listProduct;
	
}SuperMarket;


int		initSuperMarket(SuperMarket* pMarket);
void	printSuperMarket(const SuperMarket* pMarket);
int		addProduct(SuperMarket* pMarket);
int		addNewProduct(SuperMarket* pMarket, const char* barcode);
int		addCustomer(SuperMarket* pMarket);
int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int		doShopping(SuperMarket* pMarket);
Customer* doPrintCart(SuperMarket* pMarket);
int		doPayment(SuperMarket* pMarket);
Customer*		getCustomerShopPay(SuperMarket* pMarket);
void fillCart(SuperMarket* pMarket, ShoppingCart* pCart);

void	printProductByType(SuperMarket* pMarket);
void	printAllProducts(const SuperMarket* pMarket);
void	printAllCustomers(const SuperMarket* pMarket);


int		getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductFromUser(SuperMarket* pMarket, char* barcode);
void	getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer* FindCustomerByName(SuperMarket* pMarket, const char* name);

Customer* getCustomerWhoShop(SuperMarket* pMarket);
Product* getProductAndCount(SuperMarket* pMarket, int* pCount);

void	freeMarket(SuperMarket* pMarket);


NODE* getTheNodeBeforNewProductBybarcode(LIST* pList, const char* barcode);
eCustomersSortType qsortCustomersArry(SuperMarket* pMarket);
int compareCustomerByName(const void* a, const void* b);
int compareCustomerByShopTimes(const void* a, const void* b);
int compareCustomerByTotalSpend(const void* a, const void* b);
eCustomersSortType getqsortCustomersType();
eCustomersSortType SearchCustomerbySortType(SuperMarket* pMarket);
Customer* findCustomerByShopTimes(Customer* arr, int count, int shopTimes);
Customer* findCustomerByTotalSpend(Customer* arr, int count, float TotalSpend);
Customer* findCustomerByName(Customer* arr, int count, char* Name);
int countProduct(const LIST* pList);
int initSuperMarketFromBinFile(SuperMarket* pMarket);
int readAddresSuperMarket(SuperMarket* pMarket, FILE* file);
int readNameSuperMarket(SuperMarket* pMarket, FILE* file);
int readProductSuperMarket(SuperMarket* pMarket, FILE* file);
int readCustomersSuperMarketFromTxt(SuperMarket* pMarket);
int saveSuperMarket(SuperMarket* sm);
int saveSuperMarketToBinaryFile(SuperMarket* sm,  FILE* filename);
int saveCustomers(SuperMarket* supermarket);





