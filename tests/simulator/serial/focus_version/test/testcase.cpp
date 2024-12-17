#include "testing/setup-googletest.h"
#include "Kaleidoscope.h"
#include <iostream>
#include "gmock/gmock.h"  // For matchers like Eq()

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

class FocusVersionCommand : public VirtualDeviceTest {};

TEST_F(FocusVersionCommand, SendVersionCommand) {
  // Run a cycle to initialize
  RunCycle();

  // Send version command and get response using new helper
  auto response = sim_.SendFocusCommand("version");

  // Response should be exactly the version number
  EXPECT_THAT(response, ::testing::Eq("0.1.2"))
    << "Expected version response '0.1.2', got: '" << response << "'";
}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope