/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // EXPOSE VTS TO EDITOR
    juce::AudioProcessorValueTreeState& getVTS() {
        return *mValueTreeState;
    };

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:

    float mSineTonePhase = 0.f;

    std::atomic<float>* mDryWet = nullptr;
    std::atomic<float>* mFeedback = nullptr;
    std::atomic<float>* mColorR = nullptr;
    std::atomic<float>* mColorG = nullptr;
    std::atomic<float>* mColorB = nullptr;

    juce::LinearSmoothedValue<float> mSmoothedDryWet;
    juce::LinearSmoothedValue<float> mSmoothedFeedback;
    juce::LinearSmoothedValue<float> mSmoothedColorR;
    juce::LinearSmoothedValue<float> mSmoothedColorG;
    juce::LinearSmoothedValue<float> mSmoothedColorB;

    std::unique_ptr<juce::AudioProcessorValueTreeState> mValueTreeState;

    // value tree construction function, this needs to be called in the constructor of the plugin.
    void _constructValueTreeState();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
