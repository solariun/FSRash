/*
 *  FrmTest.cpp
 *  FSRash
 *
 *  Created by Gustavo Campos on 1/11/10.
 *  Copyright 2010 `. All rights reserved.
 *
 */

#include "FrmTest.h"
#include <FastVector.h>
#include <vt100.h>


int main (int nArgs, char* pszArgs [])
{
	FastVector<char>* pFastVector;
	unsigned long int nCount;
	char     szKey [25] = "";
	clock_t  tClock;
	double   dElipsedTime;
		
	
	VT100_Cls ();
	
	try
	{		
		pFastVector = new FastVector<char> (4,50);
		
		pFastVector->AddData (NULL, "Gustav4", 7L);

		//gets (szKey);
		
		for (nCount=0; nCount < 20000000; nCount++)
		{
			snprintf (szKey, sizeof (szKey)-1, "Data[%-.8lu]", nCount);
			tClock = Util_GetClock();
			pFastVector->AddData("DATA", szKey, strlen (szKey));
			dElipsedTime = (double) (Util_GetClock() - tClock) / CLOCKS_PER_SEC;
			
			TRACE ("Adding: %-40s  (Time: %5.4f) Interactions: %u\n", szKey, dElipsedTime, pFastVector->GetInteractions ()); fflush (stdout);
		}

		pFastVector->AddData (NULL, "Gustav4", 7L);

		delete pFastVector;
		pFastVector = NULL;
	}
	catch (Exception* pEx) 
	{
		printf ("Error: %s\n", pEx->GetExceptionMessage ());
		
		if (pFastVector != NULL)
			delete pFastVector;
		
		exit (1);		
	}
	
	exit (0);
	
}