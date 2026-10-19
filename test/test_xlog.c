#include "xlog.h"

int main() {
    int errCode = -42;
    XLOGV("XJNI-Test", "Verbose log test");
    XLOGD("XJNI-Test", "Debug log test");
    XLOGI("XJNI-Test", "Info log test");
    XLOGW("XJNI-Test", "Warning log test");
    XLOGE("XJNI-Test", "Error log test");
    XLOGF("XJNI-Test", "Fatal log test, errCode=%d\n", errCode);
    return 0;
}
