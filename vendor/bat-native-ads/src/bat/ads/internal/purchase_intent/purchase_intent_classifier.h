/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_ADS_INTERNAL_PURCHASE_INTENT_CLASSIFIER_H_
#define BAT_ADS_INTERNAL_PURCHASE_INTENT_CLASSIFIER_H_

#include <string>
#include <vector>

#include "bat/ads/internal/search_providers.h"
#include "bat/ads/internal/purchase_intent/purchase_intent_signal_info.h"

namespace ads {

// TODO(Moritz Haller): Rather use struct with collection of static helper methods?
class PurchaseIntentClassifier {
 public:
  PurchaseIntentClassifier();
  ~PurchaseIntentClassifier();

  PurchaseIntentSignalInfo ExtractIntentSignal(const std::string& url);
};

}  // namespace ads

#endif  // BAT_ADS_INTERNAL_PURCHASE_INTENT_CLASSIFIER_H_
