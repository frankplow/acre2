#include "FilterOcclusion.h"

#include "AcreDsp.h"
#include "Log.h"

#include <cmath>
#include <math.h>

#define TS_SAMPLE_RATE_Hz 48000
#define CUTTOFF_FREQ_Hz    15000

ACRE_RESULT CFilterOcclusion::process(int16_t *const samples, const int32_t sampleCount, const int32_t channels, const ACRE_VOLUME volume, Dsp::Filter *&filter) {
    float32_t buffer[4096], *floatPointer[1];
    int16_t *shortPointer[1];

    if (channels != this->getChannelCount() || !filter) {
        this->setChannelCount(channels);
        //LOG("INITIALIZING FILTER COMPLETE %d, %d, %x", channels, this->getChannelCount(), filter);
        if (channels == 1) {
            filter = new Dsp::SmoothedFilterDesign
                <Dsp::Butterworth::Design::LowPass<4>, 1, Dsp::TransposedDirectFormII> (4800);
        } else if (channels == 2) {
            filter = new Dsp::SmoothedFilterDesign
                <Dsp::Butterworth::Design::LowPass<4>, 2, Dsp::TransposedDirectFormII> (4800);
        }
        if (filter) {
            Dsp::Params params;
            params[0] = TS_SAMPLE_RATE_Hz; // sample rate
            params[1] = 4;
            params[2] = CUTTOFF_FREQ_Hz; // cutoff frequency
            params[3] = 0.9; // Q
            filter->setParams(params);
        }
    }

    if (volume > 0.001f) {
        floatPointer[0] = buffer;
        shortPointer[0] = samples;
        memset(floatPointer[0], 0x00, 4096*sizeof(float));
        for (int32_t i = 0; i < sampleCount; ++i) {
            floatPointer[0][i] = static_cast<float>(samples[i]) / 32768.0f;
        }
        if (filter) {
            filter->setParam(2, ((float64_t) CUTTOFF_FREQ_Hz*(float64_t)pow(log10(volume*10.0f),4.0f)));
            filter->process(sampleCount*channels, floatPointer);
        }
        for (int32_t i = 0; i < sampleCount; ++i) {
            if (floatPointer[0][i] > 1.0f) {
                floatPointer[0][i] = 1.0f;
            }
            if (floatPointer[0][i] < -1.0f) {
                floatPointer[0][i] = -1.0f;
            }
            shortPointer[0][i] = static_cast<int16_t>(std::floor(floatPointer[0][i] * 32767.0f));
        }
    } else {
        memset(samples, 0x00, (sampleCount*channels)*sizeof(int16_t) );
    }

    return ACRE_OK;
}

CFilterOcclusion::CFilterOcclusion(void)
{
    this->setChannelCount(0);
}

CFilterOcclusion::~CFilterOcclusion(void)
{
}
