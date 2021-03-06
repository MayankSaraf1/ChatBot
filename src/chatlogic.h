#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <vector>
#include <string>
#include "chatgui.h"
#include <memory> // Task 3

// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:
    //// STUDENT CODE - Task 3 and 4
    ////

    // data handles (owned)
    // std::vector<GraphNode *> _nodes; // Task 3 - explanation below
    std::vector<std::unique_ptr<GraphNode>> _nodes; // Task 3 - explanation below
    /* --------------- Task 3 - Explanation ---------------------------------------
     * _nodes now contain a vector of unique_ptr and not raw pointers.
     * This allows us not manually delete objects of GraphNode class on the heap
     * -----------------------------------------------------------------------------*/

    // std::vector<GraphEdge *> _edges; // Task 4 
    /* --------------- Task 4 - Explanation ---------------------------------------
     * ChatLogic doesn't manage the vector of GraphNodes anymore. 
     * Now, the parent nodes (in GraphNode) manage the child edges (_childEdges) connected to it.
     * -----------------------------------------------------------------------------*/

    ////
    //// EOF STUDENT CODE - Task 3 and 4

    // data handles (not owned)
    // GraphNode *_currentNode; // The handle to current node is not needed inside the ChatLogic object
    ChatBot *_chatBot;
    ChatBotPanelDialog *_panelDialog;

    // proprietary type definitions
    typedef std::vector<std::pair<std::string, std::string>> tokenlist;

    // proprietary functions
    template <typename T>
    void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
    // constructor / destructor
    ChatLogic();
    ~ChatLogic();

    // getter / setter
    void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
    void SetChatbotHandle(ChatBot *chatbot);

    // proprietary functions
    void LoadAnswerGraphFromFile(std::string filename);
    void SendMessageToChatbot(std::string message);
    void SendMessageToUser(std::string message);
    wxBitmap *GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */