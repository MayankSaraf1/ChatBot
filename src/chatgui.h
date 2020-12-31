#ifndef CHATGUI_H_
#define CHATGUI_H_

#include <wx/wx.h>
#include <memory> // Task 1

class ChatLogic; // forward declaration

// middle part of the window containing the dialog between user and chatbot
class ChatBotPanelDialog : public wxScrolledWindow
{
private:
    // control elements
    wxBoxSizer *_dialogSizer;
    wxBitmap _image;

    //// STUDENT CODE
    ////

    //ChatLogic *_chatLogic; // Task 1 - explanation below 
    std::unique_ptr<ChatLogic> _chatLogic; // Task 1 - explanation below
    /* ----------- Task 1: Explanation --------------
     * _chatLogic has been changed to a smart pointer. Earlier, _chatLogic was a raw pointer to the heap resource.
     * Now, _chatLogic is a unique_ptr and recides on the stack. When _chatLogic goes out of scope, the destructor of
     * _chatLogic is called which automatically deletes the heap resouce. Therefore, we can remove the manual deletion
     * of the heap resouce currently being done in ChatBotPanelDialog destructor.
     * ----------------------------------------------- */

    ////
    //// EOF STUDENT CODE

public:
    // constructor / destructor
    ChatBotPanelDialog(wxWindow *parent, wxWindowID id);
    ~ChatBotPanelDialog();

    // getter / setter
    // ChatLogic *GetChatLogicHandle() { return _chatLogic; } // Task 1 - explanation below 
    ChatLogic *GetChatLogicHandle() { return _chatLogc.get(); } // Task 1 - explanation below 
    /* ----------- Task 1: Explanation --------------
     * Per the prototype of GetChatLogicHandle() method. it returns the pointer to the ChatLogic object. 
     * Originally, _chatLogic was a raw pointer. However, now _chatLogic is a smart pointer and hence we need
     * to use the .get() method to be able to return the raw pointer of the  ChatLogic object. 

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    // proprietary functions
    void AddDialogItem(wxString text, bool isFromUser = true);
    void PrintChatbotResponse(std::string response);

    DECLARE_EVENT_TABLE()
};

// dialog item shown in ChatBotPanelDialog
class ChatBotPanelDialogItem : public wxPanel
{
private:
    // control elements
    wxStaticBitmap *_chatBotImg;
    wxStaticText *_chatBotTxt;

public:
    // constructor / destructor
    ChatBotPanelDialogItem(wxPanel *parent, wxString text, bool isFromUser);
};

// frame containing all control elements
class ChatBotFrame : public wxFrame
{
private:
    // control elements
    ChatBotPanelDialog *_panelDialog;
    wxTextCtrl *_userTextCtrl;

    // events
    void OnEnter(wxCommandEvent &WXUNUSED(event));

public:
    // constructor / desctructor
    ChatBotFrame(const wxString &title);
};

// control panel for background image display
class ChatBotFrameImagePanel : public wxPanel
{
    // control elements
    wxBitmap _image;

public:
    // constructor / desctructor
    ChatBotFrameImagePanel(wxFrame *parent);

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    DECLARE_EVENT_TABLE()
};

// wxWidgets app that hides main()
class ChatBotApp : public wxApp
{
public:
    // events
    virtual bool OnInit();
};

#endif /* CHATGUI_H_ */
