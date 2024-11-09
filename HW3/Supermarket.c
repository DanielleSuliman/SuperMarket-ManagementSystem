#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include<stdlib.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "Address.h"
#include "General.h"
#include "ShoppingCart.h"
#include"listGen.h"
#define S_BIN "SuperMarket.bin"
#define S_TXT "Customers.txt"


///typedef void* DATA;		

int		initSuperMarket(SuperMarket* pMarket)
{
pMarket->customerCount = 0;
pMarket->customerArr = NULL;
pMarket->SortType = eNoSort;
pMarket->listProduct;
LIST* pList=&pMarket->listProduct;
if (!L_init(pList))
	return 0;
	
pList->head.key = 0;//count of product
pMarket->name = getStrExactLength("Enter market name");
return initAddress(&pMarket->location);
}

void	printSuperMarket(const SuperMarket* pMarket)
{
printf("Super Market Name: %s\t", pMarket->name);
printf("Address: ");
printAddress(&pMarket->location);
printf("\n");
printAllProducts( pMarket);
printf("\n");
printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
char barcode[BARCODE_LENGTH + 1];
Product* pProd = getProductFromUser(pMarket,barcode);
if (pProd != NULL) //This barcode exist in stock
	updateProductCount(pProd);
else { //new product to stock
	if(!addNewProduct(pMarket, barcode))
		return 0;
}

return 1;
}

int		addNewProduct(SuperMarket* pMarket,const char* barcode)

{
	
Product* pProd = (Product*)calloc(1, sizeof(Product));
if (!pProd)
{
	return 0;
}

strcpy(pProd->barcode, barcode);
initProductNoBarcode(pProd);
LIST* plistProduct = &pMarket->listProduct;
NODE* pNewNode;
NODE* pNodbefor = getTheNodeBeforNewProductBybarcode(plistProduct, barcode);//get the nodeBefor to func L_insert(NODE* pNode, DATA Value)
pNewNode=L_insert(pNodbefor, pProd);
if (!pNewNode)
	return 0;

return 1;
}
NODE*  getTheNodeBeforNewProductBybarcode(LIST* pList, const char* barcode) {
NODE* pNode = pList->head.next;
	
if (pNode == NULL)
	return &(pList->head);

NODE* tump = pNode->next;
Product* pProduct = (Product*)pNode->key;
if (strcmp(barcode, pProduct->barcode) == -1) {//if the node is the head
	return &pList->head;
}
		
while (tump != NULL)
{
		pProduct = (Product*)tump->key;  // if the node is  in the midule;
	if (strcmp(barcode, pProduct->barcode) == -1)
	{
		return pNode;
	}
	pNode = pNode->next;
	tump = pNode->next;
}
return pNode;//node is in the end ;

}

int		addCustomer(SuperMarket* pMarket)
{
Customer cust;
if (!initCustomer(&cust))
	return 0;

if (isCustomerInMarket(pMarket, &cust))
{
	printf("This customer already in market\n");
	free(cust.name);
	return 0;
}

pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
if (!pMarket->customerArr)
{
	free(cust.name);
	return 0;
}

pMarket->customerArr[pMarket->customerCount] = cust;
pMarket->customerCount++;
return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
for (int i = 0; i < pMarket->customerCount; i++)
{
	if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
		return 1;
}
return 0;
}

int	doShopping(SuperMarket* pMarket)//111111111111
{
Customer* pCustomer = getCustomerShopPay(pMarket);
if (!pCustomer)
	return 0;

if (pCustomer->pCart == NULL)
{
	pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
	if (!pCustomer->pCart)
		return 0;
	initCart(pCustomer->pCart);
}
fillCart(pMarket, pCustomer->pCart);
if (pCustomer->pCart->count == 0) {
	free(pCustomer->pCart);
	pCustomer->pCart =NULL;

}
	
else
{
	printf("---------- Shopping ended ----------\n");
}
return 1;
}



int	doPayment(SuperMarket* pMarket)//11

{
Customer* pCustomer = doPrintCart(pMarket);
if(!pCustomer)
	return 0;
pCustomer->totalSpend = getTotalPrice(pCustomer->pCart);//init the total spend
pay(pCustomer);
return 1;
}
Customer*	doPrintCart(SuperMarket* pMarket)//222222222222222
{
Customer* pCustomer = getCustomerShopPay(pMarket);
if (!pCustomer)
	return NULL;
if (pCustomer->pCart == NULL)
{
	printf("Customer cart is empty\n");
	return NULL;
}
printShoppingCart(pCustomer->pCart);
return pCustomer;
}

Customer*	getCustomerShopPay(SuperMarket* pMarket)//3333333333333
{
if (pMarket->customerCount == 0)
{
	printf("No customer listed to market\n");
	return NULL;
}

if (pMarket->listProduct.head.key == 0)
{
	printf("No products in market - cannot shop\n");
	return NULL;
}

Customer* pCustomer = getCustomerWhoShop(pMarket);
if (!pCustomer)
{
	printf("this customer not listed\n");
	return NULL;
}

return pCustomer;
}
Customer* getCustomerWhoShop(SuperMarket* pMarket)//4444444444444444
{
printAllCustomers(pMarket);
char name[MAX_STR_LEN];
getsStrFixSize(name, sizeof(name), "Who is shopping? Enter cutomer name\n");

Customer* pCustomer = FindCustomerByName(pMarket, name);

return pCustomer;
}
void	printAllProducts(const SuperMarket* pMarket)
{
	int count=countProduct(&(pMarket->listProduct));
printf("There are %d products\n", count);
printf("%-20s %-10s\t", "Name", "Barcode");
printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
printf("--------------------------------------------------------------------------------\n");
	
	L_print((LIST*)(&pMarket->listProduct),printProduct);
		
}

void	printAllCustomers(const SuperMarket* pMarket)
{
printf("There are %d listed customers\n", pMarket->customerCount);
printf(" %s\n", CustomersSortType[pMarket->SortType]);

/*for (int i = 0; i < pMarket->customerCount; i++)
	printCustomer(&pMarket->customerArr[i]);*/
generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(pMarket->customerArr[0]), printCustomer);
}





void fillCart(SuperMarket* pMarket,ShoppingCart* pCart)
{
printAllProducts(pMarket);
char op;
while (1)
{
	printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
	do {
		scanf("%c", &op);
	} while (isspace(op));
	getchar(); //clean the enter
	if (op != 'y' && op != 'Y')
		break;
	int count;
	Product* pProd = getProductAndCount(pMarket, &count);
	if(pProd)
	{
		if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
		{
			printf("Error adding item\n");
			return;
		}
		pProd->count -= count; //item bought!!!
	}
}
	

}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
char barcode[BARCODE_LENGTH + 1];
Product* pProd = getProductFromUser(pMarket, barcode);
if (pProd == NULL)
{
	printf("No such product\n");
	return NULL;
} 
	
if (pProd->count == 0)
{
	printf("This product out of stock\n");
	return NULL;
}
	
int count;
do {
	printf("How many items do you want? max %d\n", pProd->count);
	scanf("%d", &count);
} while (count <= 0 || count > pProd->count);
*pCount = count;
return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
if (pMarket ->listProduct.head.key== 0)
{
	printf("No products in market\n");
	return;
}
eProductType	type = getProductType();
int count = 0;
Product* pProd;
NODE* pNode = pMarket->listProduct.head.next;

for (pNode; pNode!=NULL; pNode= pNode->next)
{
	pProd = (Product*)pNode->key;
	if (pProd->type == type)
	{
		count++;
		printProduct(pProd);
	}

}
if (count == 0)
	printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
getBorcdeCode(barcode);
return getProductByBarcode(pMarket, barcode);
}

void	freeMarket(SuperMarket* pMarket)
{
free(pMarket->name);
freeAddress(&(pMarket->location));

L_free(&(pMarket->listProduct), freeProduct);
generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(pMarket->customerArr[0]), freeCustomer);
free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
int cont = 1;
while (cont)
{
	getBorcdeCode(barcode);
	int index = getProductIndexByBarcode(pMarket, barcode);
	if (index == -1)
		cont = 0;
	else
		printf("This product already in market\n");
}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{

int count = 0;
Product* pProd;
NODE* pNode = pMarket->listProduct.head.next;
if (pNode == NULL)
	return  -1;

for (pNode; pNode != NULL; pNode = pNode->next)
{
	pProd = (Product*)pNode->key;
	if (isProduct(pProd, barcode))
	{
		return  count;
			
	}
	count++;

}
return -1;

}


Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
Product* pProd;
NODE* pNode = pMarket->listProduct.head.next;
if(pNode==NULL)
	return  NULL;

for (pNode; pNode!= NULL; pNode=pNode->next)
{
	pProd = (Product*)pNode->key;
	if (isProduct(pProd, barcode))
		return  pProd;
}
return  NULL;

}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
for (int i = 0; i < pMarket->customerCount; i++)
{
	if (isCustomer(&pMarket->customerArr[i], name))
		return &pMarket->customerArr[i];
}
return  NULL;
}
eCustomersSortType qsortCustomersArry(SuperMarket* pMarket) {
Customer* arryCustomers = pMarket->customerArr;
	
if (!arryCustomers) {
	return eNoSort;//no customers in the arry;
}


eCustomersSortType choicetypeSort = getqsortCustomersType();
if (choicetypeSort == eSortByNane)
	qsort(arryCustomers, pMarket->customerCount, sizeof(Customer), compareCustomerByName);

if(choicetypeSort==eSortByShopTimes)
	qsort(arryCustomers, pMarket->customerCount, sizeof(Customer), compareCustomerByShopTimes);

if(choicetypeSort==eSortByTotalSpend)
	qsort(arryCustomers, pMarket->customerCount, sizeof(Customer), compareCustomerByTotalSpend);
pMarket->SortType = choicetypeSort;
printf("tha Customers arry %s\n", CustomersSortType[choicetypeSort]);
return choicetypeSort;
}
int compareCustomerByName(const void* a, const void* b) {
const Customer cus1 = *(const Customer*)a;
const Customer cus2 = *(const Customer*)b;
return strcmp(cus1.name, cus2.name);

}
int compareCustomerByShopTimes(const void* a, const void* b) {
const Customer cus1 = *(const Customer*)a;
const Customer cus2 = *(const Customer*)b;

return cus1.shopTimes-cus2.shopTimes;
		
}
int compareCustomerByTotalSpend(const void* a, const void* b) {
const Customer cus1 = *(const Customer*)a;
const Customer cus2 = *(const Customer*)b;

return (int)(cus1.totalSpend - cus2.totalSpend);

}

eCustomersSortType getqsortCustomersType() {
int option;
printf("\n\n");
do {
printf("Please enter one of the following types\n");
for (int i = 1; i < eNumSortType; i++)
	printf("%d for %s\n", i, CustomersSortType[i]);
scanf("%d", &option);
} while (option <1 || option >= eNumSortType);
getchar();
printf("you choice is %s\n", CustomersSortType[option]);
return (eCustomersSortType)option;

}
eCustomersSortType SearchCustomerbySortType(SuperMarket* pMarket) {
Customer* found = NULL;
if (pMarket->customerArr == NULL) {
	printf("NO customers--->pMarket->customerArr == NULL  \n");
	return 1;
}
		
if (pMarket->SortType == eNoSort)
	return eNoSort;
if (pMarket->SortType == eSortByNane)
{
	char* name;
	do {
			name  = getStrExactLength("Enter  name of customer that you want to found\n");
		if (!name)
			return 0;
	} while (checkEmptyString(name));
	found = findCustomerByName(pMarket->customerArr, pMarket->customerCount, name);
}
if (pMarket->SortType == eSortByShopTimes)
{
	int shopeTimes;
	printf("Enter  shope times of customer  that you want to found\n");
	scanf("%d", &shopeTimes);
	found = findCustomerByShopTimes(pMarket->customerArr, pMarket->customerCount, shopeTimes);
}

if (pMarket->SortType == eSortByTotalSpend) 
{
	float TotalSpend;
	printf("Enter Total Spend of customer  that you want to found\n");
	scanf("%f.2", &TotalSpend);
	found = findCustomerByTotalSpend(pMarket->customerArr, pMarket->customerCount, TotalSpend);
}
if (found!=NULL) 
{
	printf("-----the  ditel of customer you search-- \n");
	printCustomer(found);
}
else
{
	printf("the  customer  NOT FOUND ");
}

return pMarket->SortType;
}
Customer* findCustomerByShopTimes(Customer* arr, int count, int shopTimes)
{
Customer* pPer = NULL;
Customer key = { "",NULL,shopTimes,0 };
pPer = (Customer*)bsearch(&key, arr, count, sizeof(Customer), compareCustomerByShopTimes);
return pPer;
}
Customer* findCustomerByTotalSpend(Customer* arr, int count, float TotalSpend)
{
Customer* pPer = NULL;
Customer key = { "",NULL,0,TotalSpend };
pPer = (Customer*)bsearch(&key, arr, count, sizeof(Customer), compareCustomerByTotalSpend);
return pPer;
}
Customer* findCustomerByName(Customer* arr, int count, char* Name)
{
Customer* pPer = NULL;
Customer key = { Name,NULL,0,0 };
pPer = (Customer*)bsearch(&key, arr, count, sizeof(Customer), compareCustomerByName);
return pPer;
}
/////////////////////////////////////////////////////////////////////////////////////////////BINARI
int countProduct(const LIST* pList) {
NODE	*tmp;
int		 count = 0;

if (!pList) return 0;

for (tmp = pList->head.next; tmp; tmp = tmp->next) {
	count++;
}
	
return  count;
}
int initSuperMarketFromBinFile(SuperMarket* pMarket) {
pMarket->SortType = eNoSort;
	
FILE* f = fopen(S_BIN,"rb");
if (f == NULL)
{
	printf("Failed opening the file. Exiting!\n");
	fclose(f);
	return 0;
}
if (!readNameSuperMarket(pMarket, f)){
																																																																																																																																																																																																																																																																																																													
	printf("Failed read Name SuperMarket\n");
	fclose(f);
	return 0;
}
if (!readAddresSuperMarket(pMarket, f) ) {

	printf("Failed read Addres SuperMarket\n");
	fclose(f);
	return 0;
}
if (!readProductSuperMarket(pMarket, f)) {

	printf("Failed read product SuperMarket\n");
	fclose(f);
	return 0;
}
if (!readCustomersSuperMarketFromTxt(pMarket)) {

	printf("Failed read customer SuperMarket\n");
	fclose(f);
	return 0;
}

	
fclose(f);
return 1;
}
int readCustomersSuperMarketFromTxt(SuperMarket* pMarket) {//////

FILE* ft = fopen(S_TXT, "r");
if (ft == NULL)
{
	printf("Failed opening the file. Exiting!\n");
	fclose(ft);
	return 0;
}
if (!fscanf(ft, "%d\n", &pMarket->customerCount)) {
	fclose(ft);
	return 0;
}
char temp[MAX_STR_LEN];
pMarket->customerArr = (Customer*)malloc(pMarket->customerCount * sizeof(Customer));

for (int i = 0; i < pMarket->customerCount; i++)
{
		
		

	if (fgets(temp, MAX_STR_LEN, ft))
	{
		temp[strcspn(temp, "\n")] = '\0';
	}
	else
	{
		fclose(ft);
		return 0;
	}
		
	pMarket->customerArr[i].name = getDynStr(temp);
			
	if (!fscanf(ft, "%d %f\n",&pMarket->customerArr[i].shopTimes, &pMarket->customerArr[i].totalSpend)) {
		fclose(ft);
		return 0;
			
	}
	pMarket->customerArr[i].pCart = NULL;
		
		
}
fclose(ft);
return 1;
}
int readNameSuperMarket(SuperMarket* pMarket, FILE* file)
{
int len;
if (!fread(&len,sizeof(int), 1, file) )
{
printf("Failed read lenght of Name \n");
return 0;
}
pMarket->name = (char*)malloc(len*sizeof(char));//
if(pMarket->name ==NULL)
	return 0;
	
if (!fread(pMarket->name, len * sizeof(char), 1, file))
{
	free(pMarket->name);
	printf("Failed read Name \n");
	return 0;
}
return 1;
}
int readAddresSuperMarket(SuperMarket* pMarket, FILE* file) {
int len;
if (!fread(&pMarket->location.num,  sizeof(int), 1, file))
{
	printf("Failed read numberHome \n");
	return 0;
}
if (!fread(&len, sizeof(int), 1, file))
{
	printf("Failed read size street \n");
	return 0;
}
pMarket->location.street=(char*)malloc(len *sizeof(char));
if (!fread(pMarket->location.street, len*sizeof(char), 1, file))
{
	free(pMarket->location.street);
	printf("Failed read name street \n");
	return 0;
}
if (!fread(&len, sizeof(int), 1, file))
{
	printf("Failed read size city \n");
	return 0;
}
pMarket->location.city = (char*)malloc(len * sizeof(char));
if (!fread(pMarket->location.city, len * sizeof(char), 1, file))
{
	free(pMarket->location.city);
	printf("Failed read name city \n");
	return 0;
}


return 1;
}
int readProductSuperMarket(SuperMarket* pMarket, FILE* file) {
LIST* pList = &pMarket->listProduct;
if (!L_init(pList))
	return 0;
	
NODE* pNewNode;
int productCount;
if (!fread(&productCount, sizeof(int), 1, file))
{
	printf("Failed read number of customers \n");
	return 0;
}
for (int i = 0; i < productCount; i++)
{
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	return 0;
	if (!fread(pProd, sizeof(Product), 1, file))
	{
		printf("Failed read  customers \n");
		return 0;
	}
	NODE* pNodbefor = getTheNodeBeforNewProductBybarcode(&pMarket->listProduct, pProd->barcode);//get the nodeBefor to func L_insert(NODE* pNode, DATA Value)
	pNewNode = L_insert(pNodbefor, pProd);
	if (!pNewNode)
		return 0;
}
return 1;
}

int saveSuperMarket(SuperMarket* sm) {
FILE* f = fopen(S_BIN, "wb");
if (f == NULL)
{
	printf("Failed opening the file. Exiting!\n");
	fclose(f);
	return 0;
}
if (!saveSuperMarketToBinaryFile(sm, f))
{
	printf("Failed save SuperMarket To Binary File. Exiting!\n");
	fclose(f);
	return 0;
}
	
fclose(f);
if (!saveCustomers(sm)) {
	printf("Failed save customers To text File. Exiting!\n");
	return 0;
}

return 1;
}
int saveSuperMarketToBinaryFile(SuperMarket* sm, FILE* filename)
{
int count = countProduct(&sm->listProduct);

// Write name
int nameLen = (int)strlen(sm->name) + 1;
if (!fwrite(&nameLen, sizeof(int), 1, filename))
{
	printf("Failed save SuperMarket namelen To Binary File. Exiting!\n");
	fclose(filename);
	return 0;
}

if (!fwrite(sm->name, nameLen * sizeof(char), 1, filename))
{
	printf("Failed save SuperMarket name To Binary File. Exiting!\n");
	fclose(filename);
	return 0;
}

// Write address

if (!fwrite(&sm->location.num, sizeof(int), 1, filename)) {
	printf("Failed save SuperMarket namber of street To Binary File. Exiting!\n");
	fclose(filename);
	return 0;

}

int streetLen = (int)strlen(sm->location.street) + 1;
if (!fwrite(&streetLen, sizeof(int), 1, filename)) {

	printf("Failed save SuperMarket namber of street To Binary File. Exiting!\n");
	fclose(filename);
	return 0;
}
if (!fwrite(sm->location.street, streetLen*sizeof(char),1, filename)) {

	printf("Failed save SuperMarket street To Binary File. Exiting!\n");
	fclose(filename);
	return 0;
}

int cityLen = (int)(strlen(sm->location.city)) + 1;
if (!fwrite(&cityLen, sizeof(int), 1, filename)) 
{
	printf("Failed save SuperMarket city Len Binary File. Exiting!\n");
	fclose(filename);
	return 0;
}
if (!fwrite(sm->location.city, cityLen * sizeof(char), 1, filename)) 
{
	printf("Failed save SuperMarket city  Binary File. Exiting!\n");
	fclose(filename);
	return 0;
}

//// Write each product//////////////////////////////////////////////
NODE*tmp;
	
if (!fwrite(&count, sizeof(int),1, filename))
{
	printf("Failed save count product  Binary File. Exiting!\n");
	fclose(filename);
	return 0;
}

for (tmp = sm->listProduct.head.next; tmp != NULL;tmp=tmp->next) {
	Product* product = (Product*)tmp->key;
	if(!fwrite(product, sizeof(Product), 1, filename))
	{
		printf("Failed save Product number city  Binary File. Exiting!\n");
		fclose(filename);
		return 0;
	}
}
return 1;
}

int saveCustomers(SuperMarket* supermarket) {
// Open the file for writing
FILE* fp = fopen(S_TXT, "w");
if (!fp) {
	printf("couldnt open the file. please try again.");
	fclose(fp);
	return 0;
}
// Write the number of customers to the file
fprintf(fp, "%d\n", supermarket->customerCount);

// Write each customer's data to the file
for (int i = 0; i < supermarket->customerCount; i++) {
	fprintf(fp, "\n%s\n%d %f\n", supermarket->customerArr[i].name, supermarket->customerArr[i].shopTimes, supermarket->customerArr[i].totalSpend);
}
// Close the file
fclose(fp);
return 1;
}