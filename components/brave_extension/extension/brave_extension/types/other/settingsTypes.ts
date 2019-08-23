/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

export type Settings = {
  key: string
  type: any
  value: boolean
}

export type SettingsData = {
  [key in OneOfSettings]: boolean
}

export type SettingsOptions = {
  [key: string]: OneOfSettings
}

export type OneOfSettings =
  'showAdvancedView' |
  'statsBadgeVisible'
