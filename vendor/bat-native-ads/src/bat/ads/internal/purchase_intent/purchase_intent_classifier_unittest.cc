/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <tuple>

#include "bat/ads/internal/ads_client_mock.h"
#include "bat/ads/internal/ads_impl.h"

#include "testing/gtest/include/gtest/gtest.h"

#include "bat/ads/internal/purchase_intent/purchase_intent_classifier.h"
#include "bat/ads/internal/purchase_intent/funnel_sites.h"

using ::testing::_;

// npm run test -- brave_unit_tests --filter=AdsPurchaseIntentClassifier*

namespace ads {

// TODO(MH): Add more test cases
const std::vector<std::string> audi_a6_segments = {
  "automotive purchase intent by make-audi",
  "automotive purchase intent by category-mid luxury car"
};
const std::vector<std::string> no_segments = {};
const std::vector<std::tuple<std::string, std::vector<std::string>, uint8_t>> kTestSearchQueries = {
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://yandex.com/search/?text=audi%20a6%20review%202020&lr=109565", audi_a6_segments, 1),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.google.com/search?q=audi+a6+review+2020&gs_l=psy-ab.3..0j0i22i30l3.26031.26031..26262...0.0..0.82.82.1......0....2j1..gws-wiz.MZlXqcvydls&ved=0ahUKEwjAjpziu8fnAhVLzYUKHSriDZMQ4dUDCAg&uact=5", audi_a6_segments, 1),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.google.com/search?q=audi+a6+review+2020&oq=audi&aqs=chrome.1.69i59l2j69i57j69i60l3.2273j0j1&sourceid=chrome&ie=UTF-8", audi_a6_segments, 1),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.bing.com/search?q=audi+a6+review+2020&qs=HS&pq=audi+a&sc=8-6&cvid=68F9883A6926440F8F6CCCBCDB87A7AA&FORM=QBLH&sp=1", audi_a6_segments, 1),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://duckduckgo.com/?q=audi+a6+dealer+reviews&t=h_&ia=web", audi_a6_segments, 3),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://search.yahoo.com/search?p=audi+a6+review+2020&fr=sfp&iscqry=", audi_a6_segments, 1),
  // std::tuple<std::string, std::vector<std::string>, uint8_t>("https://uk.search.yahoo.com/search?p=audi+a6+dealership+reviews&fr=yfp-t&fp=1&toggle=1&cop=mss&ei=UTF-8", audi_a6_segments, 1), // TODO: see `search_providers.h` for comments
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://fireball.com/search?q=audi+a6+review", audi_a6_segments, 1),
  // std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.google.de/search?source=hp&ei=GdRDXoy0EsGXkwX_37mABw&q=audi+a6+review&oq=audi+a6+review&gs_l=psy-ab.3..0l10.1808.4567..4710...0.0..0.74.661.14......0....1..gws-wiz.......0i131.IaqaaGYkPnc&ved=0ahUKEwjMpPzY58vnAhXBy6QKHf9vDnAQ4dUDCAg&uact=5", audi_a6_segments, 1), // TODO: see `search_providers.h` for comments
  // std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.startpage.com/do/search", audi_a6_segments, 1), // TODO: see `search_providers.h` for comments
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.ecosia.org/search?q=audi+a6+review", audi_a6_segments, 1),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.kbb.com/bmw/6-series/2017/styles/?intent=trade-in-sell&mileage=100000", _segments, 3),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.cars.com/for-sale/searchresults.action/?mkId=20050&rd=10&searchSource=QUICK_FORM&zc=10001", _segments, 3),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://www.google.com/search?source=hp&ei=lY5BXpenN-qUlwSd7bvICQ&q=foo+bar&oq=foo+bar&gs_l=psy-ab.3..0l10.1452.2016..2109...0.0..0.57.381.7......0....1..gws-wiz.......0i131j0i10.CeBo7A4BiSM&ved=0ahUKEwjXxbuPvcfnAhVqyoUKHZ32DpkQ4dUDCAg&uact=5", no_segments, 0),
  std::tuple<std::string, std::vector<std::string>, uint8_t>("https://creators.brave.com/", no_segments, 0)
};
class AdsPurchaseIntentClassifierTest : public ::testing::Test {
 protected:
  std::unique_ptr<MockAdsClient> mock_ads_client_;
  std::unique_ptr<AdsImpl> ads_;

  AdsPurchaseIntentClassifierTest() :
      mock_ads_client_(std::make_unique<MockAdsClient>()),
      ads_(std::make_unique<AdsImpl>(mock_ads_client_.get())) {
    // You can do set-up work for each test here
  }

  ~AdsPurchaseIntentClassifierTest() override {
    // You can do clean-up work that doesn't throw exceptions here
  }

  // If the constructor and destructor are not enough for setting up and
  // cleaning up each test, you can use the following methods

  void SetUp() override {
    // Code here will be called immediately after the constructor (right before
    // each test)
    purchase_intent_classifier_ = std::make_unique<PurchaseIntentClassifier>();
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right before the
    // destructor)
  }

  // Objects declared here can be used by all tests in the test case
  std::unique_ptr<PurchaseIntentClassifier> purchase_intent_classifier_;
};

TEST_F(AdsPurchaseIntentClassifierTest, ExtractsIntentSignal) {
  for (const auto& test_search_query: kTestSearchQueries) {
    // Arrange
    auto url = std::get<0>(test_search_query);
    auto segments = std::get<1>(test_search_query);
    auto weight = std::get<2>(test_search_query);

    // Act
    std::cout << "[TEST] Search URL: " << url << "\n";
    IntentSignalInfo intent_signal = purchase_intent_classifier_->ExtractIntentSignal(url);

    // Assert
    EXPECT_EQ(segments, intent_signal.segments);
    EXPECT_EQ(weight, intent_signal.strength);
  }
}

}  // namespace ads
