/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <tuple>

#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/ads_impl.h"

#include "testing/gtest/include/gtest/gtest.h"

#include "bat/ads/internal/purchase_intent/keywords.h"

using ::testing::_;

// npm run test -- brave_unit_tests --filter=AdsPurchaseIntentKeywords*

namespace ads {

const std::vector<std::string> audi_a4_segments = {
  "automotive purchase intent by make-audi",
  "automotive purchase intent by category-entry luxury car"
};
const std::vector<std::string> audi_a6_segments = {
  "automotive purchase intent by make-audi",
  "automotive purchase intent by category-mid luxury car"
};
const std::vector<std::string> no_segments = {};
const std::vector<std::tuple<std::string, std::vector<std::string>, uint8_t>>
    kTestSearchqueries = {
  std::tuple<std::string, std::vector<std::string>, uint8_t>
      ("latest audi a6 review", audi_a6_segments, 2),
  std::tuple<std::string, std::vector<std::string>, uint8_t>
      ("  \tlatest audi\na6 !?# @& review  \t  ", audi_a6_segments, 2),
  std::tuple<std::string, std::vector<std::string>, uint8_t>
      ("latest audi a4 dealer reviews", audi_a4_segments, 3),
  std::tuple<std::string, std::vector<std::string>, uint8_t>
      ("latest audi a6 ", audi_a6_segments, 1),
  std::tuple<std::string, std::vector<std::string>, uint8_t>
      ("this is a test", no_segments, 1)
};

class AdsPurchaseIntentKeywordsTest : public ::testing::Test {
 protected:
  std::unique_ptr<MockAdsClient> mock_ads_client_;
  std::unique_ptr<AdsImpl> ads_;

  AdsPurchaseIntentKeywordsTest() :
      mock_ads_client_(std::make_unique<MockAdsClient>()),
      ads_(std::make_unique<AdsImpl>(mock_ads_client_.get())) {
    // You can do set-up work for each test here
  }

  ~AdsPurchaseIntentKeywordsTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }

  // Objects declared here can be used by all tests in the test case
};

TEST_F(AdsPurchaseIntentKeywordsTest, MatchesSegmentKeywords) {
  for (const auto& search_query : kTestSearchqueries) {
    // Arrange
    auto query = std::get<0>(search_query);
    auto true_segments = std::get<1>(search_query);

    // Act
    auto matched_segments = Keywords::MatchSegmentsList(query);

    // Assert
    EXPECT_EQ(true_segments, matched_segments);
  }
}

TEST_F(AdsPurchaseIntentKeywordsTest, MatchesFunnelKeywords) {
  for (const auto& search_query : kTestSearchqueries) {
    // Arrange
    auto query = std::get<0>(search_query);
    auto actual_weight = std::get<2>(search_query);

    // Act
    u_int8_t kw_weight = Keywords::MatchFunnelList(query);

    // Assert
    EXPECT_EQ(actual_weight, kw_weight);
  }
}

}  // namespace ads
