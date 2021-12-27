#ifndef __STREAM_PROCESSOR_H__
#define __STREAM_PROCESSOR_H__

#include "abstract_connection.h"

class StreamProcessor {
public:
    virtual int process(AbstractConnection * ac) = 0;
};

#endif
