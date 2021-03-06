#ifndef COMPOSERSHELL_MENU_H
#define COMPOSERSHELL_MENU_H

#include "dll.h"
#include <windows.h>
#include <shlobj.h>
#include <string>
#include <vector>

const PCWSTR COMPOSER_RUNAS = L"RunasAdministrator";

#define CST_NONE 0
#define CST_PROJECT 1
#define CST_PROJECT_INSTALLED 2
#define CST_PACKAGE 3
#define CST_PACKAGE_INSTALLED 4
#define CST_COMPOSER 1
#define CST_INSTALLED 1

#define CSX_USER 0
#define CSX_ADMIN 1

// struct for storing menu items
typedef struct _CSXINFO {
    int id;
    wchar_t title[60];
    wchar_t help[60];
    wchar_t cmd[60];
} CSXINFO;

typedef CSXINFO *LPCSXINFO;

#define CMD_SHELL 0
#define CMD_INIT 1
#define CMD_INSTALL 2
#define CMD_INSTALL_DST 3
#define CMD_INSTALL_SRC 4
#define CMD_UPDATE 5
#define CMD_UPDATE_DST 6
#define CMD_UPDATE_SRC 7
#define CMD_DUMP_AUTOLOAD 8
#define CMD_DUMP_AUTOLOAD_OPT 9
#define CMD_REQUIRE 10
#define CMD_OPTIONS 11
#define CMD_SELF_UPDATE 12
#define CMD_HELP 13
#define CMD_ADMIN 14

const CSXINFO COMPOSER_INFO[] = {
    {CMD_SHELL, L"Use Composer here", L"Open Command Prompt in this directory", L"--version"},
    {CMD_INIT, L"Composer Init", L"Configure a new package in this directory", L"init"},
    {CMD_INSTALL, L"Composer Install", L"Install dependencies in this directory", L"install"},
    {CMD_INSTALL_DST, L"Install prefer-dist", L"Install dependencies - distribution", L"install --prefer-dist"},
    {CMD_INSTALL_SRC, L"Install prefer-source", L"Install dependencies - source", L"install --prefer-source"},
    {CMD_UPDATE, L"Composer Update", L"Update dependencies in this directory", L"update"},
    {CMD_UPDATE_DST, L"Update prefer-dist", L"Update dependencies - distribution", L"update --prefer-dist"},
    {CMD_UPDATE_SRC, L"Update prefer-source", L"Install dependencies - source", L"update --prefer-source"},
    {CMD_DUMP_AUTOLOAD, L"Dump-Autoload", L"Update the autoloader for class-map dependencies", L"dump-autoload"},
    {CMD_DUMP_AUTOLOAD_OPT, L"Dump-Autoload optimize", L"Convert PSR-0 autoloading to a class-map", L"dump-autoload --optimize"},
    {CMD_OPTIONS, L"Composer Options", L"See more commands", L""},
    {CMD_SELF_UPDATE, L"Self-Update", L"Install the latest version of Composer", L"self-update"},
    {CMD_HELP, L"Show Help", L"View usage information", L""},
    {CMD_ADMIN, L"Run as admin (Install/Update)", L"Run Install/Update commands as administrator", L""}
};

// struct for holding displayed menu items 
typedef struct _CSXIDREC {
    int id;
    int info;
} CSXIDREC;


// struct for holding target directory info
typedef struct _CSXSTATUS {
    BOOLEAN invalid;
    BOOLEAN composer;
    BOOLEAN installed;
    BOOLEAN project;
    BOOLEAN package;
} CSXSTATUS;

class ComposerShellMenu : public IShellExtInit, public IContextMenu
{
public:
    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IShellExtInit
    IFACEMETHODIMP Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID);

    // IContextMenu
    IFACEMETHODIMP QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
    IFACEMETHODIMP InvokeCommand(LPCMINVOKECOMMANDINFO pici);
    IFACEMETHODIMP GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax);
    
    ComposerShellMenu(void);

protected:
    ~ComposerShellMenu(void);

private:
    void ClearStorage(); 
    BOOLEAN DirectoryExists(const std::wstring& path);
    BOOLEAN FileExists(const std::wstring& filepath);
    HRESULT GetDataFromShellItem(IShellItem* item);
    BOOLEAN GetCmdInfo(UINT csxId, LPCSXINFO csxInfo);
    BOOLEAN GetInfoFromCmdId(UINT_PTR cmdId, LPCSXINFO csxInfo);
    HRESULT GetShellItem(IShellItem ** newItem, PBOOLEAN isFile);
    HRESULT GetShellItemParent(IShellItem* child, IShellItem ** parent);
    HRESULT GetTargetData();
    HBITMAP IconToBitmap(HICON hIcon);
    HRESULT IsShellItemValid(IShellItem* item, BOOLEAN requireFolder, PBOOLEAN isFile);
    BOOLEAN MenuAdd(UINT infoId, HMENU hMenu, HMENU hSub, UINT idCmdFirst, PUINT cmdId, PUINT position, PDWORD error);
    BOOLEAN MenuBuild(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, PDWORD result);
    BOOLEAN MenuExists(HMENU menu);
    void RunasGet();
    HBITMAP RunasGetBitmap();
    void RunasInit();
    HRESULT RunasSet(DWORD value);
    HRESULT RunasToggle();
    BOOLEAN RunasUse(int cmdId);
    void SetStatus();
    std::wstring ReadComposerJson();

    long m_RefCount;
    LPITEMIDLIST m_Folder;
    LPDATAOBJECT m_Data;
    std::wstring m_TargetDir;
    CSXSTATUS m_Status;
    std::vector<CSXIDREC> m_IdList;
    CSXGLOBALS m_Globals;

};

#endif
