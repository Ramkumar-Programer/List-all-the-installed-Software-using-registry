#include <iostream>
#include <Windows.h>

using namespace std;


int registry_read(HKEY Rootkey ,string subkey, LPCTSTR name, DWORD type)
{
    LPCTSTR sub = subkey.c_str();
    HKEY key;                                                                  // handle 
    TCHAR value[255];                                                         // value of registry is store in it
    DWORD value_length = 255;                                                 // length of the value
    RegOpenKey(Rootkey, sub, &key);                             // to open the registry using hkey and subkey then in store in key

    

    if (RegQueryValueEx( key, name, NULL, &type, (LPBYTE)&value, &value_length ) == ERROR_SUCCESS)
    {
        //cout << "Sorry" << endl;
        RegCloseKey(key);
        return 1;
    }

     else if(RegQueryValueEx( key, "DisplayName", NULL, &type, (LPBYTE)&value, &value_length ) == ERROR_SUCCESS){
        //RegQueryValueEx( key, "DisplayName", NULL, &type, (LPBYTE)&value, &value_length );
          //registry_read("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{56070933-B0D1-493F-8C12-4F7E83CA3071}", , );  
                                   // printing the value
        RegCloseKey(key);
        std::cout << value << std:: endl; 
        return 1;
     }
     RegCloseKey(key);
     return 0;
     
}


void EnumerateSubKeys(HKEY RootKey, LPCTSTR subKey) 
{
 HKEY hKey;
    DWORD cSubKeys;        //Used to store the number of Subkeys
    DWORD maxSubkeyLen;    //Longest Subkey name length
    DWORD cValues;        //Used to store the number of Subkeys
    DWORD maxValueLen;    //Longest Subkey name length
    DWORD retCode;        //Return values of calls
    
    RegOpenKeyEx(RootKey, subKey, 0, KEY_ALL_ACCESS, &hKey);

    RegQueryInfoKey(hKey,            // key handle
                    NULL,            // buffer for class name
                    NULL,            // size of class string
                    NULL,            // reserved
                    &cSubKeys,        // number of subkeys
                    &maxSubkeyLen,    // longest subkey length
                    NULL,            // longest class string 
                    &cValues,        // number of values for this key 
                    &maxValueLen,    // longest value name 
                    NULL,            // longest value data 
                    NULL,            // security descriptor 
                    NULL);            // last write time

    if(cSubKeys>0)
    {
        char currentSubkey[MAX_PATH];

        for(int i=0;i < cSubKeys; i++)
        {
             DWORD currentSubLen=MAX_PATH;

            retCode=RegEnumKeyEx(hKey,    // Handle to an open/predefined key
            i,                // Index of the subkey to retrieve.
            (LPSTR)currentSubkey,            // buffer to receives the name of the subkey
            &currentSubLen,            // size of that buffer
            NULL,                // Reserved
            NULL,                // buffer for class string 
            NULL,                // size of that buffer
            NULL);                // last write time

            if(retCode==ERROR_SUCCESS)
        {
                
                //printf("(%d) %s\n", i+1, currentSubkey);

                string crtsubkey =  (string(subKey) +  "\\" + string(currentSubkey)) ;
                //cout << (string(subKey) +  "\\" + string(currentSubkey)) << endl;
                registry_read(RootKey ,crtsubkey, "SystemComponent", REG_SZ);

                char* subKeyPath = new char[currentSubLen + strlen(subKey)];
                
        EnumerateSubKeys(RootKey, subKeyPath);
   }
  }
 }
    else
 {
//   EnumerateValues(hKey, cValues);
 }

 RegCloseKey(hKey); 
}




int main()
{

    EnumerateSubKeys(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
    EnumerateSubKeys(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
    EnumerateSubKeys(HKEY_LOCAL_MACHINE,"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");

    return 0;
}