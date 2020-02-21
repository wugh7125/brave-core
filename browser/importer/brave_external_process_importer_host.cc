/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/bind.h"
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/task/post_task.h"
#include "base/task/task_traits.h"
#include "brave/browser/importer/brave_external_process_importer_host.h"
#include "brave/browser/importer/brave_importer_lock_dialog.h"
#include "brave/browser/importer/brave_profile_lock.h"
#include "brave/browser/importer/chrome_profile_lock.h"
#include "brave/common/importer/chrome_importer_utils.h"
#include "brave/common/importer/importer_constants.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "chrome/browser/extensions/webstore_install_with_prompt.h"
#include "chrome/common/extensions/webstore_install_result.h"
#endif

namespace {

#if BUILDFLAG(ENABLE_EXTENSIONS)
base::Optional<base::Value> GetChromeExtensionsList(
    const base::FilePath& secured_preference_path) {
  if (!base::PathExists(secured_preference_path))
    return base::nullopt;

  std::string secured_preference_content;
  base::ReadFileToString(secured_preference_path, &secured_preference_content);
  base::Optional<base::Value> secured_preference =
      base::JSONReader::Read(secured_preference_content);
  if (auto* extensions = secured_preference->FindPath(
          kChromeExtensionsListPath)) {
    return extensions->Clone();
  }
  return base::nullopt;
}

// Silent installer via websotre w/o any prompt or bubble.
class WebstoreInstallerForImporting
    : public extensions::WebstoreInstallWithPrompt {
 public:
  using WebstoreInstallWithPrompt::WebstoreInstallWithPrompt;

 private:
  ~WebstoreInstallerForImporting() override {}

  std::unique_ptr<ExtensionInstallPrompt::Prompt>
      CreateInstallPrompt() const override {
    return nullptr;
  }
  bool ShouldShowAppInstalledBubble() const override { return false; }
  bool ShouldShowPostInstallUI() const override { return false; }
};
#endif

}  // namespace

BraveExternalProcessImporterHost::BraveExternalProcessImporterHost()
    : ExternalProcessImporterHost(),
      weak_ptr_factory_(this) {}
BraveExternalProcessImporterHost::~BraveExternalProcessImporterHost() = default;

void BraveExternalProcessImporterHost::ShowWarningDialog() {
  DCHECK(!headless_);
  brave::importer::ShowImportLockDialog(
      parent_window_, source_profile_,
      base::Bind(&BraveExternalProcessImporterHost::OnImportLockDialogEnd,
                 weak_ptr_factory_.GetWeakPtr()));
}

void BraveExternalProcessImporterHost::OnImportLockDialogEnd(bool is_continue) {
  if (is_continue) {
    // User chose to continue, then we check the lock again to make sure that
    // the other browser has been closed. Try to import the settings if
    // successful. Otherwise, show a warning dialog.
    browser_lock_->Lock();
    if (browser_lock_->HasAcquired()) {
      is_source_readable_ = true;
      LaunchImportIfReady();
    } else {
      ShowWarningDialog();
    }
  } else {
    NotifyImportEnded();
  }
}

bool BraveExternalProcessImporterHost::CheckForChromeOrBraveLock() {
  if (!(source_profile_.importer_type == importer::TYPE_CHROME ||
        source_profile_.importer_type == importer::TYPE_BRAVE))
    return true;

  DCHECK(!browser_lock_.get());

  if (source_profile_.importer_type == importer::TYPE_CHROME) {
    // Extract the user data directory from the path of the profile to be
    // imported, because we can only lock/unlock the entire user directory with
    // ProcessSingleton.
    base::FilePath user_data_dir = source_profile_.source_path.DirName();
    browser_lock_.reset(new ChromeProfileLock(user_data_dir));
  } else {  // source_profile_.importer_type == importer::TYPE_BRAVE
    browser_lock_.reset(new BraveProfileLock(source_profile_.source_path));
  }

  browser_lock_->Lock();
  if (browser_lock_->HasAcquired())
    return true;

  // If fail to acquire the lock, we set the source unreadable and
  // show a warning dialog, unless running without UI (in which case the import
  // must be aborted).
  is_source_readable_ = false;
  if (headless_)
    return false;

  ShowWarningDialog();
  return true;
}

#if BUILDFLAG(ENABLE_EXTENSIONS)
void BraveExternalProcessImporterHost::LaunchExtensionsImport() {
  DCHECK_EQ(importer::TYPE_CHROME, source_profile_.importer_type);

  const base::FilePath pref_file = source_profile_.source_path.AppendASCII(
      kChromeSecurePreferencesFile);
  base::PostTaskAndReplyWithResult(
      FROM_HERE,
      {base::ThreadPool(), base::MayBlock(), base::TaskPriority::USER_VISIBLE,
       base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
      base::BindOnce(&GetChromeExtensionsList, pref_file),
      base::BindOnce(
          &BraveExternalProcessImporterHost::OnGetChromeExtensionsList,
          weak_ptr_factory_.GetWeakPtr()));
}

void BraveExternalProcessImporterHost::OnGetChromeExtensionsList(
    base::Optional<base::Value> extensions_list) {
  if (!extensions_list || !extensions_list->is_dict()) {
    ExternalProcessImporterHost::NotifyImportEnded();
    return;
  }

  const auto ids =
      GetImportableListFromChromeExtensionsList(extensions_list.value());
  for (const auto& id : ids) {
    scoped_refptr<WebstoreInstallerForImporting> installer =
        // Ignore installation result.
        // TODO(simonhong): Should we check install failure and retry?
        new WebstoreInstallerForImporting(
            id, profile_,
            base::BindOnce([] (bool, const std::string&,
                               extensions::webstore_install::Result) {}));
    installer->BeginInstall();
  }

  if (!ids.empty() && observer_)
    observer_->ImportItemEnded(importer::EXTENSIONS);

  ExternalProcessImporterHost::NotifyImportEnded();
}

void BraveExternalProcessImporterHost::NotifyImportEnded() {
  // If user chooses extension importing, start importing extensions.
  // and NotifyImportEnded() will be called from OnGetChromeExtensionsList().
  // Handling extensions importing after finishing all other properties makes
  // logic simpler.
  // Don't import if cancelled.
  if (!cancelled_ && (items_ & importer::EXTENSIONS)) {
    LaunchExtensionsImport();
    return;
  }

  // Otherwise, notifying here and importing is finished.
  ExternalProcessImporterHost::NotifyImportEnded();
}
#endif
