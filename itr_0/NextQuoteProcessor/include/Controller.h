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
#include "Gwen/Controls/Label.h"

#include "Grid.h"
#include "QuoteTypesetter.h"

#include "WindowPreviewIso.h"
#include "WindowPreviewTex.h"
#include "WindowParseError.h"

#include "cinder/Json.h"

#include <cstdlib>
#include <vector>

using namespace ci;
using namespace std;

typedef std::shared_ptr<class Controller> ControllerRef;


/*--------------------------------------------------------------------------------------------*/

class Controller : public Gwen::Event::Handler {
    cigwen::GwenRendererGl*  mRenderer;
    cigwen::GwenInputRef     mInput;
    Gwen::Controls::Canvas*  mCanvas;
    WindowPreviewIso*        mWindowPreviewIso;
    WindowPreviewTex*        mWindowPreviewTex;
    WindowParseError*        mWindowParseError;
    Gwen::Controls::Label*   mLabel;
    
    
    Grid*             mGrid;
    QuoteTypesetter*  mQuoteTypesetter;
    
    
    bool                   mValid;
    FileWatcher*           mFileWatcher;
    std::string            mFilePath;
    JsonTree               mJson;
    int                    mJsonQuoteIndex;
    int                    mJsonNumQuotes;
    JsonTree::ParseOptions mJsonParseOptions;
    bool                   mFileOpenWithDefaultApp;
    
    /*--------------------------------------------------------------------------------------------*/
    // Quote Format
    /*--------------------------------------------------------------------------------------------*/
    
    
    struct QuoteFormat{
        float  scale;
        int    padding[4];
        string align;
        bool   balance;
        QuoteFormat() : scale(0.7f), align("center"), balance(false){
            padding[0] = padding[1] = padding[2] = padding[3] = 0;
            
        }
    };
    
    
    struct Quote_Internal{
        std::string str;
        QuoteFormat format;
    };
    
    /*--------------------------------------------------------------------------------------------*/
    // Gwen callbacks
    /*--------------------------------------------------------------------------------------------*/
    
    inline void onMenuItemSelected( Gwen::Controls::Base* control){
        using namespace Gwen;
        using namespace std;
        Controls::MenuItem* menuItem = (Controls::MenuItem* ) control;
        std::string menuItemIdentifier = menuItem->GetText().Get();
        
        // Create new JSON template
        if(menuItemIdentifier == "New"){
            mFileWatcher->clear();
            vector<string> exts = {"json"};
            
            fs::path filePath = app::getSaveFilePath(app::getAppPath().parent_path(),exts);
            
            if(!filePath.empty()){
                string filePathStr = filePath.string();
                JsonTree _templateObj(JsonTree::makeObject());
                JsonTree _templateArr(JsonTree::makeArray("quotes"));
                _templateArr.pushBack(writeQuoteJson("Sample String"));
                _templateObj.pushBack(_templateArr);
                
                JsonTree _template(_templateObj);
                
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
                mJsonQuoteIndex = 0;
                
                loadJson(filePathStr);
            }
        }
        
        // Load JSON
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
                mJsonQuoteIndex = 0;
                
                loadJson(filePathStr);
            }
        }
        
        // Unwatch JSON
        if(menuItemIdentifier == "Close"){
            mFileWatcher->clear();
            mJson.clear();
            mValid = false;
        }
       
    }
    
    // load json, parse, display
    inline void loadJson(const string& filePath, int prefIndex = 0){
        mValid = true;
        JsonTree temp;
        
        try{ // check if json is valid
            temp = JsonTree(loadFile(filePath),mJsonParseOptions);
        } catch (JsonTree::ExcJsonParserError& exc){
            mWindowParseError->setMessage(exc.what());
            mWindowParseError->Show();
            mValid = false;
        }
        
        
        // check if quotes are valid
        string errMsg;
        if(mValid){
            if(!temp.hasChild("quotes")){
                mWindowParseError->setMessage("Error! No quote array set.");
                mWindowParseError->Show();
                mValid  = false;
            } else {
                const JsonTree& _quotes = temp.getChild("quotes");
                for(const auto& quote : _quotes){
                    if(!isValidQuoteJson(quote,&errMsg)){
                        mWindowParseError->setMessage("Error! " + errMsg);
                        mWindowParseError->Show();
                        mValid  = false;
                        break;
                    }
                }
            }
        }
        
        // build
        if(mValid){
            mJson = temp;
            JsonTree& quotes = mJson.getChild("quotes");
            mJsonNumQuotes   = quotes.getChildren().size();
            mJsonQuoteIndex  = MAX(0,MIN(mJsonQuoteIndex,mJsonNumQuotes));
            
            JsonTree& quote  = quotes.getChild(mJsonQuoteIndex);
            JsonTree& format = quote.getChild("format");
            
            string alignStr = format.getChild("align").getValue<string>();
            QuoteTypesetter::Align align = alignStr == "left"   ? QuoteTypesetter::Align::LEFT :
                                           alignStr == "center" ? QuoteTypesetter::Align::CENTER :
                                           QuoteTypesetter::Align::RIGHT;
            
            
            mQuoteTypesetter->balanceBaseline(format.getChild("balance").getValue<bool>());
            mQuoteTypesetter->setAlign(align);
            mQuoteTypesetter->setPadding(format.getChild("padding")[0].getValue<int>(),
                                         format.getChild("padding")[1].getValue<int>(),
                                         format.getChild("padding")[2].getValue<int>(),
                                         format.getChild("padding")[3].getValue<int>());
            mQuoteTypesetter->setFontScale(MAX(0,MIN(format.getChild("scale").getValue<float>(),0.7f)));
            mQuoteTypesetter->setString(quote.getChild("string").getValue<string>());
            
            mWindowParseError->Hide();
            
            mWindowPreviewIso->drawQuote(true);
            mWindowPreviewTex->updateLayout();
            mWindowPreviewTex->drawQuote(true);
            
            mLabel->SetText(toString((mJsonQuoteIndex + 1)) + " / " + toString(mJsonNumQuotes));
            mLabel->Show();
            
        
        } else { // clear
            mLabel->Hide();
            mWindowPreviewIso->drawQuote(false);
            mWindowPreviewTex->drawQuote(false);
        }
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
        quote.format.align      = tree.getChild("format.align").getValue<string>();
        quote.format.balance    = tree.getChild("format.balance").getValue<bool>();
        return quote;
    }
    
    
    inline bool isValidQuoteJson(const JsonTree& tree, string* msg = 0){
        if (!tree.hasChildren() ) {
            *msg = "Empty object.";
            return false;
        }
        
        if(!tree.hasChild("format")){
            *msg = "Quote has no format.";
            return false;
        }
        
        if(!tree.hasChild("string")){
            *msg = "Quote has no string.";
            return false;
        }
        
        const JsonTree& format = tree.getChild("format");
        if(!format.hasChild("align")){
            *msg = "Quote format align not set.";
            return false;
        }
        
        if(!format.hasChild("balance")){
            *msg = "Quote format balance not set.";
            return false;
        }
        
        if(!format.hasChild("padding")){
            *msg = "Quote format padding not set.";
            return false;
        }
        
        if(!format.hasChild("scale")){
            *msg = "Quote format scale not set.";
            return false;
        }
        
        try {
            format.getChild("align").getValue<string>();
        } catch (JsonTree::ExcNonConvertible& exc) {
            *msg = "Quote format align is not of type float.";
            return false;
        }
        
        string align = format.getChild("align").getValue<string>();
        if(align != "center" && align != "left" && align != "right"){
            *msg = "Quote format align has wrong property.";
            return false;
        }
        
        try {
            format.getChild("balance").getValue<bool>();
        } catch (JsonTree::ExcNonConvertible& exc){
            *msg = "Quote format align is not of type bool.";
            return false;
        }
        
        if(format.getChild("padding").getChildren().size() != 4){
            *msg = "Quote format padding is not of type float.";
            return false;
        }
        
        for(int i = 0; i < 4; ++i){
            try{
                format.getChild("padding")[i].getValue<int>();
            } catch (JsonTree::ExcNonConvertible& exc){
                *msg = "Quote format padding " + toString(i) + " is not of type int.";
                return false;
            }
        }
        
        return true;
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
        mValid = false;
        mJsonParseOptions.ignoreErrors(false);
        
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
        
        mQuoteTypesetter->setString("No\nJson\nfile\nto\nwatch.");
        mValid = true;
        mWindowPreviewIso->drawQuote(true);
        mWindowPreviewTex->updateLayout();
        mWindowPreviewTex->drawQuote(true);
    }
    
    inline void setCanvasSize(int width, int height){
        mCanvas->SetSize(width, height);
    }
    
    inline void update(){
        if(!mFilePath.empty() && mFileWatcher->fileDidChange(mFilePath)){
            loadJson(mFilePath);
        }
    }
    
    inline void draw(){
        mCanvas->RenderCanvas();
    }
    
   
    
private:
    
    /*--------------------------------------------------------------------------------------------*/
    // Init Gwen
    /*--------------------------------------------------------------------------------------------*/
    
    inline void initInterface(){
        using namespace Gwen::Controls;
        
        MenuStrip* menu = new MenuStrip(mCanvas);
        menu->Dock(Gwen::Pos::Top);
        MenuItem* menuFile = menu->AddItem("File");
        menuFile->GetMenu()->AddItem("New")->SetAction(this, &Controller::onMenuItemSelected);
        menuFile->GetMenu()->AddItem("Load")->SetAction(this, &Controller::onMenuItemSelected);
        menuFile->GetMenu()->AddItem("Close")->SetAction(this, &Controller::onMenuItemSelected);
        
        
        
        mWindowPreviewIso = new WindowPreviewIso(mCanvas);
        mWindowPreviewTex = new WindowPreviewTex(mCanvas);
        mWindowParseError = new WindowParseError(mCanvas);
        mWindowParseError->setMessage("No Json file to watch.");
        
        mLabel = new Gwen::Controls::Label(mWindowPreviewIso);
        mLabel->SetText("1 / 2");
        mLabel->SetPos(9, 0);
        mLabel->Hide();
     }

};



#endif
