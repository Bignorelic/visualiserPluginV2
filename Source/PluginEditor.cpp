/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VisualiserPluginV2AudioProcessorEditor::VisualiserPluginV2AudioProcessorEditor (VisualiserPluginV2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(audioProcessor.longWaveViewer);
    addAndMakeVisible(audioProcessor.shortWaveViewer);
    audioProcessor.longWaveViewer.setColours(juce::Colours::black, juce::Colours::white);
    audioProcessor.shortWaveViewer.setColours(juce::Colours::black, juce::Colours::white);

    setSize (1280, 400);
}

VisualiserPluginV2AudioProcessorEditor::~VisualiserPluginV2AudioProcessorEditor()
{
}

//==============================================================================
void VisualiserPluginV2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    using namespace juce;

    g.fillAll(Colours::black);

    auto bounds = getLocalBounds();
    auto longWaveformArea = bounds.removeFromBottom(100);
    auto shortWaveformArea = bounds.removeFromRight(500);
    auto spectrumArea = bounds;

    //g.setColour(Colours::red);
    //g.drawRect(longWaveformArea, 1.f);
    //g.drawRect(shortWaveformArea, 1.f);
    //g.drawRect(spectrumArea, 1.f);

    // Path p;
    //audioProcessor.longWaveViewer.getChannelAsPath(p, ,16, )
}

void VisualiserPluginV2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds();
    auto longWaveformArea = bounds.removeFromBottom(100);
    auto shortWaveformArea = bounds.removeFromRight(500);
    auto spectrumAre = bounds;

    audioProcessor.longWaveViewer.setBounds(longWaveformArea);
    audioProcessor.shortWaveViewer.setBounds(shortWaveformArea);
}
