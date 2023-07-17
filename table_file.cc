#include "table_file.h"
#include "avid_errorno.h"
#include "file_util.h"

File TableFileImpl::create(PSI_file_key key, const char *tableFilePath) {
  File newTableFile;
  newTableFile = FileUtil::create(key, tableFilePath, 0, O_RDWR | O_TRUNC, MYF(0));
  if (newTableFile < 0) {
    return CANNOT_CREATE_TABLE_FILE;
  }
  return newTableFile;
}


File TableFileImpl::truncate(PSI_file_key key, const char *tableFilePath) {
  // TODO: reset SDI
  File newTableFile;
  int err = FileUtil::remove(key, tableFilePath, MYF(0));
  if (err < 0) {
    return CANNOT_DELETE_TABLE_FILE;
  }

  newTableFile = FileUtil::create(key, tableFilePath, 0, O_RDWR | O_TRUNC, MYF(0));
  if (newTableFile < 0) {
    return CANNOT_CREATE_TABLE_FILE;
  }

  return newTableFile;
}
File TableFileImpl::open(PSI_file_key key, const char *tableFilePath) {
  return mysql_file_open(key, tableFilePath, O_RDWR, MYF(0));
}

int TableFileImpl::remove(PSI_file_key key, const char *tableFilePath) {
  return FileUtil::remove(key, tableFilePath, MYF(0));
}

int TableFileImpl::close(File file) {
  return FileUtil::close(file, MYF(0));
}
