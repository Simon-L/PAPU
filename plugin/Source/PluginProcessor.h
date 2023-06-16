/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "gb_apu/Gb_Apu.h"
#include "gb_apu/Multi_Buffer.h"

// Source: https://github.com/dannye/pokered-crysaudio/blob/master/crysaudio/wave_samples.asm
static uint8_t wave_samples[15][32] = {
    { 0,  2,  4,  6,  8, 10, 12, 14, 15, 15, 15, 14, 14, 13, 13, 12, 12, 11, 10,  9,  8,  7,  6,  5,  4,  4,  3,  3,  2,  2,  1,  1 },
    { 0,  2,  4,  6,  8, 10, 12, 14, 14, 15, 15, 15, 15, 14, 14, 14, 13, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  2,  1,  1 },
    { 1,  3,  6,  9, 11, 13, 14, 14, 14, 14, 15, 15, 15, 15, 14, 13, 13, 14, 15, 15, 15, 15, 14, 14, 14, 14, 13, 11,  9,  6,  3,  1 },
    { 0,  2,  4,  6,  8, 10, 12, 13, 14, 15, 15, 14, 13, 14, 15, 15, 14, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0 },
    { 0,  1,  2,  3,  4,  5,  6,  7,  8, 10, 12, 13, 14, 14, 15,  7,  7, 15, 14, 14, 13, 12, 10,  8,  7,  6,  5,  4,  3,  2,  1,  0 },
    { 0,  0,  1,  1,  2,  2,  3,  3,  4,  4,  3,  3,  2,  2,  1,  1, 15, 15, 14, 14, 12, 12, 10, 10,  8,  8, 10, 10, 12, 12, 14, 14 },
    { 0,  2,  4,  6,  8, 10, 12, 14, 12, 11, 10,  9,  8,  7,  6,  5, 15, 15, 15, 14, 14, 13, 13, 12,  4,  4,  3,  3,  2,  2,  1,  1 },
    {12,  0, 10,  9,  8,  7, 15,  5, 15, 15, 15, 14, 14, 13, 13, 12,  4,  4,  3,  3,  2,  2, 15,  1,  0,  2,  4,  6,  8, 10, 12, 14 },
    { 4,  4,  3,  3,  2,  2,  1, 15,  0,  0,  4,  6,  8, 10, 12, 14, 15,  8, 15, 14, 14, 13, 13, 12, 12, 11, 10,  9,  8,  7,  6,  5 },
    { 1,  1,  0,  0,  0,  0,  0,  8,  0,  0,  1,  3,  5,  7,  9, 10, 11,  4, 11, 10, 10,  9,  9,  8,  8,  7,  6,  5,  4,  3,  2,  1 },
    { 7,  9, 11, 13, 15, 15, 15, 15, 15, 15, 15, 15, 15, 13, 11,  9,  7,  5,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  5 },
    { 0,  1,  1,  2,  2,  3,  3,  4,  4,  5,  5,  6,  6,  7,  7,  7,  8,  8,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15 },
    { 4,  6,  8, 10, 12, 12, 12, 12, 12, 12, 12, 12, 12, 10,  8,  6,  4,  2,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  2 },
    { 7, 10, 13, 15, 15, 15, 13, 10,  7,  4,  1,  0,  0,  0,  1,  4,  7, 10, 13, 15, 15, 15, 13, 10,  7,  4,  1,  0,  0,  0,  1,  4 },
    {14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }};

//==============================================================================
class PAPUAudioProcessor;
class PAPUEngine
{
public:
    PAPUEngine (PAPUAudioProcessor& p);

    void prepareToPlay (double sampleRate);
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi);

    void prepareBlock (juce::AudioSampleBuffer& buffer);
    void handleMessage (const juce::MidiMessage& msg);
    void runUntil (int& done, juce::AudioSampleBuffer& buffer, int pos);

    int getNote()   { return lastNote; }

    uint8_t waveIndex = 0;
    void setWave(uint8_t index);
    
private:
    int parameterIntValue (const juce::String& uid);
    void runOscs (int curNote, bool trigger);

    PAPUAudioProcessor& processor;

    int lastNote = -1;
    double pitchBend = 0;
    juce::Array<int> noteQueue;
    float freq1 = 0.0f, freq2 = 0.0f, freq3 = 0.0f;

    Gb_Apu apu;
    Stereo_Buffer buf;

    blip_time_t time = 0;

    blip_time_t clock() { return time += 4; }

    void writeReg (int reg, int value, bool force);

    std::map<int, int> regCache;
};

//==============================================================================
/**
*/
class PAPUAudioProcessorEditor;
class PAPUAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    PAPUAudioProcessor();
    ~PAPUAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioSampleBuffer&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    static juce::String paramPulse1OL;
    static juce::String paramPulse1OR;
    static juce::String paramPulse1Duty;
    static juce::String paramPulse1A;
    static juce::String paramPulse1R;
    static juce::String paramPulse1Tune;
    static juce::String paramPulse1Fine;
    static juce::String paramPulse1Sweep;
    static juce::String paramPulse1Shift;
    static juce::String paramPulse2OL;
    static juce::String paramPulse2OR;
    static juce::String paramPulse2Duty;
    static juce::String paramPulse2A;
    static juce::String paramPulse2R;
    static juce::String paramPulse2Tune;
    static juce::String paramPulse2Fine;
    static juce::String paramNoiseOL;
    static juce::String paramNoiseOR;
    static juce::String paramNoiseA;
    static juce::String paramNoiseR;
    static juce::String paramNoiseShift;
    static juce::String paramNoiseStep;
    static juce::String paramNoiseRatio;
    static juce::String paramWaveOL;
    static juce::String paramWaveOR;
    static juce::String paramWaveWfm;
    static juce::String paramOutput;
    static juce::String paramVoices;

    gin::AudioFifo fifo {1, 44100};

    juce::MidiKeyboardState state;
private:
    void runUntil (int& done, juce::AudioSampleBuffer& buffer, int pos);
    PAPUEngine* findFreeVoice();
    PAPUEngine* findVoiceForNote (int note);

    juce::OwnedArray<PAPUEngine> papus;
    int nextVoice = 0;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PAPUAudioProcessor)
};
