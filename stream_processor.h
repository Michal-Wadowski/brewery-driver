#ifndef __STREAM_PROCESSOR_H__
#define __STREAM_PROCESSOR_H__


class StreamProcessor {
public:
    virtual int process(char * data, int len) = 0;
};

#endif
