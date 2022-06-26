/*
 * Copyright 2022 Maksim Lin.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "synth.h"
#include "synth_unit.h"
#include "module.h"
#include "freqlut.h"
#include "sin.h"
#include "sawtooth.h"

//for initial onlytesting
#include "patch.h"


#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio.h"

double sample_rate = 44100.0;

char epiano[] = {
  95, 29, 20, 50, 99, 95, 0, 0, 41, 0, 19, 0, 115, 24, 79, 2, 0, 95, 20, 20,
  50, 99, 95, 0, 0, 0, 0, 0, 0, 3, 0, 99, 2, 0, 95, 29, 20, 50, 99, 95, 0, 0,
  0, 0, 0, 0, 59, 24, 89, 2, 0, 95, 20, 20, 50, 99, 95, 0, 0, 0, 0, 0, 0, 59,
  8, 99, 2, 0, 95, 50, 35, 78, 99, 75, 0, 0, 0, 0, 0, 0, 59, 28, 58, 28, 0, 96,
  25, 25, 67, 99, 75, 0, 0, 0, 0, 0, 0, 83, 8, 99, 2, 0, 94, 67, 95, 60, 50,
  50, 50, 50, 4, 6, 34, 33, 0, 0, 56, 24, 69, 46, 80, 73, 65, 78, 79, 32, 49,
  32
};

RingBuffer *ring_buffer;
SynthUnit *synth_unit;


void data_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
  //std::cout << "callback!" << frameCount << std::endl;

  // Use MSFA frames
  synth_unit->GetSamples(frameCount, (int16_t *)pOutput);

  (void)pInput;
}

int load(const char *filename) {
  uint8_t syx_data[4104];
  std::ifstream fp_in;
  fp_in.open(filename, std::ifstream::in);
  if (fp_in.fail()) {
    std::cerr << "error opening file" << std::endl;
    return 1;
  }
  fp_in.read((char *)syx_data, 4104);
  if (fp_in.fail()) {
    std::cerr << "error reading file" << std::endl;
    return 1;
  }
  ring_buffer->Write(syx_data, 4104);
  return 0;
}

int main(int argc, char **argv)
{
  printf("libmsfa sample...\n");

  ma_result result;
  ma_device_config deviceConfig;
  ma_device device;
  // ma_format format;
  // ma_uint32 channels;
  // ma_uint32 sampleRate;

  // Init MSFA engine
  SynthUnit::Init(sample_rate);
  ring_buffer = new RingBuffer();
  synth_unit = new SynthUnit(ring_buffer);

  const char *fn = "../data/rom1a.syx";
  load(fn);


  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = ma_format_s16;
  deviceConfig.playback.channels = 1;
  deviceConfig.sampleRate = ma_standard_sample_rate_44100;
  deviceConfig.dataCallback = data_callback;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
  {
    printf("Failed to open playback device.\n");
    return -3;
  }

  if (ma_device_start(&device) != MA_SUCCESS)
  {
    printf("Failed to start playback device.\n");
    ma_device_uninit(&device);
    return -4;
  }

  uint8_t midiNoteNumber = 0x4c;
  uint8_t midiNoteDown[] = {0x90, midiNoteNumber, 0x57};
  uint8_t midiNoteUp[] = {0x90, midiNoteNumber, 0x00};
  
  sleep(1);
  ring_buffer->Write(midiNoteDown, 3);
  sleep(3);
 
  ring_buffer->Write(midiNoteUp, 3);

  printf("Press Enter to quit...");
  getchar();

  ma_device_uninit(&device);

  return 0;
}