//
// Created by lrf141 on 9/12/23.
//
#include "system_tablespace.h"
#include <cassert>

PSI_file_key system_tablespace_key;

namespace system_table {

void SystemTablespaceImpl::incrementMaxTablespaceId() {
  systemTablespace.maxTablespaceId++;
}

tablespace_id SystemTablespaceImpl::getMaxTablespaceId() const {
  return systemTablespace.maxTablespaceId;
}

uchar *SystemTablespaceImpl::toBinary() {
  return reinterpret_cast<uchar *>(&systemTablespace);
}

SystemTablespaceHandler::SystemTablespaceHandler()
    : systemTablespace(SystemTablespaceImpl()) {
  file = file_handler::File(SYSTEM_TABLESPACE_PATH, MYF_STRICT_MODE, system_tablespace_key);
  size_t readSize = file.read(systemTablespace.toBinary(), SYSTEM_TABLESPACE_SIZE);
  assert(readSize == SYSTEM_TABLESPACE_SIZE);
}

tablespace_id SystemTablespaceHandler::getNewMaxTablespaceId() {
  systemTablespace.incrementMaxTablespaceId();
  size_t writeSize = file.write(systemTablespace.toBinary(), SYSTEM_TABLESPACE_SIZE);
  assert(writeSize == SYSTEM_TABLESPACE_SIZE);
  return systemTablespace.getMaxTablespaceId();
}

void SystemTablespaceHandler::create() {
  file_handler::File fil(SYSTEM_TABLESPACE_PATH, 0, system_tablespace_key);
  if (fil.getFileDescriptor() < 0) {
    fil = file_handler::File::create(
        SYSTEM_TABLESPACE_PATH, system_tablespace_key
    );
    SystemTablespaceImpl tablespace(0);
    fil.write(tablespace.toBinary(), SYSTEM_TABLESPACE_SIZE);
  }
}
} // namespace system_table
