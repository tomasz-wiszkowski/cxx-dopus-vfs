#pragma once

#include <filesystem>
#include <optional>
#include <set>

#include "dopus_wstring_view_span.hh"

/// @brief Guard object to set and restore fields.
template <typename T>
class Guard {
 private:
  T& old_;
  T& new_;

 public:
  Guard(T& hold, T& hnew) : old_(hold), new_(hnew) { std::swap(old_, new_); }

  ~Guard() { std::swap(old_, new_); }

  Guard(const Guard&) = delete;
  Guard& operator=(const Guard&) = delete;
  Guard(Guard&&) = delete;
  Guard& operator=(Guard&&) = delete;
};

class PluginFindData {};

class PluginFile {};

class Plugin {
 private:
  using EntryType = void*;

  HANDLE mAbortEvent{};
  std::filesystem::path mPath;
  int mLastError{};

  LPVFSFILEDATAHEADER GetVFSforEntry(const EntryType* pEntry, HANDLE pHeap);
  void GetWfdForEntry(const EntryType& entry, LPWIN32_FIND_DATA pData);

  Guard<HANDLE> SetAbortHandle(HANDLE& hAbortEvent);
  bool ShouldAbort() const;

  FILETIME GetFileTime(const EntryType& entry);
  void SetEntryTime(EntryType* pFile, FILETIME pFT);

  void SetError(int error);

 public:
  std::optional<std::filesystem::path> LoadFile(std::filesystem::path pAfPath);

  bool ReadDirectory(LPVFSREADDIRDATAW lpRDD);
  bool ReadFile(PluginFile* pFile, std::span<uint8_t> buffer, LPDWORD readSize);
  bool WriteFile(PluginFile* pFile, std::span<uint8_t> buffer, LPDWORD writeSize);

  PluginFile* OpenFile(std::filesystem::path path, bool for_writing);
  void CloseFile(PluginFile* pFile);
  bool MoveFile(std::filesystem::path old_name, std::filesystem::path new_name);
  bool CreateDir(std::filesystem::path path);

  size_t GetAvailableSize();
  size_t GetTotalSize();

  bool Delete(LPVOID func_data, std::filesystem::path path, std::set<std::filesystem::path> files, bool pAll = false);

  PluginFindData* FindFirst(std::filesystem::path path, LPWIN32_FIND_DATA lpwfdData, HANDLE hAbortEvent);
  bool FindNext(PluginFindData* lpRAF, LPWIN32_FIND_DATA lpwfdData);
  void FindClose(PluginFindData* lpRAF);

  LPVFSFILEDATAHEADER GetfileInformation(std::filesystem::path path, HANDLE heap);

  int Import(LPVOID func_data, std::filesystem::path file, std::filesystem::path path);
  int ImportFile(LPVOID func_data, std::filesystem::path file, std::filesystem::path path);
  int ImportPath(LPVOID func_data, std::filesystem::path file, std::filesystem::path path);

  bool Extract(LPVOID func_data, std::filesystem::path source_path, std::filesystem::path target_path);
  bool ExtractFile(LPVOID func_data, const EntryType& pEntry, std::filesystem::path target_path);
  bool ExtractPath(LPVOID func_data, std::filesystem::path source_path, std::filesystem::path target_path);
  bool ExtractEntries(LPVOID func_data, dopus::wstring_view_span entry_names, std::filesystem::path target_path);

  int ContextVerb(LPVFSCONTEXTVERBDATAW lpVerbData);
  uint32_t BatchOperation(std::filesystem::path lpszPath, LPVFSBATCHDATAW lpBatchData);
  bool PropGet(vfsProperty propId, LPVOID lpPropData, LPVOID lpData1, LPVOID lpData2, LPVOID lpData3);
  int GetError() const { return mLastError; }
};
