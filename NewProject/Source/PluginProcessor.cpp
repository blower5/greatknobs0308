/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <numbers>
#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       )
{
    _constructValueTreeState();
    
    mDryWet = mValueTreeState->getRawParameterValue("dry_wet");
    mFeedback = mValueTreeState->getRawParameterValue("feedback");
    mColorR = mValueTreeState->getRawParameterValue("color_r");
    mColorG = mValueTreeState->getRawParameterValue("color_g");
    mColorB = mValueTreeState->getRawParameterValue("color_b");

    mSmoothedDryWet.setCurrentAndTargetValue(0.5f);
    mSmoothedFeedback.setCurrentAndTargetValue(0.5f);
    mSmoothedColorR.setCurrentAndTargetValue(0.5f);
    mSmoothedColorG.setCurrentAndTargetValue(0.5f);
    mSmoothedColorB.setCurrentAndTargetValue(0.5f);
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    mSmoothedDryWet.reset(sampleRate, .01f);
    mSmoothedFeedback.reset(sampleRate, .01f);
    mSmoothedColorR.reset(sampleRate, .01f);
    mSmoothedColorG.reset(sampleRate, .01f);
    mSmoothedColorB.reset(sampleRate, .01f);
}

void NewProjectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // update the smoothed values
    mSmoothedDryWet.setTargetValue(*mDryWet);
    mSmoothedFeedback.setTargetValue(*mFeedback);
    mSmoothedColorR.setTargetValue(*mColorR);
    mSmoothedColorG.setTargetValue(*mColorG);
    mSmoothedColorB.setTargetValue(*mColorB);

    // BUFFER POINTERS
    float* left_channel = buffer.getWritePointer(0);
    float* right_channel = buffer.getWritePointer(1);

    // --------------------------
    // Sine Tone Generation Start
    // --------------------------
    float sine_hz = 80.f;
    float phase_delta = sine_hz / getSampleRate();

    for (int sample_idx = 0; sample_idx < buffer.getNumSamples(); sample_idx++) {

        float sine_out = std::sin(mSineTonePhase * 2 * std::numbers::pi);

        mSineTonePhase += phase_delta;

        if (mSineTonePhase > 1.f) {
            mSineTonePhase -= 1.f;
        }

        left_channel[sample_idx] = sine_out;
        right_channel[sample_idx] = sine_out;
    }
    // ------------------------
    // Sine Tone Generation End
    // ------------------------

    // --------------------
    // Gain Computation End
    // --------------------
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = mValueTreeState->copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    mValueTreeState->replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
void NewProjectAudioProcessor::_constructValueTreeState()
{
    mValueTreeState.reset(new juce::AudioProcessorValueTreeState(*this, nullptr, juce::Identifier("My_First_Plugin"),
        {
            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("dry_wet", 1), // parameterID
                                                         "Dry/Wet", // parameter name
                                                         0.0f,   // minimum value
                                                         1.0f,   // maximum value
                                                         0.5f),   // default value

            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("feedback", 1), // parameterID
                                                         "Feedback", // parameter name
                                                         0.0f,   // minimum value
                                                         1.0f,   // maximum value
                                                         0.5f),   // default value

            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("color_r", 1), // parameterID
                                                         "Color (R)", // parameter name
                                                         0.0f,   // minimum value
                                                         1.0f,   // maximum value
                                                         0.5f),   // default value
            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("color_g", 1), // parameterID
                                                         "Color (G)", // parameter name
                                                         0.0f,   // minimum value
                                                         1.0f,   // maximum value
                                                         0.5f),   // default value
            std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("color_b", 1), // parameterID
                                                         "Color (B)", // parameter name
                                                         0.0f,   // minimum value
                                                         1.0f,   // maximum value
                                                         0.5f)   // default value

        }));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
