#include <stdio.h>

enum EXECUTION_STATUS {
    FREE,
    READY,
    SLEEP,
    BLOCK,
    ZOMBIE
};

class ProcessNode {
    private:
    int processID;
    int processParentID;
    int execStatus;
    int exitStatus;

    ProcessNode * leftSibNode;
    ProcessNode * rightSibNode;
    ProcessNode * parentNode;
    ProcessNode * nextNode;

    int * processStack;
    int * savedStackPointer;

    public:
    ProcessNode (
        int processID = 0,
        int stackSize = 0,
        EXECUTION_STATUS status = READY,
        ProcessNode * parent = nullptr,
        ProcessNode * leftSib = nullptr,
        ProcessNode * rightSib = nullptr,
        ProcessNode * nextNode = nullptr
    ) {
        this->execStatus = status;
        this->exitStatus = 0;
        this->setProcessID(processID);
        this->setParent(parent);
        this->setLeftSibling(leftSib);
        this->setRightSibling(rightSib);
        this->setNext(nextNode);
        this->processStack = nullptr;
        if (stackSize > 0) { this->createProcessStack(stackSize); }
        this->savedStackPointer = nullptr;
    }
    
    /***************************************/
    /************** Accessors **************/
    /***************************************/
    ProcessNode * getLeftSibling()  { return this->leftSibNode; }
    ProcessNode * getRightSibling() { return this->rightSibNode; }
    ProcessNode * getParent()       { return this->parentNode; }
    ProcessNode * getNext()         { return this->nextNode; }
    
    int * getProcessStack()         { return this->processStack; }
    int * getSavedStackPointer()    { return this->savedStackPointer; }

    int getProcessID()          { return this->processID; }
    int getProcessParentID()    { return this->processParentID; }
    int getExecutionStatus()    { return this->execStatus; }
    int getExitStatus()         { return this->exitStatus; }


    /***************************************/
    /************** Mutators ***************/
    /***************************************/
    void setLeftSibling(ProcessNode * left)     { this->leftSibNode = left; }
    void setRightSibling(ProcessNode * right)   { this->rightSibNode = right; }
    void setNext(ProcessNode * next)            { this->nextNode = next; }
    void setParent(ProcessNode * parent) {
        this->parentNode = parent;
        this->processParentID = parent->processID;
    }

    bool createProcessStack(unsigned int size) {
        if (size > 0) {
            int * newStack = new int[size];
            if (newStack != nullptr) {
                processStack = newStack;
                return true;
            }
        }
        return false;
    }

    bool resizeProcessStack(unsigned int newSize) {
        // #TODO: This
        return createProcessStack(newSize);
    }

    void setProcessID(unsigned int pid) { this->processID = pid; }
    void setExecutionStatus(EXECUTION_STATUS status) { this->execStatus = status; }
    void setExitStatus(int status) { this->exitStatus = status; }
};