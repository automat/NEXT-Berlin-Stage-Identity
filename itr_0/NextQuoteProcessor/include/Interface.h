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

#include "Resources.h"
#include "Config.h"
#include "Properties.h"

#include "cinder/app/App.h"

#include "cigwen/CinderGwen.h"
#include "cigwen/GwenRendererGl.h"
#include "cigwen/GwenInput.h"
#include "Gwen/Skins/Dark.h"
#include "Gwen/Events.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/CrossSplitter.h"
#include "Gwen/Controls.h"

#include "Grid.h"
#include "QuoteTypesetter.h"

#include "WindowPreviewIso.h"
#include "WindowPreviewTex.h"


typedef std::shared_ptr<class Interface> InterfaceRef;

class Interface : public Gwen::Event::Handler {
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
    
    
    inline void init(){
        using namespace Gwen::Controls;
        
        //
        //  Menu
        //
        
        MenuStrip* menu = new MenuStrip(mCanvas);
        menu->Dock(Gwen::Pos::Top);
        menu->AddItem("Close");
        menu->AddItem("File");
        
        {
            MenuItem* menuView = menu->AddItem("View");
            
            MenuItem* viewSettings = menuView->GetMenu()->AddItem("Settings");
            viewSettings->SetCheckable(true);
            viewSettings->SetChecked(true);
            viewSettings->SetAction(this, &Interface::onMenuItemSelected);
            MenuItem* viewFontInspector = menuView->GetMenu()->AddItem("Font Inspector");
            viewFontInspector->SetCheckable(true);
            viewFontInspector->SetChecked(true);
            viewFontInspector->SetAction(this, &Interface::onMenuItemSelected);
            MenuItem* viewTextInput = menuView->GetMenu()->AddItem("Text Input");
            viewTextInput->SetCheckable(true);
            viewTextInput->SetChecked(true);
            viewTextInput->SetAction(this, &Interface::onMenuItemSelected);
            MenuItem* viewConsole = menuView->GetMenu()->AddItem("Console");
            viewConsole->SetCheckable(true);
            viewConsole->SetChecked(false);
            viewConsole->SetAction(this, &Interface::onMenuItemSelected);
            MenuItem* viewFilePreview = menuView->GetMenu()->AddItem("File Preview");
            viewFilePreview->SetCheckable(true);
            viewFilePreview->SetChecked(false);
            viewFilePreview->SetAction(this, &Interface::onMenuItemSelected);
        }
        
        
        
        
        //mWindowConsole = new WindowControl(mCanvas);
        //mWindowConsole->Dock(Gwen::Pos::Fill);
        

        mWindowPreviewIso = new WindowPreviewIso(mCanvas);
        mWindowPreviewIso->SetTitle("Isometric View");
        mWindowPreviewIso->SetWidth(ci::app::getWindowWidth() * 0.5f);
        mWindowPreviewIso->SetClosable(false);
        mWindowPreviewIso->SetTabable(false);
        mWindowPreviewIso->Dock(Gwen::Pos::Left);
        
        mWindowPreviewTex = new WindowPreviewTex(mCanvas);
        mWindowPreviewTex->SetTitle("Texture View");
        mWindowPreviewTex->SetClosable(false);
        mWindowPreviewTex->SetTabable(false);
        mWindowPreviewTex->Dock(Gwen::Pos::Fill);
        
        
        //
        //  Windows
        //
        
        
        mWindowInspector = new WindowControl(mCanvas);
        mWindowInspector->SetSize(300,400);
        mWindowInspector->SetTitle("Font Inspector");
        mWindowInspector->SetPos(30, 30);
        mWindowInspector->SetDeleteOnClose(false);
        mWindowInspector->Hide();
        
        
        
        
    
        
    
        
    }
    
    inline void onMenuItemSelected( Gwen::Controls::Base* control){
        using namespace Gwen;
        using namespace std;
        Controls::MenuItem* menuItem = (Controls::MenuItem* ) control;
        std::string menuItemIdentifier = menuItem->GetText().Get();
        
        if(menuItemIdentifier == "Settings"){
            cout << "Settings" << endl;
            
        }
        
        if( menuItemIdentifier == "Font Inspector"){
            cout << "Font Inspector" << endl;
            if(mWindowInspector->Hidden()){
                mWindowInspector->Show();
            } else {
                mWindowInspector->Hide();
            }
            
        }
        
        if( menuItemIdentifier == "Text Input"){
            cout << "Text Input" << endl;
            
        }
        
        if( menuItemIdentifier == "Console") {
            cout << "Console" << endl;
            
        }
        
        if( menuItemIdentifier == "File Preview"){
            cout << "File Preview" << endl;
            
        }
        
        
        /*
        switch (menuItem->GetText().GetUnicode().c_str()) {
            case "Settings":
                cout << "Settings" << endl;
                break;
                
            default:
                break;
        }
         */
    }
    
public:
    Interface() : Gwen::Event::Handler(){
        ci::app::addAssetDirectory(ABS_ASSET_PATH_GWEN);
        
        mRenderer = new cigwen::GwenRendererGl();
        mRenderer->Init();
        
        Gwen::Skin::Dark* skin = new Gwen::Skin::Dark(mRenderer);
        skin->Init(ABS_ASSET_SKIN_DARK);
        
        mCanvas = new Gwen::Controls::Canvas(skin);
        setCanvasSize(ci::app::getWindowWidth(), ci::app::getWindowHeight());
        
        mInput = cigwen::GwenInput::create(mCanvas);
        
        init();
    }
    
    inline static InterfaceRef create(){
        return std::make_shared<Interface>();
    }
    
    ~Interface(){
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
        mCanvas->RenderCanvas();
    }
};



#endif
