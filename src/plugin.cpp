// opusPlugin.cpp : Defines the exported functions for the DLL application.
//

#include <strsafe.h>

#include <cwctype>

#include "dopus_wstring_view_span.hh"
#include "stdafx.h"
#include "text_utils.hh"

void Plugin::SetEntryTime(EntryType* pFile, FILETIME pFT) {
  /* Not implemented */
}

LPVFSFILEDATAHEADER Plugin::GetfileInformation(std::filesystem::path path, HANDLE heap) {
  /* Not implemented */
  return nullptr;
}

LPVFSFILEDATAHEADER Plugin::GetVFSforEntry(const EntryType* pEntry, HANDLE pHeap) {
  /* Not implemented */
  return nullptr;
}

void Plugin::GetWfdForEntry(const EntryType& entry, LPWIN32_FIND_DATA pData) {
  /* Not implemented */
}

std::optional<std::filesystem::path> Plugin::LoadFile(std::filesystem::path path) {
  /* Not implemented */
  return {};
}

bool Plugin::ReadDirectory(LPVFSREADDIRDATAW lpRDD) {
  /* Not implemented */
  return {};
}

bool Plugin::ReadFile(PluginFile* pFile, std::span<uint8_t> buffer, LPDWORD pReadSize) {
  /* Not implemented */
  return {};
}

bool Plugin::WriteFile(PluginFile* pFile, std::span<uint8_t> buffer, LPDWORD pWriteSize) {
  /* Not implemented */
  return {};
}

PluginFile* Plugin::OpenFile(std::filesystem::path path, bool for_writing) {
  /* Not implemented */
  return {};
}

void Plugin::CloseFile(PluginFile* pFile) {
  /* Not implemented */
}

bool Plugin::MoveFile(std::filesystem::path old_name, std::filesystem::path new_name) {
  /* Not implemented */
  return {};
}

bool Plugin::CreateDir(std::filesystem::path path) {
  /* Not implemented */
  return {};
}

int Plugin::ContextVerb(LPVFSCONTEXTVERBDATAW lpVerbData) {
  /* Not implemented */
  return VFSCVRES_DEFAULT;
}

bool Plugin::Delete(LPVOID func_data, std::filesystem::path path, std::set<std::filesystem::path> files, bool pAll) {
  /* Not implemented */
  return {};
}

PluginFindData* Plugin::FindFirst(std::filesystem::path path, LPWIN32_FIND_DATA lpwfdData, HANDLE hAbortEvent) {
  /* Not implemented */
  return {};
}

bool Plugin::FindNext(PluginFindData* lpRAF, LPWIN32_FIND_DATA lpwfdData) {
  /* Not implemented */
  return {};
}

void Plugin::FindClose(PluginFindData* pFindData) {
  /* Not implemented */
}

int Plugin::ImportFile(LPVOID func_data, std::filesystem::path destination, std::filesystem::path source) {
  /* Not implemented */
  return {};
}

std::vector<std::wstring> directoryList(std::filesystem::path path) {
  WIN32_FIND_DATAW fdata;
  HANDLE dhandle;
  std::vector<std::wstring> results;

  // CAREFUL: this uses similarly named system functions.
  if ((dhandle = ::FindFirstFileW(path.native().data(), &fdata)) == INVALID_HANDLE_VALUE)
    return results;

  results.emplace_back(fdata.cFileName);

  while (true) {
    if (::FindNextFileW(dhandle, &fdata)) {
      results.emplace_back(fdata.cFileName);
    } else {
      if (GetLastError() == ERROR_NO_MORE_FILES) {
        break;
      } else {
        FindClose(dhandle);
        return results;
      }
    }
  }

  FindClose(dhandle);
  return results;
}

int Plugin::ImportPath(LPVOID func_data, std::filesystem::path destination, std::filesystem::path source) {
  /* Not implemented */
  return {};
}

bool Plugin::Extract(LPVOID func_data, std::filesystem::path source_path, std::filesystem::path target_path) {
  /* Not implemented */
  return {};
}

bool Plugin::ExtractPath(LPVOID func_data, std::filesystem::path source_path, std::filesystem::path target_path) {
  /* Not implemented */
  return {};
}

bool Plugin::ExtractFile(LPVOID func_data, const EntryType& pEntry, std::filesystem::path target_path) {
  /* Not implemented */
  return {};
}

size_t Plugin::GetAvailableSize() {
  /* Not implemented */
  return {};
}

size_t Plugin::GetTotalSize() {
  /* Not implemented */
  return {};
}

Guard<HANDLE> Plugin::SetAbortHandle(HANDLE& hAbortEvent) {
  return Guard<HANDLE>(mAbortEvent, hAbortEvent);
}

bool Plugin::ShouldAbort() const {
  return mAbortEvent && WaitForSingleObject(mAbortEvent, 0) == WAIT_OBJECT_0;
}

uint32_t Plugin::BatchOperation(std::filesystem::path path, LPVFSBATCHDATAW lpBatchData) {
  /* Not implemented */
  return {};
}

bool Plugin::PropGet(vfsProperty propId, LPVOID lpPropData, LPVOID lpData1, LPVOID lpData2, LPVOID lpData3) {
  switch (propId) {
    case VFSPROP_CANSHOWSUBFOLDERS:
    case VFSPROP_ISEXTRACTABLE:
      *reinterpret_cast<LPBOOL>(lpPropData) = true;
      break;

    case VFSPROP_ALLOWTOOLTIPGETSIZES:
    case VFSPROP_CANDELETESECURE:
    case VFSPROP_CANDELETETOTRASH:
    case VFSPROP_SHOWFILEINFO:
    case VFSPROP_SHOWTHUMBNAILS:
    case VFSPROP_SUPPORTFILEHASH:
    case VFSPROP_SUPPORTPATHCOMPLETION:
    case VFSPROP_USEFULLRENAME:
      *reinterpret_cast<LPBOOL>(lpPropData) = false;
      break;

    case VFSPROP_SHOWPICTURESDIRECTLY:
    case VFSPROP_SHOWFULLPROGRESSBAR:  // No progress bar even when copying.
      *reinterpret_cast<LPDWORD>(lpPropData) = false;
      break;

    case VFSPROP_DRAGEFFECTS:
      *reinterpret_cast<LPDWORD>(lpPropData) = DROPEFFECT_COPY;
      break;

    case VFSPROP_BATCHOPERATION:
      *reinterpret_cast<LPDWORD>(lpPropData) = VFSBATCHRES_HANDLED;
      break;

    case VFSPROP_GETVALIDACTIONS:
      *reinterpret_cast<LPDWORD>(lpPropData) = /* SFGAO_*/ 0;
      break;

    case VFSPROP_COPYBUFFERSIZE:
      *reinterpret_cast<LPDWORD>(lpPropData) = 64 << 20;
      break;

    case VFSPROP_FUNCAVAILABILITY:
      *reinterpret_cast<LPDWORD>(lpPropData) &=
          ~(VFSFUNCAVAIL_MOVE |
            VFSFUNCAVAIL_DELETE
            // | VFSFUNCAVAIL_GETSIZES
            | VFSFUNCAVAIL_MAKEDIR | VFSFUNCAVAIL_PRINT | VFSFUNCAVAIL_PROPERTIES | VFSFUNCAVAIL_RENAME |
            VFSFUNCAVAIL_SETATTR |
            VFSFUNCAVAIL_SHORTCUT
            //| VFSFUNCAVAIL_SELECTALL
            //| VFSFUNCAVAIL_SELECTNONE
            //| VFSFUNCAVAIL_SELECTINVERT
            | VFSFUNCAVAIL_VIEWLARGEICONS |
            VFSFUNCAVAIL_VIEWSMALLICONS
            // | VFSFUNCAVAIL_VIEWLIST
            // | VFSFUNCAVAIL_VIEWDETAILS | VFSFUNCAVAIL_VIEWTHUMBNAIL
            // | VFSFUNCAVAIL_CLIPCOPY
            | VFSFUNCAVAIL_CLIPCUT | VFSFUNCAVAIL_CLIPPASTE | VFSFUNCAVAIL_CLIPPASTESHORTCUT |
            VFSFUNCAVAIL_UNDO
            //| VFSFUNCAVAIL_SHOW
            | VFSFUNCAVAIL_DUPLICATE |
            VFSFUNCAVAIL_SPLITJOIN
            //| VFSFUNCAVAIL_SELECTRESELECT
            //| VFSFUNCAVAIL_SELECTALLFILES
            //| VFSFUNCAVAIL_SELECTALLDIRS
            //| VFSFUNCAVAIL_PLAY
            | VFSFUNCAVAIL_SETTIME | VFSFUNCAVAIL_VIEWTILE | VFSFUNCAVAIL_SETCOMMENT);
      break;

      // VFSPROP_GETFOLDERICON -> return icon file?
    default:
      return false;
  }

  return true;
}

bool Plugin::ExtractEntries(LPVOID func_data, dopus::wstring_view_span entry_names, std::filesystem::path target_path) {
  /* Not implemented */
  return {};
}

void Plugin::SetError(int error) {
  mLastError = error;
  ::SetLastError(error);
}
