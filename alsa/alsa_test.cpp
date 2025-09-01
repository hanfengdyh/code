#include <alsa/asoundlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

int main() {
    const char* filename = "test.wav";

    // 打开 WAV 文件
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file " << filename << "\n";
        return 1;
    }

    // 读取 WAV header
    uint8_t header[44];
    file.read(reinterpret_cast<char*>(header), 44);
    if (!file) {
        std::cerr << "Failed to read WAV header\n";
        return 1;
    }

    uint16_t channels = *reinterpret_cast<uint16_t*>(&header[22]);
    uint32_t sampleRate = *reinterpret_cast<uint32_t*>(&header[24]);
    uint16_t bitsPerSample = *reinterpret_cast<uint16_t*>(&header[34]);

    std::cout << "Channels: " << channels << ", Rate: " << sampleRate
              << ", Bits: " << bitsPerSample << "\n";

    if (bitsPerSample != 32) {
        std::cerr << "Only 32-bit float WAV is supported\n";
        return 1;
    }

    // 打开 ALSA PCM 设备
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    int rc = snd_pcm_open(&pcm_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        std::cerr << "Unable to open PCM device: " << snd_strerror(rc) << "\n";
        return 1;
    }

    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(pcm_handle, params);
    snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_FLOAT_LE);
    snd_pcm_hw_params_set_channels(pcm_handle, params, channels);
    snd_pcm_hw_params_set_rate_near(pcm_handle, params, &sampleRate, 0);
    snd_pcm_hw_params(pcm_handle, params);

    // 播放缓冲
    const size_t bufsize = 4096;
    std::vector<float> buffer(bufsize);

    while (!file.eof()) {
        file.read(reinterpret_cast<char*>(buffer.data()), bufsize * sizeof(float));
        std::streamsize count = file.gcount();
        if (count <= 0) break;

        int frames = count / (channels * sizeof(float));
        float* ptr = buffer.data();
        int remaining_frames = frames;

        while (remaining_frames > 0) {
            rc = snd_pcm_writei(pcm_handle, ptr, remaining_frames);
            if (rc == -EPIPE) {
                snd_pcm_prepare(pcm_handle); // under-run
            } else if (rc < 0) {
                std::cerr << "Write error: " << snd_strerror(rc) << "\n";
                break;
            } else {
                remaining_frames -= rc;
                ptr += rc * channels;
            }
        }
    }

    snd_pcm_drain(pcm_handle);
    snd_pcm_close(pcm_handle);
    std::cout << "Playback finished.\n";

    return 0;
}

