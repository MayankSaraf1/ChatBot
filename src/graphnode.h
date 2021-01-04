#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include <memory> // Task 4
#include "chatbot.h"

// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE - Task 4 and 5
    ////

    // data handles (owned)
    // std::vector<GraphEdge *> _childEdges;  // edges to subsequent nodes // Task 4
    std::vector<std::unique_ptr<GraphEdge>> _childEdges;  // edges to subsequent nodes // Task 4 - explanation below
    
    /* --------------- Task 4 - Explanation ---------------------------------------
     * The parent node owns the vector of child edges. As the ownership is exclusive, 
     * we should use unique_ptr 
     * ----------------------------------------------------------------------------*/

    // data handles (not owned)
    std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes 
    
    // ChatBot *_chatBot; // Task 5
    ChatBot _chatBotObject; // Task 5 
    /* --------------- Task 5 - Explanation ---------------------------------------
     * Originally ChatBot object was managed by ChatLogic on the heap. Now, ChatBot object is contained inside GraphNode object. The handles inside the Chatbot object is moved from node to node
     * Each GraphNode object will instantiate an object of the ChatBot class. Default constructor WITHOUT memory allocation will get invoked for each node.
     * ----------------------------------------------------------------------------*/

    ////
    //// EOF STUDENT CODE

    // proprietary members
    int _id;
    std::vector<std::string> _answers;

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode();

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(std::string token); // add answers to list
    void AddEdgeToParentNode(GraphEdge *edge);
    // void AddEdgeToChildNode(GraphEdge *edge); // Task 4
    void AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge); // Task 4

    //// STUDENT CODE - Task 5
    ////

    // void MoveChatbotHere(ChatBot *chatbot); // Task 5
    void MoveChatbotHere(ChatBot MoveableChatBotObject); // Task 5

    ////
    //// EOF STUDENT CODE - Task 5

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */