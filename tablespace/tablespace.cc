//
// Created by lrf141 on 9/16/23.
//
#include "tablespace.h"
#include "file_config.h"
#include "file_util.h"

PSI_file_key tablespace_key;

namespace tablespace {

void TablespaceHeaderImpl::read(file_handler::FileDescriptor fd) {
  FileUtil::seek(fd, TABLE_SPACE_START_POSITION, SEEK_SET, MYF(file_config::MYF_STRICT_MODE));
  size_t readSize = FileUtil::read(fd, toBinary(), TABLE_SPACE_HEADER_SIZE);
  assert(readSize == TABLE_SPACE_HEADER_SIZE);
}

void TablespaceHeaderImpl::incrementPageCount() {
  tablespaceHeader.pageCount++;
}

uint64_t TablespaceHeaderImpl::getPageCount() {
  return tablespaceHeader.pageCount;
}

uchar *TablespaceHeaderImpl::toBinary() {
  return reinterpret_cast<uchar *>(&tablespaceHeader);
}

tablespace_id TablespaceHeaderImpl::getId() {
  return tablespaceHeader.id;
}

void SystemPageHeaderImpl::read(file_handler::FileDescriptor fd) {
  FileUtil::seek(fd, SYSTEM_PAGE_HEADER_START_POSITION, SEEK_SET, MYF(file_config::MYF_STRICT_MODE));
  size_t readSize = FileUtil::read(fd, toBinary(), SYSTEM_PAGE_SIZE);
  assert(readSize == SYSTEM_PAGE_SIZE);
}

uchar *SystemPageHeaderImpl::toBinary() {
  return reinterpret_cast<uchar *>(&systemPageHeader);
}

TablespaceHandler::TablespaceHandler(const char *path)
    : path(path),
      tablespaceHeader(TablespaceHeaderImpl()),
      systemPageHeader(SystemPageHeaderImpl())
{
  file = file_handler::File(path, file_config::MYF_STRICT_MODE, tablespace_key);
  size_t readSize = file.read(tablespaceHeader.toBinary(), TABLE_SPACE_HEADER_SIZE);
  assert(readSize == TABLE_SPACE_HEADER_SIZE);
  readSize = file.read(systemPageHeader.toBinary(), SYSTEM_PAGE_HEADER_SIZE);
  assert(readSize == SYSTEM_PAGE_HEADER_SIZE);
}

TablespaceHandler TablespaceHandler::create(const char *path,
                                                       tablespace_id tablespaceId) {
  file_handler::File fil = file_handler::File::create(path, tablespace_key);
  assert(fil.getFileDescriptor() > 0);
  TablespaceHeaderImpl header(tablespaceId);
  SystemPageHeaderImpl systemPage;
  size_t writeSize = fil.write(header.toBinary(),
                               TABLE_SPACE_HEADER_SIZE,
                               TABLE_SPACE_START_POSITION);
  assert(writeSize == TABLE_SPACE_HEADER_SIZE);
  writeSize = fil.write(systemPage.toBinary(),
                        SYSTEM_PAGE_SIZE,
                        SYSTEM_PAGE_HEADER_START_POSITION);
  assert(writeSize == SYSTEM_PAGE_SIZE);

  return TablespaceHandler(path, fil, header, systemPage);
}

void TablespaceHandler::remove() {
  file.remove(path);
}

TablespaceHeaderImpl& TablespaceHandler::getTablespaceHeader() {
  return tablespaceHeader;
}

SystemPageHeaderImpl& TablespaceHandler::getSystemPageHeader() {
  return systemPageHeader;
}

file_handler::FileDescriptor TablespaceHandler::getFileDescriptor() {
  return file.getFileDescriptor();
}

void TablespaceHandler::flushTablespaceHeader() {
  size_t writeSize = file.write(tablespaceHeader.toBinary(),
                                TABLE_SPACE_HEADER_SIZE,
                                TABLE_SPACE_HEADER_START_POSITION);
  assert(writeSize == TABLE_SPACE_HEADER_SIZE);
}

void TablespaceHandler::flushSystemPageHeader() {
  size_t writeSize = file.write(systemPageHeader.toBinary(),
                                SYSTEM_PAGE_SIZE,
                                SYSTEM_PAGE_HEADER_START_POSITION);
  assert(writeSize == SYSTEM_PAGE_SIZE);
}

} // namespace tablespace
