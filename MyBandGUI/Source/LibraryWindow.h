/*
  ==============================================================================

    LibraryWindow.h
    Created: 8 Aug 2023 11:21:34am
    Author:  Frédéric FELTEN

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "../../Common/TrackLibrary.h"

class LibraryWindow : public AlertWindow, public TableListBoxModel
{
public:
    LibraryWindow(TrackLibrary *tracks);
    ~LibraryWindow();
    
    void Show();
    //bool escapeKeyPressed() override;
    int getNumRows() override;
    void paintRowBackground (Graphics& g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected) override;
    void paintCell (Graphics& g, int rowNumber, int columnId, int width, int height, bool /*rowIsSelected*/) override;
    void sortOrderChanged (int newSortColumnId, bool isForwards) override;
    Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
                                        Component* existingComponentToUpdate) override;
    int getColumnAutoSizeWidth (int columnId) override;
    void selectButtonClicked();
    
    String getText (const int columnNumber, const int rowNumber) const
    {
        juce::Logger::writeToLog("getText");
        String text = "";
        switch (columnNumber)
        {
            case 1:
                text += rowNumber;
                break;
            case 2:
                text = tracks->getTrack(rowNumber)->getTitle();
                break;
            case 3:
                text = tracks->getTrack(rowNumber)->getAuthor();
                break;
        }
        
        return text;
    }

    void setText (const int columnNumber, const int rowNumber, const String& newText)
    {
        auto columnName = table.getHeader().getColumnName (columnNumber);
        //dataList->getChildElement (rowNumber)->setAttribute (columnName, newText);
    }
    
private:
    TableListBox table;
    juce::TextButton selectButton;
    TrackLibrary* tracks;
    //DialogWindow::LaunchOptions options;
    int numRows;
    
    //==============================================================================
    // This is a custom Label component, which we use for the table's editable text columns.
    class EditableTextCustomComponent  : public Label
    {
    public:
        EditableTextCustomComponent (LibraryWindow& td)  : owner (td)
        {
            // double click to edit the label text; single click handled below
            setEditable (false, true, false);
        }

        void mouseDown (const MouseEvent& event) override
        {
            // single click on the label should simply select the row
            owner.table.selectRowsBasedOnModifierKeys (row, event.mods, false);

            Label::mouseDown (event);
        }

        void textWasEdited() override
        {
            owner.setText (columnId, row, getText());
        }

        // Our demo code will call this when we may need to update our contents
        void setRowAndColumn (const int newRow, const int newColumn)
        {
            row = newRow;
            columnId = newColumn;
            setText (owner.getText(columnId, row), dontSendNotification);
        }

        void paint (Graphics& g) override
        {
            auto& lf = getLookAndFeel();
            if (! dynamic_cast<LookAndFeel_V4*> (&lf))
                lf.setColour (textColourId, Colours::black);

            Label::paint (g);
        }

    private:
        LibraryWindow& owner;
        int row, columnId;
        Colour textColour;
    };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LibraryWindow)
};
