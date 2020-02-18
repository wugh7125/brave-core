/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_DATABASE_PUBLISHER_INFO_DATABASE_H_
#define BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_DATABASE_PUBLISHER_INFO_DATABASE_H_

#include <stddef.h>
#include <stdint.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/memory/memory_pressure_listener.h"
#include "base/sequence_checker.h"
#include "bat/ledger/mojom_structs.h"
#include "build/build_config.h"
#include "sql/database.h"
#include "sql/init_status.h"
#include "sql/meta_table.h"

namespace brave_rewards {

class PublisherInfoDatabase {
 public:
  PublisherInfoDatabase(
      const base::FilePath& db_path,
      const int testing_current_version = -1);
  ~PublisherInfoDatabase();

  // Call before Init() to set the error callback to be used for the
  // underlying database connection.
  void set_error_callback(const sql::Database::ErrorCallback& error_callback) {
    db_.set_error_callback(error_callback);
  }

  // Returns the current version of the publisher info database
  int GetCurrentVersion();

  void GetTransactionReport(
      ledger::TransactionReportInfoList* list,
      const ledger::ActivityMonth month,
      const int year);

  // Vacuums the database. This will cause sqlite to defragment and collect
  // unused space in the file. It can be VERY SLOW.
  void Vacuum();

  std::string GetDiagnosticInfo(int extended_error, sql::Statement* statement);

  sql::Database& GetDB();

  bool Init();

  int GetTableVersionNumber();

  std::string GetSchema();

 private:
  bool IsInitialized();

  void OnMemoryPressure(
    base::MemoryPressureListener::MemoryPressureLevel memory_pressure_level);

  bool InitMetaTable(const int version);

  sql::MetaTable& GetMetaTable();

  bool MigrateV0toV1();

  bool MigrateV1toV2();

  bool MigrateV2toV3();

  bool MigrateV3toV4();

  bool MigrateV4toV5();

  bool MigrateV5toV6();

  bool MigrateV6toV7();

  bool MigrateV7toV8();

  bool MigrateV8toV9();

  bool MigrateV9toV10();

  bool MigrateV10toV11();

  bool MigrateV11toV12();

  bool MigrateV12toV13();

  bool MigrateV13toV14();

  bool MigrateV14toV15();

  bool Migrate(int version);

  sql::InitStatus EnsureCurrentVersion(const int table_version);

  sql::Database db_;
  sql::MetaTable meta_table_;
  const base::FilePath db_path_;
  bool initialized_;
  int testing_current_version_;

  std::unique_ptr<base::MemoryPressureListener> memory_pressure_listener_;

  SEQUENCE_CHECKER(sequence_checker_);
  DISALLOW_COPY_AND_ASSIGN(PublisherInfoDatabase);
};

}  // namespace brave_rewards

#endif  // BRAVE_COMPONENTS_BRAVE_REWARDS_BROWSER_DATABASE_PUBLISHER_INFO_DATABASE_H_
