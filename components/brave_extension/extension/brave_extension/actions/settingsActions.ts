/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Types
import * as types from '../constants/settingsTypes'
import * as actions from '../types/actions/settingsActions'
import { SettingsOptions, OneOfSettings, SettingsData } from '../types/other/settingsTypes'

// Helpers
import * as shieldsAPI from '../background/api/shieldsAPI'
import * as settingsUtils from '../helpers/settingsUtils'
import { areObjectsEqual } from '../helpers/objectUtils'

/**
 * Inform the store that settings have changed. This action is used only
 * for storing values in Redux and does not tell which brave settings have changed.
 */
export const setStoreSettingsChange: actions.SetStoreSettingsChange = (settingsData) => {
  return {
    type: types.SET_STORE_SETTINGS_CHANGE,
    settingsData
  }
}

/**
 * Perform an update in settings both in brave://settings and Shields store whenever a setting change.
 * This action is bounded to the settings listener and should not be used outside this scope.
 */
export const settingsDidChange: actions.SettingsDidChange = (settings) => {
  const settingsOptions: SettingsOptions = settingsUtils.settingsOptions
  const currentSetting: Partial<OneOfSettings> = settingsOptions[settings.key]
  return dispatch => {
    shieldsAPI.setViewPreferences({ [currentSetting]: settings.value })
      .then(
        () => dispatch(setStoreSettingsChange({ [currentSetting]: settings.value })),
        error => console.error('[Shields] error dispatching settings change', error)
      )
  }
}

/**
 * Get a list of settings values from brave://settings and update if comparison
 * against settings values from store deosn't match.
 */
export const settingsDataShouldUpdate: actions.SettingsDataShouldUpdate = () => {
  return (dispatch, getState) => {
    const settingsDataFromStore: SettingsData = getState().shieldsPanel.settingsData
    shieldsAPI.getViewPreferences()
      .then(
        (settingsData: SettingsData) => {
          if (!areObjectsEqual(settingsDataFromStore, settingsData)) {
            dispatch(setStoreSettingsChange(settingsData))
          }
        },
        error => console.error('[Shields] error updating settings data', error)
      )
  }
}
