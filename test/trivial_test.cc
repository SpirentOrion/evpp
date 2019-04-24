#include "test_common.h"

#include <evpp/sockets.h>
#include <evpp/logging.h>

TEST_UNIT(Teststrerror) {
    EVPP_LOG_ERROR << evpp::strerror(EAGAIN);
}



