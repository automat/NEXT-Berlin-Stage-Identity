//
//  ControlPanel.h
//  GwenSamples
//
//  Created by Henryk Wollik on 23/03/14.
//
//

#ifndef GwenSamples_ControlPanel_h
#define GwenSamples_ControlPanel_h

#include "cinder/Rect.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"

#include "cigwen/CinderGwen.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/WindowButtons.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/CheckBox.h"
#include "Gwen/Controls/RadioButtonController.h"
#include "Gwen/Controls/ScrollControl.h"
#include "Gwen/Controls/ScrollBar.h"
#include "Gwen/Controls/TabControl.h"
#include "Gwen/Controls/GroupBox.h"
#include "Gwen/Controls/CollapsibleList.h"
#include "Gwen/Controls/ComboBox.h"
#include "Gwen/Controls/CollapsibleCategory.h"
#include "Gwen/Controls/MenuStrip.h"
#include "Gwen/Controls/NumericUpDown.h"
#include "Gwen/Controls/Layout/Table.h"
#include "Gwen/Controls/ListBox.h"
#include "Gwen/Controls/TextBox.h"

using namespace cinder;
using namespace Gwen::Controls;




class ControlPanel : public Gwen::Controls::WindowControl {
    float mPadding[4];
    //TabControl*             mTabControl;
    
    CollapsibleList*        mColList0;
    CollapsibleCategory*    mColCategory0;
    CollapsibleCategory*    mColCategory1;
    
    WindowMinimizeButton*   mMinimizeButton;
    WindowMaximizeButton*   mMaximizeButtom;
    
    WindowControl*          mWindowControl0;
    
    ScrollControl*          mScrollControl;
    GroupBox*               mGroupBox;
    CheckBoxWithLabel*      mCheckBox;
    RadioButtonController*  mRadioButtonCntrl;
    
    
    
private:
    /*
    void OnResized(){
        Gwen::Point size(GetSize());
        mScrollControl->SetPos(size.x - mScrollControl->GetSize().x, 0);
    }*/
    
    void init(){
        mMinimizeButton = new WindowMinimizeButton(this);
        mMaximizeButtom = new WindowMaximizeButton(this);
       // mTabControl = new TabControl(this);
    
        
        Gwen::Padding padding(0,0,0,0);
        Gwen::Margin  margin(0,0,0,0);
        SetPadding(padding);
        SetMargin(margin);
        
        Gwen::Point size = GetSize();
        float width = size.x;
        
        float paddingT = mPadding[0];
        float paddingR = mPadding[1];
        float paddingB = mPadding[2];
        float paddingL = mPadding[3];
        
        float _width = width - paddingL - paddingR;
        
        
       // mScrollControl = new ScrollControl(this);
       // mScrollControl->SetPos(size.x - mScrollControl->GetSize().x,100);
        //mScrollControl->SetAutoHideBars(true);
        /*
        mCheckBox = new CheckBoxWithLabel(mScrollControl);
        mCheckBox->Label()->SetText("Label");
        
        mRadioButtonCntrl = new RadioButtonController(mScrollControl);
        mRadioButtonCntrl->SetPos(0, mCheckBox->Bottom()+10);
        mRadioButtonCntrl->SetSize(_width, 100);
        mRadioButtonCntrl->AddOption("Option A","abc");
        mRadioButtonCntrl->AddOption("Option A","abc");
        mRadioButtonCntrl->AddOption("Option A","abc");
         */
        
        
        
        Gwen::Margin baseMargin(0,10,0,0);
        
        {
            Gwen::Controls::TextBox* textbox = new Gwen::Controls::TextBox(this);
            textbox->Dock(Gwen::Pos::Top);
            textbox->SetHeight(30);
            
        }
        
        {
            ComboBox* comboBox = new ComboBox(this);
            comboBox->Dock(Gwen::Pos::Top);
            comboBox->SetHeight(30);
            comboBox->SetMargin(baseMargin);
            comboBox->SetPadding(Gwen::Padding(10,0,0,0));
            comboBox->AddItem( L"Option One", "one" );
            comboBox->AddItem( L"Number Two", "two" );
            comboBox->AddItem( L"Door Three", "three" );
            comboBox->AddItem( L"Four Legs", "four" );
            comboBox->AddItem( L"Five Birds", "five" );
        }
        
        {
            GroupBox* groupBox = new GroupBox(this);
            groupBox->SetMargin(baseMargin);
            groupBox->Dock(Gwen::Pos::Top);
            groupBox->SetText("Group Box");
            groupBox->SetHeight(60);
            groupBox->SetPadding(Gwen::Padding(10,10,10,10));
            Label* label = new Label(groupBox);
            label->SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam et dolor sit");
            label->SetWrap(true);
            label->Dock(Gwen::Pos::Fill);
        }
        
        {
            ListBox* listbox = new ListBox(this);
            listbox->SetMargin(baseMargin);
            listbox->Dock(Gwen::Pos::Top);
            listbox->SetHeight(100);
            
            listbox->SetColumnCount( 3 );
            listbox->SetAllowMultiSelect( true );
            {
                Layout::TableRow* pRow = listbox->AddItem( L"Baked Beans" );
                pRow->SetCellText( 1, L"Heinz" );
                pRow->SetCellText( 2, "£3.50" );
            }
            {
                Layout::TableRow* pRow = listbox->AddItem( L"Bananas" );
                pRow->SetCellText( 1, L"Trees" );
                pRow->SetCellText( 2, L"$1.27" );
            }
            {
                Layout::TableRow* pRow = listbox->AddItem( L"Chicken" );
                pRow->SetCellText( 1, L"\u5355\u5143\u6D4B\u8BD5" );
                pRow->SetCellText( 2, L"\u20AC8.95" );
            }
            {
                Layout::TableRow* pRow = listbox->AddItem( L"Chicken" );
                pRow->SetCellText( 1, L"\u5355\u5143\u6D4B\u8BD5" );
                pRow->SetCellText( 2, L"\u20AC8.95" );
            }
            {
                Layout::TableRow* pRow = listbox->AddItem( L"Chicken" );
                pRow->SetCellText( 1, L"\u5355\u5143\u6D4B\u8BD5" );
                pRow->SetCellText( 2, L"\u20AC8.95" );
            }
            {
                Layout::TableRow* pRow = listbox->AddItem( L"Chicken" );
                pRow->SetCellText( 1, L"\u5355\u5143\u6D4B\u8BD5" );
                pRow->SetCellText( 2, L"\u20AC8.95" );
            }
            {
                Layout::TableRow* pRow = listbox->AddItem( L"Chicken" );
                pRow->SetCellText( 1, L"\u5355\u5143\u6D4B\u8BD5" );
                pRow->SetCellText( 2, L"\u20AC8.95" );
            }
            
            
            
        }
        
        {
            GroupBox* groupBox = new GroupBox(this);
            groupBox->SetMargin(baseMargin);
            groupBox->Dock(Gwen::Pos::Top);
            groupBox->SetText("Group Box");
            groupBox->SetHeight(50);
            
            ScrollControl* scrollCntrl = new ScrollControl(groupBox);
            scrollCntrl->SetMargin(baseMargin);
            scrollCntrl->SetHeight(200);
            scrollCntrl->SetPadding(Gwen::Padding(10,10,10,10));
            scrollCntrl->Dock(Gwen::Pos::Top);
            
            Label* label = new Label(groupBox);
            label->Dock(Gwen::Pos::Left);
            label->SetText("Value 0");
            label->SetWidth(100);
            
            NumericUpDown* stepper = new NumericUpDown(scrollCntrl);
            stepper->Dock(Gwen::Pos::Left);
            stepper->Dock(Gwen::Pos::CenterH);
            
        }
        {
            CollapsibleList* list = new CollapsibleList(this);
            list->SetName("dfsdf");
            list->Dock(Gwen::Pos::Top);
            list->SetHeight(100);
            
            CollapsibleCategory* category = list->Add("Basic");
            category->Add("Hello");
            category->Add("Hello");
            category->Add("Hello");
            category->Add("Hello");
            category->Add("Hello");
            
            category = list->Add("Basic");
            category->Add("Hello");
            category->Add("Hello");
            category->Add("Hello");
            category->Add("Hello");
            category->Add("Hello");
        }
        
        
        
        
        
        /*
        
        mColList0     = new CollapsibleList(this);
        mColList0->Dock(Gwen::Pos::Top);
        mColList0->SetBounds(this->GetBounds());
        
        CollapsibleCategory* category = mColList0->Add("Basic");
        category->Add("Hello");
        category->Add("Hello");
        category->Add("Hello");
        category->Add("Hello");
        category->Add("Hello");
         */
        
        
    
 
        
        
        
        
      //  mWindowControl0 = new WindowControl(this);
      //  mWindowControl0->SetSize(200,200);
      //  mTabControl->AddPage(Gwen::TextObject("Tab 0"), mWindowControl0);
       
       // mWindowControl0 = new WindowControl(this);
        //mWindowControl0->SetSize(200, 200);
        
        
        //mTabControl = new TabControl(this);
        //mTabControl->AddPage("tab");
        
        /*
        
        
        
        mCheckBox = new CheckBoxWithLabel(mScrollControl);
        mCheckBox->SetPos(origin.x,origin.y);
        mCheckBox->Label()->SetText("Label");
        
        
              */
        
       
      
    }
    
    
    
public:
    ControlPanel( Gwen::Controls::Base* parent, const Vec2f& pos = Vec2f()) :
    Gwen::Controls::WindowControl(parent){
        SetTitle("ControlPanel");
    
        SetSize(300, 400);
        SetPos(pos.x, pos.y);
        SetClosable(true);
        SetDeleteOnClose(false);
        SetPadding(Gwen::Padding(0,0,0,0));
        SetMargin(Gwen::Margin(0,0,0,0));
        
        mPadding[0] = 10;
        mPadding[1] = 10;
        mPadding[2] = 10;
        mPadding[3] = 10;
        
        init();
    }
};

#endif
