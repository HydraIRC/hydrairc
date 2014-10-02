// DLLVersion.h: interface for the CDLLVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLVERSION_H__88B3F8C1_27F2_11D3_80A3_0090276F9F55__INCLUDED_)
#define AFX_DLLVERSION_H__88B3F8C1_27F2_11D3_80A3_0090276F9F55__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/*  For some odd reason, Microsoft published a sample code that uses shlwapi.h 
    (and shlwapi.lib)
    to tinker file versions.

    This header file could not be found anywhere !!!
    Not in Visual C++ 4.2, Visual C++ 5.0 or MSDN versions up to July 97`.

    So, I just took out the interesting structures from scraps I found 
    and re-defined them here.
*/

//  Remember: You must link version.lib to the project for this class to work !!


#ifndef _DLL_VERSION_H_
#define _DLL_VERSION_H_

/*
#ifndef DLLVERSIONINFO
typedef struct _DllVersionInfo
{
    DWORD cbSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformID;
}DLLVERSIONINFO; 
#endif

#ifndef DLLGETVERSIONPROC
typedef int (FAR WINAPI *DLLGETVERSIONPROC) (DLLVERSIONINFO *);
#endif
*/

class CDLLVersion
{
    typedef enum {  WIN_DIR, // Windows directory (e.g.: "C:\Windows\")
                    SYS_DIR, // Windows system directory (e.g.: "C:\Windows\System")
                    CUR_DIR, // Current directory (e.g.: ".")
                    NO_DIR}  // No directory (path in file name)
        FileLocationType;   // Possible ways to add a path prefix to a file

public:
	
    CDLLVersion (LPSTR szDLLFileName) :
        m_dwMajor (0),
        m_dwMinor (0),
        m_dwBuild (0)
    {
        m_bValid = GetDLLVersion (szDLLFileName, m_dwMajor, m_dwMinor, m_dwBuild);
    }
        

    virtual ~CDLLVersion () {};

    DWORD GetMajorVersion ()
    {
        return m_dwMajor;
    }

    DWORD GetMinorVersion ()
    {
        return m_dwMinor;
    }

    DWORD GetBuildNumber ()
    {
        return m_dwBuild;
    }

    BOOL IsValid ()
    {
        return m_bValid;
    }

	CString GetFullVersion()
	{
		return m_stFullVersion;
	}

private:

	char *getVersion(char *fileName);
	
    BOOL GetDLLVersion (LPSTR szDLLFileName, 
                        DWORD &dwMajor, DWORD &dwMinor, DWORD &dwBuildNumber);

    BOOL CheckFileVersion (LPSTR szFileName, int FileLoc, 
                           DWORD &dwMajor, DWORD &dwMinor, DWORD &dwBuildNumber);

    BOOL ParseVersionString (LPSTR lpVersion, 
                             DWORD &dwMajor, DWORD &dwMinor, DWORD &dwBuildNumber);
	BOOL ParseVersionString1 (LPSTR lpVersion, 
                                      DWORD &dwMajor, DWORD &dwMinor, 
                                      DWORD &dwBuildNumber);

    BOOL FixFilePath (char * szFileName, int FileLoc);

    DWORD   m_dwMajor,      // Major version number
            m_dwMinor,      // Minor version number
            m_dwBuild;      // Build number
    BOOL    m_bValid;		// Is the DLL version information valid ?
	CString m_stFullVersion;
};

#endif

#endif // !defined(AFX_DLLVERSION_H__88B3F8C1_27F2_11D3_80A3_0090276F9F55__INCLUDED_)

/***************************************************************************

   Function:   GetDLLVersion

   Purpose:    Retrieves DLL major version, minor version and build numbers

   Input:      DLL file name
               Reference to Major number 
               Reference to Minor number 
               Reference to Build number 

   Output:     TRUE only if successful

   Remarks:    This function first tries to get the DLL version the nice way,
               that is, call the DllGetVersion function in the DLL.

               If this fails, it tries to located the DLL file in the file system,
               read the file information block and retrieve the file version.

****************************************************************************/
BOOL CDLLVersion::GetDLLVersion (LPSTR szDLLFileName, 
                                 DWORD &dwMajor, DWORD &dwMinor, DWORD &dwBuildNumber)
{
HINSTANCE   hDllInst;           // Instance of loaded DLL
char szFileName [_MAX_PATH];    // Temp file name

BOOL bRes = TRUE;               // Result


    lstrcpy (szFileName, szDLLFileName);    // Save a file name copy for the loading
    
	
	hDllInst = LoadLibrary(TEXT(szFileName));   //load the DLL


    if(hDllInst) {  // Could successfully load the DLL
        DLLGETVERSIONPROC pDllGetVersion;
        /*
        You must get this function explicitly because earlier versions of the DLL 
        don't implement this function. That makes the lack of implementation of the 
        function a version marker in itself.
        */
        pDllGetVersion = (DLLGETVERSIONPROC) GetProcAddress(hDllInst,
                          TEXT("DllGetVersion"));
   
        if(pDllGetVersion) {    // DLL supports version retrieval function
            DLLVERSIONINFO    dvi;

            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);
            HRESULT hr = (*pDllGetVersion)(&dvi);

            if(SUCCEEDED(hr)) { // Finally, the version is at our hands
                dwMajor = dvi.dwMajorVersion;
                dwMinor = dvi.dwMinorVersion;
                dwBuildNumber = dvi.dwBuildNumber;
            } else
                bRes = FALSE;   // Failure
        } else  // GetProcAddress failed, the DLL cannot tell its version
            bRes = FALSE;       // Failure

        FreeLibrary(hDllInst);  // Release DLL
    } else  
        bRes = FALSE;   // DLL could not be loaded

    if (!bRes) // Cannot read DLL version the nice way
	{
		for(int iDir = WIN_DIR; iDir <= NO_DIR; iDir++) // loop for each possible directory
		{
			lstrcpy (szFileName, szDLLFileName);    // Save a file name copy for the loading
			bRes = CheckFileVersion (szFileName, iDir, 
                                 dwMajor, dwMinor, dwBuildNumber); // Try the ugly way
			if(bRes)
				break;
		};
		return bRes;
	}
    else
        return TRUE;
}

/***************************************************************************

   Function:   CheckFileVersion

   Purpose:    Check the version information of a given file

   Input:      File name
               File location (Windows dir, System dir, Current dir or none)
               Reference to Major number 
               Reference to Minor number 
               Reference to Build number 

   Output:     TRUE only if successful

   Remarks:    Trashes original file name

****************************************************************************/
BOOL CDLLVersion::CheckFileVersion (LPSTR szFileName, int FileLoc, 
                                    DWORD &dwMajor, DWORD &dwMinor, 
                                    DWORD &dwBuildNumber)
{
LPSTR   lpVersion;			            // String pointer to 'version' text
//UINT    uVersionLen;
DWORD   dwVerHnd=0;			            // An 'ignored' parameter, always '0'
//VS_FIXEDFILEINFO vsFileInfo;

    FixFilePath (szFileName, FileLoc);  // Add necessary path prefix to file name

    DWORD dwVerInfoSize = GetFileVersionInfoSize (szFileName, &dwVerHnd);
    if (!dwVerInfoSize)     // Cannot reach the DLL file
        return FALSE;

    LPSTR lpstrVffInfo = 
           (LPSTR) malloc (dwVerInfoSize);  // Alloc memory for file info
    if (lpstrVffInfo == NULL)
        return FALSE;   // Allocation failed

        // Try to get the info
    if (!GetFileVersionInfo(szFileName, dwVerHnd, dwVerInfoSize, lpstrVffInfo)) {
        free (lpstrVffInfo);
        return FALSE;   // Cannot read the file information - 
                        // wierd, since we could read the information size
    }
		/* The below 'hex' value looks a little confusing, but
		   essentially what it is, is the hexidecimal representation
		   of a couple different values that represent the language
		   and character set that we are wanting string values for.
		   040904E4 is a very common one, because it means:
			 US English, Windows MultiLingual characterset
		   Or to pull it all apart:
		   04------        = SUBLANG_ENGLISH_USA
		   --09----        = LANG_ENGLISH
		   ----04E4 = 1252 = Codepage for Windows:Multilingual
        */
	/*static char fileVersion[256];
	LPVOID version=NULL;
	DWORD vLen,langD;
	BOOL retVal;

	sprintf(fileVersion,"\\VarFileInfo\\Translation");
	retVal = VerQueryValue (    lpstrVffInfo,  
                fileVersion, &version, (UINT *)&uVersionLen);
	if (retVal && vLen==4)
	{
			memcpy(&langD,version,4);
			sprintf(fileVersion,"\\StringFileInfo\\%02X%02X%02X%02X\\FileVersion",(langD & 0xff00)>>8,langD & 0xff,(langD & 0xff000000)>>24, (langD & 0xff0000)>>16);			
	}
	else 
		sprintf(fileVersion,"\\StringFileInfo\\%04X04B0\\FileVersion",GetUserDefaultLangID());

	if (!VerQueryValue (    lpstrVffInfo, fileVersion, 
                (LPVOID *)&lpVersion, (UINT *)&uVersionLen))
		{	
		free (lpstrVffInfo);
		return FALSE;     // Query was unsuccessful	
		}
	*/
    // Now we have a string that looks like this :
    // "MajorVersion.MinorVersion.BuildNumber", so let's parse it
	lpVersion = getVersion(szFileName);
	m_stFullVersion = getVersion(szFileName);
    BOOL bRes = ParseVersionString (lpVersion, dwMajor, dwMinor, dwBuildNumber);
    if(!bRes)
		// Lets try for commas
		bRes = ParseVersionString1 (lpVersion, dwMajor, dwMinor, dwBuildNumber);
	free (lpstrVffInfo);
    return bRes;
}

/***************************************************************************

   Function:   ParseVersionString

   Purpose:    Parse version information string into 3 different numbers

   Input:      The version string formatted as "MajorVersion.MinorVersion.BuildNumber"
               Reference to Major number 
               Reference to Minor number 
               Reference to Build number 

   Output:     TRUE only if successful

   Remarks:    

****************************************************************************/
BOOL CDLLVersion::ParseVersionString (LPSTR lpVersion, 
                                      DWORD &dwMajor, DWORD &dwMinor, 
                                      DWORD &dwBuildNumber)
{
        // Get first token (Major version number)
    LPSTR token = strtok( lpVersion, TEXT (".") );  
    if (token == NULL)  // End of string
        return FALSE;       // String ended prematurely
    dwMajor = atoi (token);

    token = strtok (NULL, TEXT ("."));  // Get second token (Minor version number)
    if (token == NULL)  // End of string
        return FALSE;       // String ended prematurely
    dwMinor = atoi (token);

    token = strtok (NULL, TEXT ("."));  // Get third token (Build number)
    if (token == NULL)  // End of string
        return FALSE;       // String ended prematurely
    dwBuildNumber = atoi (token);

    return TRUE;
}


/***************************************************************************

   Function:   FixFilePath

   Purpose:    Adds the correct path string to a file name according 
               to given file location 

   Input:      Original file name
               File location (Windows dir, System dir, Current dir or none)

   Output:     TRUE only if successful

   Remarks:    Trashes original file name

****************************************************************************/
BOOL CDLLVersion::FixFilePath (char * szFileName, int FileLoc)
{
    char    szPathStr [_MAX_PATH];      // Holds path prefix
   
    switch (FileLoc) {
        case WIN_DIR:
                // Get the name of the windows directory
            if (GetWindowsDirectory (szPathStr, _MAX_PATH) ==  0)  
                return FALSE;   // Cannot get windows directory
            break;

        case SYS_DIR:
                // Get the name of the windows SYSTEM directory
            if (GetSystemDirectory (szPathStr, _MAX_PATH) ==  0)  
                return FALSE;   // Cannot get system directory
            break;

        case CUR_DIR:
                // Get the name of the current directory
            if (GetCurrentDirectory (_MAX_PATH, szPathStr) ==  0)  
                return FALSE;   // Cannot get current directory
            break;

        case NO_DIR:
            lstrcpy (szPathStr,"");
            break;

        default:
            return FALSE;
    }
    lstrcat (szPathStr, _T("\\"));
    lstrcat (szPathStr, szFileName);
    lstrcpy (szFileName, szPathStr);
    return TRUE;
}            



/***************************************************************************

   Function:   ParseVersionString

   Purpose:    Parse version information string into 3 different numbers

   Input:      The version string formatted as "MajorVersion.MinorVersion.BuildNumber"
               Reference to Major number 
               Reference to Minor number 
               Reference to Build number 

   Output:     TRUE only if successful

   Remarks:    

****************************************************************************/
BOOL CDLLVersion::ParseVersionString1 (LPSTR lpVersion, 
                                      DWORD &dwMajor, DWORD &dwMinor, 
                                      DWORD &dwBuildNumber)
{
        // Get first token (Major version number)
    LPSTR token = strtok( lpVersion, TEXT (",") );  
    if (token == NULL)  // End of string
        return FALSE;       // String ended prematurely
    dwMajor = atoi (token);

    token = strtok (NULL, TEXT (","));  // Get second token (Minor version number)
    if (token == NULL)  // End of string
        return FALSE;       // String ended prematurely
    dwMinor = atoi (token);

    token = strtok (NULL, TEXT (","));  // Get third token (Build number)
    if (token == NULL)  // End of string
        return FALSE;       // String ended prematurely
    dwBuildNumber = atoi (token);

    return TRUE;
}



// a static buffer is used to hold the version, calling this function
// a second time will erase previous information.
// long paths may be used for this function.
char *CDLLVersion::getVersion(char *fileName) {
	DWORD vSize;
	DWORD vLen,langD;
	BOOL retVal;	
	
	LPVOID version=NULL;
	LPVOID versionInfo=NULL;
	static char fileVersion[256];
	bool success = true;
	vSize = GetFileVersionInfoSize(fileName,&vLen);
	if (vSize) {
			versionInfo = malloc(vSize+1);
			if (GetFileVersionInfo(fileName,vLen,vSize,versionInfo))
			{			
			sprintf(fileVersion,"\\VarFileInfo\\Translation");
			retVal = VerQueryValue(versionInfo,fileVersion,&version,(UINT *)&vLen);						
			if (retVal && vLen==4) {
			memcpy(&langD,version,4);			
			sprintf(fileVersion,"\\StringFileInfo\\%02X%02X%02X%02X\\FileVersion",(langD & 0xff00)>>8,langD & 0xff,(langD & 0xff000000)>>24, (langD & 0xff0000)>>16);			
			}
			else sprintf(fileVersion,"\\StringFileInfo\\%04X04B0\\FileVersion",GetUserDefaultLangID());
			retVal = VerQueryValue(versionInfo,fileVersion,&version,(UINT *)&vLen);
			if (!retVal) success = false;
			}
			else success = false;
		}
	else success=false;	
	
	if (success) {
		if (vLen<256)
			strcpy(fileVersion,(char *)version);
		else {
			strncpy(fileVersion,(char *)version,250);
			fileVersion[250]=0;			
		}
		if (versionInfo) free(versionInfo);
		versionInfo = NULL;
		return fileVersion;
	}
	else {
		if (versionInfo) free(versionInfo);
		versionInfo = NULL;
		return NULL;	
	}
}

