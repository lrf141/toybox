//
// Created by lrf141 on 9/13/23.
//
#include <gtest/gtest.h>
#include <iostream>
#include "system_tablespace.h"
#include "tablespace_type.h"

class SystemTablespaceTest : public testing::Test {
 protected:
  system_table::SystemTablespaceHandler *sut;

  void SetUp() override {
    system_table::SystemTablespaceHandler::create();
    sut = new system_table::SystemTablespaceHandler;
  }

  void TearDown() override {
    delete sut;
    sut = nullptr;
    std::remove(system_table::SYSTEM_TABLESPACE_PATH);
  }
};

TEST_F(SystemTablespaceTest, getNewMaxTablespaceId) {
  // Setup
  tablespace_id newTablespaceId = 1;

  // Exercise
  tablespace_id result = sut->getNewMaxTablespaceId();
  file_handler::File file;
  file.open(system_table::SYSTEM_TABLESPACE_PATH);
  system_table::SystemTablespaceImpl systemTablespace(0);
  size_t readSize = file.read(systemTablespace.toBinary(),
                              system_table::SYSTEM_TABLESPACE_SIZE);

  // Verify
  ASSERT_EQ(result, newTablespaceId);
  ASSERT_EQ(readSize, system_table::SYSTEM_TABLESPACE_SIZE);
  ASSERT_EQ(systemTablespace.getMaxTablespaceId(), newTablespaceId);
}
