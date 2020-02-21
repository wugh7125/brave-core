/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "bat/ledger/internal/contribution/contribution_util.h"

#include "wrapper.hpp"  // NOLINT

using challenge_bypass_ristretto::UnblindedToken;
using challenge_bypass_ristretto::VerificationKey;
using challenge_bypass_ristretto::VerificationSignature;

namespace braveledger_contribution {

ledger::ReconcileDirections
FromContributionQueuePublishersToReconcileDirections(
    ledger::ContributionQueuePublisherList list) {
  ledger::ReconcileDirections directions;

  for (auto& item : list) {
    if (!item || item->publisher_key.empty()) {
      continue;
    }

    ledger::ReconcileDirectionProperties direction;
    direction.publisher_key = item->publisher_key,
    direction.amount_percent = item->amount_percent;

    directions.push_back(direction);
  }

  return directions;
}

ledger::ReportType GetReportTypeFromRewardsType(
    const ledger::RewardsType type) {
  switch (static_cast<int>(type)) {
    case static_cast<int>(ledger::RewardsType::AUTO_CONTRIBUTE): {
      return ledger::ReportType::AUTO_CONTRIBUTION;
    }
    case static_cast<int>(ledger::RewardsType::ONE_TIME_TIP): {
      return ledger::ReportType::TIP;
    }
    case static_cast<int>(ledger::RewardsType::RECURRING_TIP): {
      return ledger::ReportType::TIP_RECURRING;
    }
    default: {
      // missing conversion, returning dummy value.
      NOTREACHED();
      return ledger::ReportType::TIP;
    }
  }
}

void GenerateSuggestion(
    const std::string& token_value,
    const std::string& public_key,
    const std::string& suggestion_encoded,
    base::Value* result) {
  UnblindedToken unblinded = UnblindedToken::decode_base64(token_value);
  VerificationKey verification_key = unblinded.derive_verification_key();
  VerificationSignature signature = verification_key.sign(suggestion_encoded);
  const std::string pre_image = unblinded.preimage().encode_base64();

  if (challenge_bypass_ristretto::exception_occurred()) {
    challenge_bypass_ristretto::TokenException e =
        challenge_bypass_ristretto::get_last_exception();
    return;
  }

  result->SetStringKey("t", pre_image);
  result->SetStringKey("publicKey", public_key);
  result->SetStringKey("signature", signature.encode_base64());
}

void GenerateSuggestionMock(
    const std::string& token_value,
    const std::string& public_key,
    const std::string& suggestion_encoded,
    base::Value* result) {
  result->SetStringKey("t", token_value);
  result->SetStringKey("publicKey", public_key);
  result->SetStringKey("signature", token_value);
}

}  // namespace braveledger_contribution
