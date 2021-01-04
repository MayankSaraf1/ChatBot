#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"


ChatLogic::ChatLogic()
{
    //// STUDENT CODE - Task 5
    ////

    // create instance of chatbot
    // _chatBot = new ChatBot("../images/chatbot.png"); // Task 5
    
    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    // _chatBot->SetChatLogicHandle(this); // Task 5

    /* --------------- Task 5 - Explanation ---------------------------------------
     * As a part of Task 5, the ChatBot instance will not be created on the heap, 
     * rather, it will be created on the stack in the LoadAnswerGraphFromFile method.
     * -----------------------------------------------------------------------------*/

    ////
    //// EOF STUDENT CODE - Task 5
}

ChatLogic::~ChatLogic()
{ 
    //// STUDENT CODE - Task 3, 4 and 5
    ////

    // delete chatbot instance 
    // delete _chatBot; // Task 5

    //// delete all nodes
    // for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    // {
    //     delete *it; // typeid(*it).name() is Pint. Therefore, to get/delete the pointer to int we need to use *it. "it" contains some sort of vector iterator.
    // }
    /* --------------- Task 3 - Explanation ---------------------------------------
     * As _nodes now contain a vector of unique_ptr pointers to GraphNode objects, 
     * therefore, the deletion of the objects is automated and we don't need to manually 
     * delete the nodes 
     * -----------------------------------------------------------------------------*/
    
    // // delete all edges
    // for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
    // {
    //     delete *it;
    // }
    /* --------------- Task 4 - Explanation ---------------------------------------
     * As ChatLogic class doesn't own _edges vector, we don't have to worry about deleting the edges
     * ----------------------------------------------------------------------------*/

    ////
    //// EOF STUDENT CODE - Task 3, 4 and 5
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        //// STUDENT CODE - Task 3
                        ////

                        // check if node with this ID exists already
                        // auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](GraphNode *node) { return node->GetID() == id; }); // Task 3 - explanation below

                        auto id_match_fn = [&id](const std::unique_ptr<GraphNode> &node) { return node->GetID() == id; }; // Task 3 - explanation below
                        /* --------------- Task 3 - Explanation ---------------------------------------
                         * auto id_match_fn = [capture - local variable inside the function](argument to the function){ function body};
                         * capture can be passed by reference as there is no real reason to pass it by value. We are anyway not modifying it inside the function.
                         * Originally, the argument to the function was a raw GraphNode pointer as the _nodes was a vector of raw pointers.
                         * Now, _nodes is a vector of unique_pointers, therefore, the function will accept the smart pointer as an argument.
                         * As we don't want to transfer ownership of the smart pointer, we will pass it by reference. Also, as we don't want the lambda
                         * to modify the smart pointer, we should mark it with a const qualifier. 
                         * -----------------------------------------------------------------------------*/
                        
                        // check if node with this ID exists already
                        std::vector<std::unique_ptr<GraphNode>>::iterator newNode = std::find_if(_nodes.begin(), _nodes.end(), id_match_fn);
                        /* --------------- Task 3 - Explanation ---------------------------------------
                        // http://www.cplusplus.com/reference/algorithm/find_if/                       
                         * find_if : returns an iterator oject to the 1st element in the vector which satisfies the lamba function (argument 3).  
                         * Earlier newNode was the iterator object to the raw pointer, now it is an iterator to the unique_ptr. 
                         * To get the raw pointer we need to use the .get() method i.e newNode.get()
                         * -----------------------------------------------------------------------------*/

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                            //_nodes.emplace_back(new GraphNode(id)); // Task 3 - explanation below
                            _nodes.emplace_back(std::make_unique<GraphNode>(id)); // Task 3 - explanation below
                            /* --------------- Task 3 - Explanation ---------------------------------------
                             * Now _nodes contain a vector of unique_ptr of GraphNode objects on the heap
                             * -----------------------------------------------------------------------------*/
                            
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                        ////
                        //// EOF STUDENT CODE - Task 3
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
                        //// STUDENT CODE - Task 3
                        ////

                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            // get iterator on incoming and outgoing node via ID search
                            // auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](GraphNode *node) { return node->GetID() == std::stoi(parentToken->second); }); // Task 3
                            // auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](GraphNode *node) { return node->GetID() == std::stoi(childToken->second); }); // Task 3 
                            
                            // get iterator on incoming and outgoing node via ID search                           
                            std::vector<std::unique_ptr<GraphNode>>::iterator  parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(parentToken->second); }); //Task 3
                            std::vector<std::unique_ptr<GraphNode>>::iterator  childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](std::unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(childToken->second); }); //Task 3
                            // ------------------ Regarding dereferencing operator.----------------------
                            // As smart pointers overload dereferencing operator, therefore node->GetID() and node.get()->GetID() are identical
                            // Basically, we can access the member functions directly from the smart pointers and we don't need to get down to the raw pointer level
                            // Originally, auto keyword was taking care of the type. I put an explicit type because they are both iterators objects

                            // create new edge 
                            // GraphEdge *edge = new GraphEdge(id); // Task 4 - edge should be a unique_ptr
                            std::unique_ptr<GraphEdge> edge = std::make_unique<GraphEdge>(id); // Task 4

                            // edge->SetChildNode(*childNode); // Task 3 
                            // edge->SetParentNode(*parentNode); // Task 3

                            edge->SetChildNode((*childNode).get()); // Task 3 - Explanation below 
                            edge->SetParentNode((*parentNode).get()); // Task 3 - Explanation below 
                            /* ------------------Regarding parentNode and childNode ------------------------------------
                             * To access the smart pointer, we need to dereference like so. (*parentNode)
                             * To access the raw pointer, we need to call the .get() method on the smart pointer.. like so. (*parentNode).get()
                             * ------------------------------------------------------------------------------------------ */

                            // _edges.push_back(edge); // Task 4
                            // ChatLogic doesn't manage the vector of GraphEdges anymore and have gotten rid of _edges vector from ChatLogic

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            // store reference in child node and parent node
                            // (*childNode)->AddEdgeToParentNode(edge); // Task 4
                            // (*parentNode)->AddEdgeToChildNode(edge); // Task 4

                            // store reference in child node and parent node
                            (*childNode)->AddEdgeToParentNode(edge.get()); // Task 4
                            /* --------------- Task 4 - Explanation ---------------------------------------
                             * As childNode doesn't own the edge, it stores the raw pointer of the edge. We use .get() to get the raw pointer
                             * (*childNode) gets the smart pointer from the iterator object. Then we can directly access the GraphNode member function.
                             * -----------------------------------------------------------------------------*/
                            (*parentNode)->AddEdgeToChildNode(std::move(edge)); // Task 4
                            /* --------------- Task 4 - Explanation ---------------------------------------
                             * parentNode owns the edge(s) connected to it. 
                             * As edge is unique_ptr, we need to transfer ownership by using the std::move on the "edge" unique_ptr
                             * as we add this edge to the parent node
                             * -----------------------------------------------------------------------------*/
                            
                        }

                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE  - Task 3 and 5
    ////

    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0) // We can access member functions with smart pointers as well.
        {

            if (rootNode == nullptr)
            {
                // rootNode = *it; // assign current node to root // Task 3
                rootNode = it->get(); // assign current node to root // Task 3 - rootNode is a raw pointer and hence we need to use the .get() method
            }
            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

    // create instance of chatbot (on the stack).
    ChatBot MoveableChatBotObject("../images/chatbot.png"); // Task 5
    
    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    // _chatBot->SetChatLogicHandle(this); // Task 5
    MoveableChatBotObject.SetChatLogicHandle(this); // Task 5 - This is needed to ensure ChatBot can reach ChatLogic object
    this->SetChatbotHandle(&MoveableChatBotObject); // Task 5 - This is needed to ensure ChatLogic can reach ChatBot object.

    // add chatbot to graph root node
    // _chatBot->SetRootNode(rootNode); // Task 5
    // rootNode->MoveChatbotHere(_chatBot); // Task 5

    // add chatbot to graph root node
    MoveableChatBotObject.SetRootNode(rootNode); // Task 5
    rootNode->MoveChatbotHere(std::move(MoveableChatBotObject)); // Task 5
    /* --------------- Task 5 - Explanation ---------------------------------------
     * MoveableChatBotObject is a l-value and its scope is limited to LoadAnswerGraphFromFile() method.
     * When we call std::move on MoveableChatBotObject, we convert it to an r-value. In the next line on caller side, MoveableChatBotObject will cease to exist once it has been converted to a r-value
     * MoveChatbotHere() method, accepts a r-value referece. Essentially, inside MoveChatbotHere() method, MoveableChatBotObject will exisit as a l-value and will continue to exist in its scope.
     * -----------------------------------------------------------------------------*/
    
    ////
    //// EOF STUDENT CODE - Task 3 and 5
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()
{
    return _chatBot->GetImageHandle();
}
