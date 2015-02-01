/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Fl2flangerAudioProcessor::Fl2flangerAudioProcessor()
{
    kDelay = 0.5;
    kDepth = 0.5;
    kRate = 0.5;
    kRegen = 0.5;
    kMix = 0.5;
}

Fl2flangerAudioProcessor::~Fl2flangerAudioProcessor()
{
}

//==============================================================================
const String Fl2flangerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int Fl2flangerAudioProcessor::getNumParameters()
{
    return totalNumParams;
}

float Fl2flangerAudioProcessor::getParameter (int index)
{
    switch (index) {
        case delayParam:
            return kDelay;
        case depthParam:
            return kDepth;
        case rateParam:
            return kRate;
        case regenParam:
            return kRegen;
        case mixParam:
            return kMix;
        default:
            return 0.0f;
    }
}

void Fl2flangerAudioProcessor::setParameter (int index, float newValue)
{
    switch (index) {
        case delayParam:
            kDelay = newValue;
            delaySmooth.setSteep(newValue);
            sendChangeMessage();
            break;
        case depthParam:
            kDepth = newValue;
            depthSmooth.setSteep(newValue);
            sendChangeMessage();
            break;
        case rateParam:
            kRate = newValue;
            rateSmooth.setSteep(newValue);
            sendChangeMessage();
            break;
        case regenParam:
            kRegen = newValue;
            regenSmooth.setSteep(newValue);
            sendChangeMessage();
            break;
        case mixParam:
            kMix = newValue;
            mixSmooth.setSteep(newValue);
            sendChangeMessage();
            break;
        default:
            break;
    }
}

float Fl2flangerAudioProcessor::getParameterDefaultValue(int index)
{
    switch (index) {
        case delayParam:
            return 0.0f;
        case depthParam:
            return 0.1f;
        case rateParam:
            return 0.5f;
        case regenParam:
            return 0.5f;
        case mixParam:
            return 0.5f;
        default:
            return 0.0f;
    }
}

const String Fl2flangerAudioProcessor::getParameterName (int index)
{
    switch (index) {
        case delayParam:
            return "Delay";
        case depthParam:
            return "Depth";
        case rateParam:
            return "Rate";
        case regenParam:
            return "Regen";
        case mixParam:
            return "Mix";
        default:
            return String::empty;
    }
}

const String Fl2flangerAudioProcessor::getParameterText (int index)
{
    return String::empty;
}

const String Fl2flangerAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String Fl2flangerAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool Fl2flangerAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool Fl2flangerAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool Fl2flangerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Fl2flangerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Fl2flangerAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double Fl2flangerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Fl2flangerAudioProcessor::getNumPrograms()
{
    return 0;
}

int Fl2flangerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Fl2flangerAudioProcessor::setCurrentProgram (int index)
{
}

const String Fl2flangerAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void Fl2flangerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Fl2flangerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    leftBuffer.setBufferSize(4410);
    rightBuffer.setBufferSize(4410);
    
    delaySmooth.setSampleRate(sampleRate);
    depthSmooth.setSampleRate(sampleRate);
    rateSmooth.setSampleRate(sampleRate);
    regenSmooth.setSampleRate(sampleRate);
    mixSmooth.setSampleRate(sampleRate);
    
    leftWave.setSampleRate(sampleRate);
    rightWave.setSampleRate(sampleRate);
    setParameterNotifyingHost(delayParam, kDelay);
    setParameterNotifyingHost(depthParam, kDepth);
    setParameterNotifyingHost(rateParam, kRate);
    setParameterNotifyingHost(regenParam, kRegen);
    setParameterNotifyingHost(mixParam, kMix);
    
}

void Fl2flangerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Fl2flangerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const float sampleRate = getSampleRate();
    
    if (getNumInputChannels() == 1) buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
    
    for (int i = 0; i < buffer.getNumSamples(); i++) {
       
        float rateParam = rateSmooth.smoothStep();
        if (rateParam == 0) {
            leftWave.reset();
            rightWave.reset();
        }
        
        leftWave.setFrequency(rateParam * 1.0);
        rightWave.setFrequency(rateParam * 1.1);
        
        
        float depthParam = depthSmooth.smoothStep();
        float leftMod = (leftWave.tick() + 1) * depthParam * (sampleRate/100);
        float rightMod = (rightWave.tick() + 1) * depthParam  * (sampleRate/100);
        
        float delayParam = delaySmooth.smoothStep();
        float leftSamplesDelay = ((delayParam * 15) + 0.75) * (sampleRate/1000) + leftMod;
        float rightSamplesDelay = ((delayParam * 20) + 0.5) * (sampleRate/1000) + rightMod;
    
        float l_xn = buffer.getWritePointer(0)[i];
        float r_xn = buffer.getWritePointer(1)[i];
        
        float l_combined;
        float r_combined;
        
        float l_yn = leftBuffer[leftSamplesDelay];
        float r_yn = rightBuffer[rightSamplesDelay];
        
        float regenParam = regenSmooth.smoothStep();
        l_combined = l_xn + r_yn * regenParam;
        r_combined = r_xn + l_yn * regenParam;
        
        leftBuffer.addSample(l_combined);
        rightBuffer.addSample(r_combined);
        
        float mixParam = mixSmooth.smoothStep();
        buffer.getWritePointer(0)[i] = (l_xn * (1-mixParam) + l_yn * mixParam);
        buffer.getWritePointer(1)[i] = (r_xn * (1-mixParam) + r_yn * mixParam);
        
    
    }
}

//==============================================================================
bool Fl2flangerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Fl2flangerAudioProcessor::createEditor()
{
    return new Fl2flangerAudioProcessorEditor (this);
}

//==============================================================================
void Fl2flangerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Create an outer XML element..
    XmlElement xml ("MYPLUGINSETTINGS");
    
    // add some attributes to it..
    xml.setAttribute ("delay", kDelay);
    xml.setAttribute ("depth", kDepth);
    xml.setAttribute("rate", kRate);
    xml.setAttribute("regen", kRegen);
    xml.setAttribute("mix", kMix);
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
}

void Fl2flangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr) {
        if (xmlState->hasTagName ("MYPLUGINSETTINGS")) {
            kDelay = (float) xmlState->getDoubleAttribute("delay");
            kDepth = (float) xmlState->getDoubleAttribute("depth");
            kRate = (float) xmlState->getDoubleAttribute("rate");
            kRegen = (float) xmlState->getDoubleAttribute("regen");
            kMix = (float) xmlState->getDoubleAttribute("mix");
        }
    }
    sendChangeMessage();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Fl2flangerAudioProcessor();
}
