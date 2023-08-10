/*
  ==============================================================================

    LibraryWindow.cpp
    Created: 8 Aug 2023 11:21:34am
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#include "LibraryWindow.h"

LibraryWindow::LibraryWindow(TrackLibrary *_tracks) : AlertWindow("test", "gestion de la librairie", MessageBoxIconType::NoIcon)
{
    tracks = _tracks;
    
    table.setModel(this);
    table.setColour (ListBox::outlineColourId, Colours::grey);
    table.setOutlineThickness (1);
    table.setMultipleSelectionEnabled (false);
    table.setBounds(0, 0, 150, 150);
    
    table.getHeader().addColumn("Id", 1, 50, 30);
    table.getHeader().addColumn("Title", 2, 50, 30);
    table.getHeader().addColumn("Author", 3, 50, 30);

    /*
    options.content.setOwned (&table);
    options.content->setSize(400, 600);

    options.dialogTitle                   = "Dialog Window";
    options.dialogBackgroundColour        = Colour (0xff0e345a);
    options.escapeKeyTriggersCloseButton  = true;
    options.useNativeTitleBar             = true;
    options.resizable                     = true;
    */
    
    //options.content.setOwned(&selectButton);
    
    addCustomComponent(&table);
    addButton("Select", 1);
}

LibraryWindow::~LibraryWindow()
{
    ;
}

void LibraryWindow::Show()
{
    //centreWithSize (600, 600);
    //options.launchAsync();
    table.repaint();
    enterModalState(true,nullptr,true);
}

/*
bool LibraryWindow::escapeKeyPressed()
{
    //tracks = NULL;
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
 
    return true;
}*/

int LibraryWindow::getNumRows()
{
    return tracks->getSize();
}

void LibraryWindow::paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    juce::Logger::writeToLog("paintRowbackground");
    auto alternateColour = getLookAndFeel().findColour (ListBox::backgroundColourId)
                                           .interpolatedWith (getLookAndFeel().findColour (ListBox::textColourId), 0.03f);
    if (rowIsSelected)
        g.fillAll (Colours::lightblue);
    else if (rowNumber % 2)
        g.fillAll (alternateColour);
}

void LibraryWindow::paintCell (Graphics& g, int rowNumber, int columnId,
                int width, int height, bool /*rowIsSelected*/)
{
    juce::Logger::writeToLog("paintCell");
    g.setColour (getLookAndFeel().findColour (ListBox::textColourId));
    //g.setFont (font);

    if (rowNumber <= tracks->getSize())
    {
        String text = getText(columnId, rowNumber);

        g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    /*
    if (auto* rowElement = dataList->getChildElement (rowNumber))
    {
        auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));

        g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
    }*/

    g.setColour (getLookAndFeel().findColour (ListBox::backgroundColourId));
    g.fillRect (width - 1, 0, 1, height);
}

void LibraryWindow::sortOrderChanged (int newSortColumnId, bool isForwards)
{
    /*
    if (newSortColumnId != 0)
    {
        DemoDataSorter sorter (getAttributeNameForColumnId (newSortColumnId), isForwards);
        dataList->sortChildElements (sorter);

        table.updateContent();
    }*/
}

Component* LibraryWindow::refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                    Component* existingComponentToUpdate) 
{
    if (columnId == 1 || columnId == 7) // The ID and Length columns do not have a custom component
    {
        jassert (existingComponentToUpdate == nullptr);
        return nullptr;
    }
    

    // The other columns are editable text columns, for which we use the custom Label component
    auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);

    // same as above...
    if (textLabel == nullptr)
        textLabel = new EditableTextCustomComponent (*this);

    textLabel->setRowAndColumn (rowNumber, columnId);
    return textLabel;
}

// This is overloaded from TableListBoxModel, and should choose the best width for the specified
// column.
int LibraryWindow::getColumnAutoSizeWidth (int columnId)
{
    if (columnId == 5)
        return 100; // (this is the ratings column, containing a custom combobox component)

    int widest = 32;

    // find the widest bit of text in this column..
    for (int i = getNumRows(); --i >= 0;)
    {
        /*
        if (auto* rowElement = dataList->getChildElement (i))
        {
            auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));

            widest = jmax (widest, font.getStringWidth (text));
        }*/
    }

    return widest + 8;
}

void LibraryWindow::selectButtonClicked()
{
    ;
}
