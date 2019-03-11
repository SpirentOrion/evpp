/// \brief Create a null stream to redirect all logs,
///        thereby supressing all evpp library logging.
///

#include <ostream>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>

namespace evpp {

class EVPP_EXPORT NullStream {
public:
    NullStream() {}
    virtual ~NullStream() {}

public:
    inline static std::ostream& get() {
        static boost::iostreams::stream< boost::iostreams::null_sink > nullOstream((boost::iostreams::null_sink()));
        return nullOstream;
    }
};
}
