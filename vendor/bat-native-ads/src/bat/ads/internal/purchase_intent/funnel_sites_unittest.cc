/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <tuple>

#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/ads_impl.h"

#include "testing/gtest/include/gtest/gtest.h"

#include "bat/ads/internal/purchase_intent/funnel_sites.h"

using ::testing::_;

// npm run test -- brave_unit_tests --filter=AdsPurchaseIntentFunnelSites*

namespace ads {

// TODO(MH): Add more test cases
const std::vector<std::tuple<std::string, FunnelSiteInfo>> kTestUrls = {
  std::tuple<std::string, FunnelSiteInfo>("http://www.carmax.com", _automotive_funnel_sites.at(1)),
  std::tuple<std::string, FunnelSiteInfo>("http://www.carmax.com/foobar", _automotive_funnel_sites.at(1)),
  std::tuple<std::string, FunnelSiteInfo>("http://carmax.com", _automotive_funnel_sites.at(1)),
  std::tuple<std::string, FunnelSiteInfo>("http://brave.com/foobar", FunnelSiteInfo()),
};

class AdsPurchaseIntentFunnelSitesTest : public ::testing::Test {
 protected:
  std::unique_ptr<MockAdsClient> mock_ads_client_;
  std::unique_ptr<AdsImpl> ads_;

  AdsPurchaseIntentFunnelSitesTest() :
      mock_ads_client_(std::make_unique<MockAdsClient>()),
      ads_(std::make_unique<AdsImpl>(mock_ads_client_.get())) {
    // You can do set-up work for each test here
  }

  ~AdsPurchaseIntentFunnelSitesTest() override {
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

TEST_F(AdsPurchaseIntentFunnelSitesTest, MatchesFunnelSites) {  
  for (const auto& test_url: kTestUrls) {
    // Arrange
    std::string url = std::get<0>(test_url);
    FunnelSiteInfo funnel_site = std::get<1>(test_url);

    // Act
    const FunnelSiteInfo matched_site = FunnelSites::MatchFunnelSite(url);

    // Assert
    EXPECT_EQ(funnel_site.weight, matched_site.weight);
    EXPECT_EQ(funnel_site.url_netloc, matched_site.url_netloc);
  }
}

}  // namespace ads
