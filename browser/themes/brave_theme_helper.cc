/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/themes/brave_theme_helper.h"

#include "brave/browser/profiles/profile_util.h"
#include "brave/browser/themes/brave_dark_mode_utils.h"
#include "brave/browser/themes/theme_properties.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_properties.h"
#include "chrome/browser/ui/omnibox/omnibox_theme.h"
#include "ui/gfx/color_palette.h"
#include "ui/native_theme/native_theme.h"

#if defined(OS_LINUX)
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"
#endif

namespace {

// TODO(simonhong): Get colors from brave's palette.
// Omnibox text colors
const SkColor kDarkOmniboxText = SkColorSetRGB(0xff, 0xff, 0xff);
const SkColor kLightOmniboxText = SkColorSetRGB(0x42, 0x42, 0x42);

// Location bar colors
const SkColor kPrivateLocationBarBgBase = SkColorSetRGB(0x1b, 0x0e, 0x2c);

SkColor GetLocationBarBackground(bool dark, bool priv, bool hover) {
  return dark ? (hover ? SkColorSetRGB(0x44, 0x44, 0x44)
                       : SkColorSetRGB(0x22, 0x22, 0x22))
              : (priv ? color_utils::HSLShift(kPrivateLocationBarBgBase,
                                              {-1, -1, hover ? 0.54 : 0.52})
                      : (hover ? color_utils::AlphaBlend(
                                     SK_ColorWHITE,
                                     SkColorSetRGB(0xf3, 0xf3, 0xf3), 0.7f)
                               : SK_ColorWHITE));
}

// Omnibox result bg colors
SkColor GetOmniboxResultBackground(int id, bool dark, bool priv) {
  // For high contrast, selected rows use inverted colors to stand out more.
  ui::NativeTheme* native_theme = ui::NativeTheme::GetInstanceForNativeUi();
  bool high_contrast = native_theme && native_theme->UsesHighContrastColors();
  OmniboxPartState state = OmniboxPartState::NORMAL;
  if (id == ThemeProperties::COLOR_OMNIBOX_RESULTS_BG_HOVERED) {
    state = OmniboxPartState::HOVERED;
  } else if (id == ThemeProperties::COLOR_OMNIBOX_RESULTS_BG_SELECTED) {
    state = OmniboxPartState::SELECTED;
  }
  return color_utils::BlendTowardMaxContrast(
      dark
          ? (high_contrast ? gfx::kGoogleGrey900 : gfx::kGoogleGrey800)
          : (priv ? color_utils::HSLShift(kPrivateLocationBarBgBase,
                                          {-1, -1, high_contrast ? 0.45 : 0.56})
                  : SK_ColorWHITE),
      gfx::ToRoundedInt(GetOmniboxStateOpacity(state) * 0xff));
}

}  // namespace

BraveThemeHelper::~BraveThemeHelper() = default;

void BraveThemeHelper::SetProfile(Profile* profile) {
  profile_ = profile;
}

SkColor BraveThemeHelper::GetDefaultColor(
    int id,
    bool incognito,
    const CustomThemeSupplier* theme_supplier) const {
  DCHECK(profile_);
#if defined(OS_LINUX)
  // IF gtk theme is selected, respect it.
  if (ThemeServiceFactory::GetForProfile(profile_)->UsingSystemTheme()) {
    return ThemeHelper::GetDefaultColor(id, incognito, theme_supplier);
  }
#endif
  // Brave Tor profiles are always 'incognito' (for now)
  if (!incognito &&
      (brave::IsTorProfile(profile_) || brave::IsGuestProfile(profile_))) {
    incognito = true;
  }
  const dark_mode::BraveDarkModeType type =
      dark_mode::GetActiveBraveDarkModeType();
  const base::Optional<SkColor> braveColor =
      MaybeGetDefaultColorForBraveUi(id, incognito, type);
  if (braveColor) {
    return braveColor.value();
  }
  // Make sure we fallback to Chrome's dark theme (incognito) for our dark theme
  if (type == dark_mode::BraveDarkModeType::BRAVE_DARK_MODE_TYPE_DARK) {
    incognito = true;
  }
  return ThemeHelper::GetDefaultColor(id, incognito, theme_supplier);
}

base::Optional<SkColor> BraveThemeHelper::GetOmniboxColor(
    int id,
    bool incognito,
    const CustomThemeSupplier* theme_supplier,
    bool* has_custom_color) const {
  DCHECK(profile_);
#if defined(OS_LINUX)
  // If gtk theme is selected, respect it.
  if (ThemeServiceFactory::GetForProfile(profile_)->UsingSystemTheme()) {
    return ThemeHelper::GetOmniboxColor(id, incognito, theme_supplier,
                                        has_custom_color);
  }
#endif
  const bool dark = dark_mode::GetActiveBraveDarkModeType() ==
      dark_mode::BraveDarkModeType::BRAVE_DARK_MODE_TYPE_DARK;
  incognito = incognito || brave::IsTorProfile(profile_) ||
              brave::IsGuestProfile(profile_);
  // TODO(petemill): Get colors from color-pallete and theme constants
  switch (id) {
    case ThemeProperties::COLOR_OMNIBOX_BACKGROUND: {
      return GetLocationBarBackground(dark, incognito, /*hover*/ false);
    }
    case ThemeProperties::COLOR_OMNIBOX_BACKGROUND_HOVERED: {
      return GetLocationBarBackground(dark, incognito, /*hover*/ true);
    }
    case ThemeProperties::COLOR_OMNIBOX_TEXT: {
      return (dark || incognito) ? kDarkOmniboxText : kLightOmniboxText;
    }
    case ThemeProperties::COLOR_OMNIBOX_RESULTS_BG:
    case ThemeProperties::COLOR_OMNIBOX_RESULTS_BG_HOVERED:
    case ThemeProperties::COLOR_OMNIBOX_RESULTS_BG_SELECTED: {
      return GetOmniboxResultBackground(id, dark, incognito);
    }
    default:
      break;
  }

  // All other values, call original function
  return ThemeHelper::GetOmniboxColor(id, incognito, theme_supplier,
                                      has_custom_color);
}
