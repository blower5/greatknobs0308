/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Slider mDryWetSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDryWetSliderAttachment;

    juce::Slider mFeedbackSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFeedbackSliderAttachment;

    juce::Slider mColorRSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mColorRSliderAttachment;

    juce::Slider mColorGSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mColorGSliderAttachment;

    juce::Slider mColorBSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mColorBSliderAttachment;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
