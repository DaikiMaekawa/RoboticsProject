#include "XnErrorChecker.h"
#include <stdexcept>

void xnErrorCheck(XnStatus status){
    if(status != XN_STATUS_OK) throw std::runtime_error(xnGetStatusString(status));
}
