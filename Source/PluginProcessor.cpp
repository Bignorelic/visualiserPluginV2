/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VisualiserPluginV2AudioProcessor::VisualiserPluginV2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    longWaveViewer(2),
    shortWaveViewer(2)
#endif
{
    longWaveViewer.setRepaintRate(60);
    longWaveViewer.setBufferSize(8192);
    longWaveViewer.setSamplesPerBlock(64);

    shortWaveViewer.setRepaintRate(60);
    shortWaveViewer.setBufferSize(1024);
    shortWaveViewer.setSamplesPerBlock(16);
}

VisualiserPluginV2AudioProcessor::~VisualiserPluginV2AudioProcessor()
{
}

//==============================================================================
const juce::String VisualiserPluginV2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VisualiserPluginV2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VisualiserPluginV2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VisualiserPluginV2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VisualiserPluginV2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VisualiserPluginV2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VisualiserPluginV2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void VisualiserPluginV2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VisualiserPluginV2AudioProcessor::getProgramName (int index)
{
    return {};
}

void VisualiserPluginV2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VisualiserPluginV2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    juce::dsp::ProcessSpec spec;

    //osc.initialise([](float x) { return std::sin(x); });

    //spec.numChannels = getTotalNumOutputChannels();
    //osc.prepare(spec);
    //osc.setFrequency(100);
}

void VisualiserPluginV2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VisualiserPluginV2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void VisualiserPluginV2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    //buffer.clear();

    //juce::dsp::ProcessContextReplacing<float> stereoContext(block);
    //osc.process(stereoContext);

    juce::AudioBuffer<float> reducedBuffer(buffer.getNumChannels(), buffer.getNumSamples());


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        auto* reducedChannelData = reducedBuffer.getWritePointer(channel);

        // ..do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            //float signalToReduce = *channelData;

            *reducedChannelData = *channelData * 0.5;

            channelData++;
            reducedChannelData++;
        }
    }

    longWaveViewer.pushBuffer(buffer);
    shortWaveViewer.pushBuffer(reducedBuffer);
}

//==============================================================================
bool VisualiserPluginV2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VisualiserPluginV2AudioProcessor::createEditor()
{
    return new VisualiserPluginV2AudioProcessorEditor (*this);
}

//==============================================================================
void VisualiserPluginV2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VisualiserPluginV2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VisualiserPluginV2AudioProcessor();
}
