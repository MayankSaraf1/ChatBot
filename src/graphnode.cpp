#include "graphedge.h"
#include "graphnode.h"

GraphNode::GraphNode(int id)
{
    _id = id;
}

GraphNode::~GraphNode()
{
    //// STUDENT CODE - Task 0
    ////

    // delete _chatBot; // See explanation below
    /* ----------- TASK 0: Explanation --------------
     * Per the legacy student version of the code, the _chatBot destructor was getting called
     * every time a GraphNode was deleted. However, we know there is only 1 instance _chatBot 
     * which gets passed around from node to node. Therefore, it is incorrect to delete the 
     * _chatBot instance inside the destructor for graphNode (i.e. line above). The correct 
     * place is inside the chatLogic destructor (already there)
     * ----------------------------------------------- */
    ////
    //// EOF STUDENT CODE - Task 0
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
// void GraphNode::AddEdgeToParentNode(std::unique_ptr<GraphEdge> edge)
{
    _parentEdges.push_back(edge);
}

// void GraphNode::AddEdgeToChildNode(GraphEdge *edge)
void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge)
{
    _childEdges.push_back(std::move(edge)); // Task 4
    /* ----------- TASK 4: Explanation --------------
     * We have to use std::move to ensure the ownership of the "edge" unique_ptr is transferred to the _childEdges vector and not copied
     * ----------------------------------------------- */
}

//// STUDENT CODE - Task 5: Moving the ChatBot
////
// void GraphNode::MoveChatbotHere(ChatBot *chatbot) // Task 5
// {
//     _chatBot = chatbot;
//     _chatBot->SetCurrentNode(this);
// }

// MoveChatbotHere() method has been modifed to accept a r-value reference. Now, MoveableChatBotObject exists as a l-value in the scope of MoveChatbotHere() method
void GraphNode::MoveChatbotHere(ChatBot&& MoveableChatBotObject) // Task 5
{
    _chatBotObject = std::move(MoveableChatBotObject); // MoveableChatBotObject is moved to the current node. 
    _chatBotObject.SetCurrentNode(this); // Set the current node handle in _chatBotObject as well. 
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    // newNode->MoveChatbotHere(_chatBot); // Task 5
    newNode->MoveChatbotHere(std::move(_chatBotObject)); // Task 5
    // _chatBot = nullptr; // invalidate pointer at source // Task 5
}
////
//// EOF STUDENT CODE - Task 5: Moving the ChatBot

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE - Task 4
    ////

    // return _childEdges[index]; // Task 4
    return _childEdges[index].get(); // Task 4
    /* ----------- TASK 4: Explanation --------------
     * As _childEdges is a vector of unique_ptr, we need to use the .get() method to return the raw GraphEdge pointer
     * ----------------------------------------------- */

    ////
    //// EOF STUDENT CODE - Task 4
}