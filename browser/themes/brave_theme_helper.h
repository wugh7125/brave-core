/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_THEMES_BRAVE_THEME_HELPER_H_
#define BRAVE_BROWSER_THEMES_BRAVE_THEME_HELPER_H_

#include "base/optional.h"
#include "chrome/browser/themes/theme_helper.h"

class Profile;

class BraveThemeHelper : public ThemeHelper {
 public:
  BraveThemeHelper() = default;
  ~BraveThemeHelper() override;

  BraveThemeHelper(const BraveThemeHelper&) = delete;
  BraveThemeHelper& operator=(const BraveThemeHelper&) = delete;

  void SetProfile(Profile* profile);

 protected:
  // ThemeHelper overrides:
  SkColor GetDefaultColor(
      int id,
      bool incognito,
      const CustomThemeSupplier* theme_supplier) const override;

  base::Optional<SkColor> GetOmniboxColor(
      int id,
      bool incognito,
      const CustomThemeSupplier* theme_supplier,
      bool* has_custom_color) const override;

 private:
  Profile* profile_;
};

#endif  // BRAVE_BROWSER_THEMES_BRAVE_THEME_HELPER_H_
