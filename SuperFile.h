#pragma once

#include <stdio.h>
#include "Supermarket.h"

int		saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
			const char* customersFileName, int isCompress);
int		loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
			const char* customersFileName);

//int		loadCustomerFromTextFile(SuperMarket* pMarket, const char* customersFileName);


int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName);//ceck
/////////////////////////////compress/////////////////////////////////////////
int		saveSuperMarketToFileCompressed(const SuperMarket* pMarket, const char* fileName, const char* customersFileName);
int	loadSuperMarketFromCompressFile( SuperMarket* pMarket, const char* fileName, const char* customersFileName);

