/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/no_destructor.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"

#if defined(OS_WIN)
#include "brave/browser/themes/brave_theme_helper_win.h"
#else
#include "brave/browser/themes/brave_theme_helper.h"
#endif

#if !defined(USE_X11)
namespace {

// Forward declare the original function.
const ThemeHelper& GetThemeHelper();

const ThemeHelper& GetBraveThemeHelper(Profile* profile) {
#if defined(OS_WIN)
  using BraveThemeHelper = BraveThemeHelperWin;
#endif
  // Prevent unused function error for the original function.
  const ThemeHelper& unused = GetThemeHelper();
  (void)unused;

  // Create BraveThemeHelper and pass the profile into it.
  static base::NoDestructor<std::unique_ptr<ThemeHelper>> theme_helper(
      std::make_unique<BraveThemeHelper>());
  (static_cast<BraveThemeHelper*>(theme_helper.get()->get()))
      ->SetProfile(profile);
  return **theme_helper;
}

}  // namespace

#include "brave/browser/themes/brave_theme_service.h"
#define ThemeService BraveThemeService
#define BRAVE_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR \
  GetBraveThemeHelper(static_cast<Profile*>(profile))
#else
#define BRAVE_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR \
  GetThemeHelper()
#endif

#include "../../../../../chrome/browser/themes/theme_service_factory.cc"
