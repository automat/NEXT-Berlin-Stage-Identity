//
//  Controls.h
//  NextQuoteProcessor
//
//  Created by Henryk Wollik on 25/03/14.
//
//

#ifndef NextQuoteProcessor_Controls_h
#define NextQuoteProcessor_Controls_h

#include <iostream>
#include <fstream>

#include "Resources.h"
#include "Config.h"
#include "Properties.h"

#include "cinder/app/App.h"

#include "FileWatcher.h"

#include "cigwen/CinderGwen.h"
#include "cigwen/GwenRendererGl.h"
#include "cigwen/GwenInput.h"
#include "Gwen/Skins/Dark.h"
#include "Gwen/Events.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/CrossSplitter.h"
#include "Gwen/Controls.h"
#include "Gwen/Controls/NumericUpDown.h"

#include "Grid.h"
#include "QuoteTypesetter.h"

#include "WindowPreviewIso.h"
#include "WindowPreviewTex.h"

#include "cinder/Json.h"

#include <cstdlib>
#include <vector>

using namespace ci;
using namespace std;

typedef std::shared_ptr<class Controller> ControllerRef;


/*--------------------------------------------------------------------------------------------*/

class Controller : public Gwen::Event::Handler {
    cigwen::GwenRendererGl*        mRenderer;
    cigwen::GwenInputRef           mInput;
    Gwen::Controls::Canvas*        mCanvas;
    
    Gwen::Controls::WindowControl* mWindowInspector;
    Gwen::Controls::WindowControl* mWindowSettings;
    Gwen::Controls::WindowControl* mWindowConsole;
    Gwen::Controls::WindowControl* mWindowTextInput;
    Gwen::Controls::WindowControl* mWindowFilePreview;
    

    
    Grid*             mGrid;
    QuoteTypesetter*  mQuoteTypesetter;
    WindowPreviewIso* mWindowPreviewIso;
    WindowPreviewTex* mWindowPreviewTex;
    
    FileWatcher*      mFileWatcher;
    std::string       mFilePath;
    JsonTree          mJson;
    JsonTree*         mJsonSelected;
    bool              mFileOpenWithDefaultApp;
    
    /*--------------------------------------------------------------------------------------------*/
    // Quote Format
    /*--------------------------------------------------------------------------------------------*/
    
    struct QuoteFormat{
        float  scale;
        int    padding[4];
        int    align;
        bool   balance;
        QuoteFormat() : scale(1.0f), align(0), balance(false){
            padding[0] = padding[1] = padding[2] = padding[3] = 0;
            
        }
    };
    
    
    struct Quote_Internal{
        std::string str;
        QuoteFormat format;
    };
    
    
    /*--------------------------------------------------------------------------------------------*/
    // Init Gwen
    /*--------------------------------------------------------------------------------------------*/
    
    inline void initInterface(){
        using namespace Gwen::Controls;
        
        //
        //  Menu
        //
        
        MenuStrip* menu = new MenuStrip(mCanvas);
        menu->Dock(Gwen::Pos::Top);
        menu->AddItem("Quit");
        MenuItem* menuFile = menu->AddItem("File");
        menuFile->GetMenu()->AddItem("New")->SetAction(this, &Controller::onMenuItemSelected);
        menuFile->GetMenu()->AddItem("Load")->SetAction(this, &Controller::onMenuItemSelected);
        menuFile->GetMenu()->AddItem("Close")->SetAction(this, &Controller::onMenuItemSelected);
        
        
        
        //mWindowConsole = new WindowControl(mCanvas);
        //mWindowConsole->Dock(Gwen::Pos::Fill);
        
        WindowControl* windowView = new WindowControl(mCanvas);
        //windowView->SetTitle("View");
        windowView->SetHeight(750);
        windowView->Dock(Gwen::Pos::Top);
        windowView->SetClosable(false);
        
    

        mWindowPreviewIso = new WindowPreviewIso(windowView);
        mWindowPreviewIso->SetTitle("Isometric View");
        mWindowPreviewIso->SetWidth(ci::app::getWindowWidth() * 0.5f);
        mWindowPreviewIso->SetClosable(false);
        mWindowPreviewIso->SetTabable(false);
        mWindowPreviewIso->Dock(Gwen::Pos::Left);
        
     
        
        mWindowPreviewTex = new WindowPreviewTex(windowView);
        mWindowPreviewTex->SetTitle("Texture View");
        mWindowPreviewTex->SetClosable(false);
        mWindowPreviewTex->SetTabable(false);
        mWindowPreviewTex->Dock(Gwen::Pos::Fill);
        
        
        mWindowInspector = new WindowControl(mCanvas);
        mWindowInspector->SetTitle("Inspector");
        mWindowInspector->SetClosable(false);
        mWindowInspector->SetDeleteOnClose(false);
        mWindowInspector->SetHeight(500);
        mWindowInspector->Dock(Gwen::Pos::Fill);
        
        {
            WindowControl* windowText = new WindowControl(mWindowInspector);
            windowText->SetTitle("Text Input");
            windowText->SetWidth(500);
            windowText->SetClosable(false);
            windowText->Dock(Gwen::Pos::Left);
            
           
        
            GroupBox* groupL = new GroupBox(windowText);
            groupL->SetText("String Input");
            groupL->Dock(Gwen::Pos::Fill);
            groupL->SetInnerMargin(10);
            
            TextBoxMultiline* textbox = new TextBoxMultiline(groupL);
            textbox->Dock(Gwen::Pos::Fill);
            textbox->SetPadding(Gwen::Padding(10,10,10,10));
           // textbox->OnPaste.Add(this,&Interface::OnStringInputPaste);
            
            
            
   
            /*
            Properties* propString = new Properties(propGroup);
            propString->Add("srting",new Property::Text(propString),L"string");
            propString->Dock(Gwen::Pos::Fill);
            */
            
            
            

           
            
            /*
            Gwen::Controls::TextBox* textbox = new Gwen::Controls::TextBox(windowText);
            textbox->Dock(Gwen::Pos::Left);
            textbox->SetWrap(true);
            */
            
            int innerMargin = 20;
            int boxWidth    = 280;
            
            GroupBox* groupR = new GroupBox(windowText);
            groupR->Dock(Gwen::Pos::Right);
            groupR->SetWidth(boxWidth);
            groupR->SetText("Font Properties");
            groupR->SetMargin(Gwen::Margin(6,0,0,0));
            groupR->SetInnerMargin(innerMargin);
  
            
            
            
            {
                
                int labelStepY = 28;
                
                int inputWidth = 150;
                int inputStepX = boxWidth - innerMargin * 2 - inputWidth;
                int inputStepY = 25;
                
                float inputLabelLineHeight = 1.85f;
                
                
                
                Label*  labelFontScale = new Label(groupR);
                labelFontScale->SetText("Scale");
                labelFontScale->SetPos(0, 4);
                
                
                
                
                NumericUpDown* fontScale = new NumericUpDown(groupR);
                fontScale->SetWidth(inputWidth);
                fontScale->SetPos(inputStepX, 0);
                
                Label* labelPadding = new Label(groupR);
                labelPadding->SetText("Padding");
                labelPadding->SetPos(0, labelStepY);
                
                int   inputPadding = 1;
                float inputWidth4 = inputWidth / 4.0f - inputPadding * 3;
                float inputStep4  = inputWidth / 4.0f + inputPadding;
                
                {
                    NumericUpDown* padding = new NumericUpDown(groupR);
                    padding->SetWidth(inputWidth4);
                    padding->SetPos(inputStepX + inputStep4 * 0, inputStepY);
                }
                {
                    NumericUpDown* padding = new NumericUpDown(groupR);
                    padding->SetWidth(inputWidth4);
                    padding->SetPos(inputStepX + inputStep4 * 1, inputStepY);
                }
                {
                    NumericUpDown* padding = new NumericUpDown(groupR);
                    padding->SetWidth(inputWidth4);
                    padding->SetPos(inputStepX + inputStep4 * 2, inputStepY);
                }
                {
                    NumericUpDown* padding = new NumericUpDown(groupR);
                    padding->SetWidth(inputWidth4);
                    padding->SetPos(inputStepX + inputStep4 * 3, inputStepY);
                }
                
                ComboBox* align = new ComboBox(groupR);
                align->SetWidth(inputWidth);
                align->SetPos(inputStepX,inputStepY * 2);
                align->AddItem(L"Left");
                align->AddItem(L"Center");
                align->AddItem(L"Right");
                
                Label* labelAlign = new Label(groupR);
                labelAlign->SetText("Align");
                labelAlign->SetPos(0, labelStepY * inputLabelLineHeight);
                
                CheckBox* balancedBaseline = new CheckBox(groupR);
                balancedBaseline->SetPos(inputStepX, inputStepY * 3);
                
                Label* labelBalancedBaseline = new Label(groupR);
                labelBalancedBaseline->SetText("Bal. Baseline");
                labelBalancedBaseline->SetPos(0, labelStepY * inputLabelLineHeight  + labelStepY / inputLabelLineHeight * 2);
            
                
            
            }
            
            
            
            /*
            NumericUpDown* fontScale = new NumericUpDown(groupR);
            */
            
            
            
            WindowControl* windowOutput = new WindowControl(mWindowInspector);
            windowOutput->SetTitle("Json Output");
            windowOutput->SetClosable(false);
            windowOutput->Dock(Gwen::Pos::Fill);
            
        }
        
        //
        //  Windows
        //
        
        
       
        
        
        
        
        
    
        
    
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Gwen callbacks
    /*--------------------------------------------------------------------------------------------*/
    
    inline void OnStringInputPaste( Gwen::Controls::Base* control){
        using namespace Gwen;
    }
    
    inline void onMenuItemSelected( Gwen::Controls::Base* control){
        using namespace Gwen;
        using namespace std;
        Controls::MenuItem* menuItem = (Controls::MenuItem* ) control;
        std::string menuItemIdentifier = menuItem->GetText().Get();
        
        
        //
        // Create new JSON template
        //
        if(menuItemIdentifier == "New"){
            mFileWatcher->clear();
            vector<string> exts = {"json"};
            
            fs::path filePath = app::getSaveFilePath(app::getAppPath().parent_path(),exts);
            
            
            if(!filePath.empty()){
                string filePathStr = filePath.string();
                JsonTree _templateObj(JsonTree::makeObject());
                JsonTree _templateArr(JsonTree::makeArray("quotes"));
                _templateArr.pushBack(writeQuoteJson("Sample String"));
                
                JsonTree _template(_templateArr);
                
                
                
                //JsonTree _template = writeQuoteJson("HAAAL");
                
                ofstream ostream(filePathStr);
                ostream << _template.serialize();
                ostream.close();
                
                // open file with default application
                if(mFileOpenWithDefaultApp){
                    string cmd = "open  \"" + filePathStr +  "\"";
                    std::system(cmd.c_str());
                }
                
                mFilePath = filePathStr;
                mFileWatcher->addFile(filePathStr);
            }
            
        }
        
        //
        // Load JSON
        //
        
        if(menuItemIdentifier == "Load"){
            mFileWatcher->clear();
            vector<string> exts = {"json"};
            
            fs::path filePath = app::getOpenFilePath(app::getAppPath().parent_path(),exts);
            
            if(!filePath.empty()){
                string filePathStr = filePath.string();
                
                // open file with default application
                if(mFileOpenWithDefaultApp){
                    string cmd = "open  \"" + filePathStr +  "\"";
                    std::system(cmd.c_str());
                }
                
                mFilePath = filePathStr;
                mFileWatcher->addFile(filePathStr);
            }
        }
        
        //
        // Unwatch JSON
        //
        
        if(menuItemIdentifier == "Close"){
            cout << "New" << endl;
        }
       
    }
    
    inline void displayQuote(){
        
    }
    
    /*--------------------------------------------------------------------------------------------*/
    // Handle Json
    /*--------------------------------------------------------------------------------------------*/

    inline JsonTree writeQuoteJson(const string& str, const QuoteFormat& format = QuoteFormat()){
        JsonTree _format = JsonTree::makeObject("format");
        _format.pushBack(JsonTree("scale",format.scale));
        _format.pushBack(JsonTree::makeArray("padding"));
        
        JsonTree& padding = _format.getChild(1);
        padding.pushBack(JsonTree("",format.padding[0]));
        padding.pushBack(JsonTree("",format.padding[1]));
        padding.pushBack(JsonTree("",format.padding[2]));
        padding.pushBack(JsonTree("",format.padding[3]));
        
        _format.pushBack(JsonTree("align",format.align));
        _format.pushBack(JsonTree("balance",format.balance));
        
        JsonTree quote;
        quote.pushBack(JsonTree("string",str));
        quote.pushBack(_format);
        
        return quote;
    }
    
    inline JsonTree writeQuoteJson(const Quote_Internal& quote){
        return writeQuoteJson(quote.str, quote.format);
    }
    
    inline Quote_Internal readQuoteJson(const JsonTree& tree){
        Quote_Internal quote;
        quote.str               = tree.getChild("string").getValue<string>();
        quote.format.scale      = tree.getChild("format.scale").getValue<float>();
        quote.format.padding[0] = tree.getChild("format.padding")[0].getValue<int>();
        quote.format.padding[1] = tree.getChild("format.padding")[1].getValue<int>();
        quote.format.padding[2] = tree.getChild("format.padding")[2].getValue<int>();
        quote.format.padding[3] = tree.getChild("format.padding")[3].getValue<int>();
        quote.format.align      = tree.getChild("format.align").getValue<int>();
        quote.format.balance    = tree.getChild("format.balance").getValue<bool>();
        return quote;
    }
    
    /*--------------------------------------------------------------------------------------------*/

public:
    Controller() : Gwen::Event::Handler(){
        ci::app::addAssetDirectory(ABS_ASSET_PATH_GWEN);
        
        mRenderer = new cigwen::GwenRendererGl();
        mRenderer->Init();
        
        Gwen::Skin::Dark* skin = new Gwen::Skin::Dark(mRenderer);
        skin->Init(ABS_ASSET_SKIN_DARK /*ABS_ASSET_SKIN_DEFAULT*/);
        
        mCanvas = new Gwen::Controls::Canvas(skin);
        setCanvasSize(ci::app::getWindowWidth(), ci::app::getWindowHeight());
        
        mInput = cigwen::GwenInput::create(mCanvas);
        
        mFileWatcher = new FileWatcher();
        
        mFileOpenWithDefaultApp = true;
        
        
        initInterface();
    }
    
    inline static ControllerRef create(){
        return std::make_shared<Controller>();
    }
    
    ~Controller(){
        delete mCanvas;
        delete mRenderer;
    }
    
    inline void connect(Grid* grid, QuoteTypesetter* typesetter){
        mGrid = grid;
        mQuoteTypesetter = typesetter;
        mWindowPreviewIso->connect(mGrid, mQuoteTypesetter);
        mWindowPreviewTex->connect(mQuoteTypesetter);
    }
    
    inline void updateLayout(){
        mWindowPreviewTex->updateLayout();
    }
    
    inline void setCanvasSize(int width, int height){
        mCanvas->SetSize(width, height);
    }
    
    inline void draw(){
        if(mFileWatcher->fileDidChange(<#const string &filePath#>))
        
        mCanvas->RenderCanvas();
    }
    
    inline void onAppFileDrop(ci::app::FileDropEvent event){
        mFileWatcher->clear();
        
    }
};



#endif
