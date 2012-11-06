#include <Graphic.h>
#include <FastVector.h>
#include <iostream>
#include <FileSearch.h>
#include <Exception.h>


char* FSRash_GetStrFromTime (time_t tTime, char* pszReturn, unsigned int nStrLen)
{
	NULL_VERIFY (pszReturn, NULL);
	Verify      (nStrLen >= 8, "Str to short, please supply a string higher than 8", NULL);
	
	struct tm* pTM;
	
	Verify ((pTM = localtime (&tTime)) != NULL, "", NULL);
	
	Verify (snprintf (pszReturn, nStrLen, " %-.2d:%-.2d:%-.2d %-.2d/%-.2d/%-.4d",
				pTM->tm_hour,  pTM->tm_min,
				pTM->tm_sec,   pTM->tm_mday,
				pTM->tm_mon,   pTM->tm_year + 1900)>0, "", NULL);
	
	
	return pszReturn;
}




char* FSRash_GetStrFromMode (const mode_t mModel, char* pszReturn, unsigned int nStrLen)
{
	NULL_VERIFY (pszReturn, NULL);
	Verify      (nStrLen >= 4, "Str to short, please supply a string higher than 8", NULL);

#ifndef WIN32
	
	const char* pszDataType;
	
	switch ((mModel & S_IFMT))
	{
		case S_IFREG:
				pszDataType = "Regular";
			break;
			
		case S_IFLNK:
				pszDataType = "Link";
			break;
		
		case S_IFDIR:
				pszDataType = "Directory";
			break;
		
		case S_IFIFO:
				pszDataType = "Fifo";
			break;
		
		default:
				pszDataType = "Outros tipos";
	}
	
	
	snprintf (pszReturn, nStrLen, "{%c%c%c%c%c%c%c%c%c%c};%s",
			  (S_ISUID & mModel) != false ? 'S' : '-',
			  (S_IRUSR & mModel) != false ? 'R' : '-',
			  (S_IWUSR  & mModel) != false ? 'W' : '-',
			  (S_IXUSR & mModel) != false ? 'X' : '-',

			  (S_IRGRP  & mModel) != false ? 'R' : '-',
			  (S_IWGRP  & mModel) != false ? 'W' : '-',
			  (S_IXGRP  & mModel) != false ? 'X' : '-',

			  (S_IROTH & mModel) != false ? 'R' : '-',
			  (S_IWOTH & mModel) != false ? 'W' : '-',
			  (S_IXOTH & mModel) != false ? 'X' : '-',
			  
			  pszDataType);
#else
	
	pszReturn = "NA;NA";

#endif

	return pszReturn;
}


int main (int argc, char * const argv[]) 
{
	
	
	try
	{	
		long int nData;
		char     pszData [25];
		char*    pszRecoverData;
		
		FastVector<char>* pFastVector;
		
		pFastVector = new FastVector<char> (5,20);
		
		for (nData=0; nData < 10000L; nData++)
		{
			snprintf (pszData, sizeof (pszData) - 1, "Data_%lu", nData);
			printf ("Current Data: %s\r", pszData);
			pFastVector->AddData (strdup (pszData), pszData, strlen (pszData));
		}
		
		pszRecoverData = pFastVector->GetData ("Data_2999", 9);
		
		printf ("\n\n Recovered data: (%s)\n", pszRecoverData); 
		

		delete pFastVector;
	}
	catch (Exception* pEx) 
		{
		printf ("Error: %s\n", pEx->GetExceptionMessage ());
		
		exit (1);		
	}

	exit (0);


	
	
	FileSearch* pFileSearch;
	
	FileScan pScan [] = { "*.c", "*.h", "*.txt", "*.pdf", "*.dll", "*.asp", "*.aspx", 0 };

#ifndef WIN32
	nice (20);
#endif
	
	
	
#ifdef _BASE64_
	
	uint8_t szData [3], szReturn [4] = {0,0,0,0};
	

	szData [0] = 155;
	szData [1] = 162;
	szData [2] = 233;
	
	Util_Base64Encode (szData, szReturn);

	printf ("1: [%u] 2: [%u] 3: [%u]\n", szData [0], szData [1], szData [2]);

	printf ("Base64 =%c%c%c%c\n", szReturn [0], szReturn [1], szReturn [2], szReturn [3]);
	
	
	exit (0);
#endif
	
	char szLastAccess [30] = "";
	char szLastModification [30] = "";
	char szLastStatus [30] = "";
	char  szFormat [] = "%s;%lld;%s;{%s};%d;%d;%s;%s;%s\n";
	char szMode [50] = "";

	uint64_t nCounter = 0;
	uint64_t nTotal = 0;
	uint     nPorcent;
	const char szGauge [] = "||||||||||";
	
	Util_CPU_Init ();
	
	if (argc > 1)
		Util_SetMaxCPU (atoi (argv [1]));
	else 
		Util_SetMaxCPU (0);
	
	
	fprintf (stderr, "Collecting information...\r");
	
	printf ("File Name;Size;Permission;Type;UID;Owner;Group;Last Access;Last Modif;Last Status\n");
	
	try 
	{
		pFileSearch = new FileSearch (argv [2],  pScan, true, true);		
		
		pFileSearch->LoadDirectory ();
		
		
		nTotal = pFileSearch->GetNumFiles ();
		
		pFileSearch->SetTopOfList ();
		
		do 
		{
			FSRash_GetStrFromTime (pFileSearch->GetLastAccessTime (), szLastAccess, sizeof (szLastAccess) - 1);
			FSRash_GetStrFromTime (pFileSearch->GetLastModificationTime (), szLastModification, sizeof (szLastModification) - 1);
			FSRash_GetStrFromTime (pFileSearch->GetLastStatusTime (), szLastStatus, sizeof (szLastStatus) - 1); 
			FSRash_GetStrFromMode (pFileSearch->GetPermission (), szMode, sizeof (szMode) - 1);

			nPorcent = (nCounter++ * 100 / nTotal);
			
	        fprintf (stderr, "Processing: [%-10s] [%u%%] [%llu] of [%llu]     \r",  &szGauge [10 - (nPorcent / 10)],  nPorcent, nCounter, nTotal);				
			
			printf   (szFormat, 
					 pFileSearch->GetFileName (),
					 pFileSearch->GetFileSize (),
					 szMode,
					 pFileSearch->GetUID (),
#ifndef WIN32
					 pFileSearch->GetUserOwner (),
					 pFileSearch->GetGroupOwner (),
#else
					 -1, -1,
#endif
					  szLastAccess, szLastModification, szLastStatus);
			
			 
			
		} while (pFileSearch->SetNextItem ());
		
		delete (pFileSearch);
		
		fprintf (stderr, "\n\n");
	}
	catch (Exception* pEx)
	{
		printf ("Error: %s\n", pEx->GetExceptionMessage ());
		
		return 1;
	}
    return 0;
}
