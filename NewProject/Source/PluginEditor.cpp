/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    //
    mDryWetSlider.setRange(0, 1.f);
    mDryWetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    mDryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    addAndMakeVisible(mDryWetSlider);

    mDryWetSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),
        "dry_wet",
        mDryWetSlider));

    //
    mFeedbackSlider.setRange(0, 1.f);
    mFeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    mFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    addAndMakeVisible(mFeedbackSlider);

    mFeedbackSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),
        "feedback",
        mFeedbackSlider));

    //
    mColorRSlider.setRange(0, 1.f);
    //mColorRSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    mColorRSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    addAndMakeVisible(mColorRSlider);

    mColorRSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),
        "color_r",
        mColorRSlider));

    //
    mColorGSlider.setRange(0, 1.f);
    //mColorGSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    mColorGSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    addAndMakeVisible(mColorGSlider);

    mColorGSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),
        "color_g",
        mColorGSlider));

    //
    mColorBSlider.setRange(0, 1.f);
    //mColorBSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20);
    mColorBSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    addAndMakeVisible(mColorBSlider);

    mColorBSliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getVTS(),
        "color_b",
        mColorBSlider));
    
    setResizable(true, true);
    setResizeLimits(20, 20, 1080, 1080);
    setSize(500, 270);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    g.setColour (juce::Colours::black);

    //set slider thumb colors based on values
    juce::AudioProcessorValueTreeState& treestate = audioProcessor.getVTS();
    mColorRSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA(*treestate.getRawParameterValue("color_r"), 0., 0., 1.));
    mColorGSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA(0., *treestate.getRawParameterValue("color_g"), 0., 1.));
    mColorBSlider.setColour(juce::Slider::thumbColourId, juce::Colour::fromFloatRGBA(0., 0., *treestate.getRawParameterValue("color_b"), 1.));
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    mColorRSlider.setBounds((getWidth() - 400) / 2, 0, 400, 100);
    mColorGSlider.setBounds((getWidth() - 400) / 2, 50, 400, 100);
    mColorBSlider.setBounds((getWidth() - 400) / 2, 100, 400, 100);

    mFeedbackSlider.setBounds((getWidth() - 400) / 2, 170, 100, 100);
    mDryWetSlider.setBounds((getWidth() - 400) / 2 + 300, 170, 100, 100);
}
