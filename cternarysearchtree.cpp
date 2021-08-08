#include "cternarysearchtree.h"
#include <QDebug>

CTernarySearchTree::CTernarySearchTree()
{
    root = NULL;
    pairDictionary = NULL;
}

CTernarySearchTree::~CTernarySearchTree()
{
    deleteNode(&root);
    pairDictionary = NULL;  // pair dictionary must be deleted by the owner
}

/**
 * @brief CTernarySearchTree::Insert : Calls internal Search TST method
 * @param word
 */
void CTernarySearchTree::insert(const QString word,
                                const QString uiq, const QString desc)
{
    insert(&root, word.toStdString().c_str(), uiq.toStdString().c_str(),
           desc.toStdString().c_str());
}

/**
 * @brief CTernarySearchTree::Insert : recursive function
 * to Insert a new word in TST
 * @param node
 * @param word
 */
void CTernarySearchTree::insert(TernarySearchTreeNode **node,
                                const char *word, const char *uiq, const char *desc)
{
    if (*node == NULL)
        *node = new TernarySearchTreeNode(*word);

    if ((*word) < (*node)->data)
    {
        insert(&((*node)->left), word, uiq, desc);
    }
    else if ((*word) > (*node)->data)
    {
        insert(&((*node)->right), word, uiq, desc);
    }
    else
    {
        if(*(word + 1))
            insert(&((*node)->eq), word + 1, uiq, desc);
        else
        {
            (*node)->isEOW = true;
            (*node)->setExData(uiq, desc);
        }
    }
}

/**
 * @brief CTernarySearchTree::DeleteWord : Driver functio to call internal
 * delete word function
 * @param QString word
 */
void CTernarySearchTree::deleteWord(const QString& word, const QString& uiq)
{
    if(search(root, word.toStdString().c_str()) != NULL)
        deleteWord(&root, word.toStdString().c_str(), uiq);
}

/**
 * @brief CTernarySearchTree::searchWord : Searches TST and returns auto complete list
 * @param word
 * @param autoCompleteListLimit
 * @return QList<QString> AutoCompleteWordList
 */
QList<LocateDataStruct> *CTernarySearchTree::search(
        QString word,
        int autoCompleteListLimit) const
{
    TernarySearchTreeNode *selectedNode = search(root, word.toStdString().c_str());
    auto autoCompleteList = traverseTree(selectedNode, word, autoCompleteListLimit);

    if(pairDictionary != NULL)
        searchInPairDictionary(word, autoCompleteList, autoCompleteListLimit);

    return autoCompleteList;
}

/**
 * @brief CTernarySearchTree::ListEntireDictionary : Returns entire dictionry from root node
 * @warning Will be performance intensive for very large dictionaries; eg: 1 lakh entries
 * @note This function doesn't search in pair dictionary (if any)
 * @param autoCompleteListLimit
 * @return QList<QString> AutoCompleteWordList
 */
QList<LocateDataStruct> *CTernarySearchTree::listEntireDictionary(int autoCompleteListLimit) const
{
    auto autoCompleteList = new QList<LocateDataStruct>();
    QString emptyWord = "";
    traverseTree(this->root, *autoCompleteList, emptyWord, autoCompleteListLimit);

    return autoCompleteList;
}

/**
 * @brief CTernarySearchTree::AddPairDictionary : Add a second dictionary for searching
 * with different id
 * @param pairDictionary
 */
void CTernarySearchTree::AddPairDictionary(CTernarySearchTree *pairDictionary)
{
    this->pairDictionary = pairDictionary;
}


/**
 * @brief CTernarySearchTree::Search : recursive function
 * to search a word in the TST and return a node* to end of the found word;
 * else null*
 * @param node
 * @param word
 * @return TernarySearchTreeNode* Searched node
 */
TernarySearchTreeNode* CTernarySearchTree::search(
        TernarySearchTreeNode *node, const char *word) const
{
    if (node == NULL)
    {
        return NULL;
    }

    if(*word < node->data)
    {
        return search(node->left, word);
    }
    else if(*word > node->data)
    {
        return search(node->right, word);
    }
    else
    {
        if(*(word + 1) == '\0')
        {
            return node;
        }

        return search(node->eq, word + 1);
    }
}

/**
 * @brief CTernarySearchTree::TraverseTree : To call internal function TraverseTree
 * for traversing the tree from given node
 * @param node
 * @param autoCompleteListLimit
 * @return QList<QString> AutoCompleteList
 */
QList<LocateDataStruct>* CTernarySearchTree::traverseTree(
        TernarySearchTreeNode *node, QString phrase, int autoCompleteListLimit) const
{
    auto autoCompleteWordList = new QList<LocateDataStruct>();

    if (node != NULL)
    {
        // special case; to include search phrase in auto complete list
        if(node->isEOW)
        {
            appendNodeData(*autoCompleteWordList, node, phrase);
        }

        traverseTree(node->eq, *autoCompleteWordList, phrase, autoCompleteListLimit);
    }

    return autoCompleteWordList;
}

/**
 * @brief CTernarySearchTree::TraverseTree : Recursive function to trace
 * tree from given node
 * @param node
 * @param autoCompleteList
 * @param str
 * @param autoCompleteListLimit
 */
void CTernarySearchTree::traverseTree(TernarySearchTreeNode *node,
                                      QList<LocateDataStruct> &autoCompleteList,
                                      QString str,
                                      int autoCompleteListLimit) const
{
    if (node != NULL && autoCompleteList.count() < autoCompleteListLimit)
    {
        traverseTree(node->left, autoCompleteList, str, autoCompleteListLimit);

        QString rStr = str; // for traversing right subtree
        str.append(node->data);

        if (node->isEOW)
        {
            appendNodeData(autoCompleteList, node, str);
        }

        traverseTree(node->eq, autoCompleteList, str, autoCompleteListLimit);
        traverseTree(node->right, autoCompleteList, rStr, autoCompleteListLimit);
    }
}

/**
 * @brief CTernarySearchTree::Delete TST from selected node
 * @param node
 */
void CTernarySearchTree::deleteNode(TernarySearchTreeNode **node)
{
    if(*node == NULL)
        return;

    // delete left
    deleteNode(&((*node)->left));

    // delete eq
    deleteNode(&((*node)->eq));

    // delete right
    deleteNode(&((*node)->right));

    if ((*node)->left == NULL &&
            (*node)->eq == NULL &&
            (*node)->right == NULL)
    {
        delete(*node);
        *node = NULL;
    }
}

/**
 * @brief CTernarySearchTree::DeleteWord : Recursive function to delete a word from TST
 * deletes if word is unique; resets EOF flag if part of another word
 * @param node
 * @param word
 * @return boolean
 */
bool CTernarySearchTree::deleteWord(TernarySearchTreeNode **node, const char *word, const QString& uiq)
{
    if (node == NULL)
        return false;

    if(*word < (*node)->data)
    {
        bool deletionRequired = deleteWord(&((*node)->left), word, uiq);

        // coming from left subtree
        if (deletionRequired)
        {
            // is a free node
            if(!isEndOfWord(*node) && isFreeNode(*node))
            {
                delete(*node);
                *node = NULL;
                return true;
            }
        }
    }
    else if(*word > (*node)->data)
    {
        bool deletionRequired = deleteWord(&((*node)->right), word, uiq);

        // coming from right subtree
        if (deletionRequired)
        {
            // is a free node
            if(!isEndOfWord(*node) && isFreeNode(*node))
            {
                delete(*node);
                *node = NULL;
                return true;
            }
        }
    }
    else
    {
        if (*(word + 1) == '\0')
        {
            // found word
            // deletion required
            if (isFreeNode(*node))
            {
                // remove uiq
                (*node)->removeExData(uiq);

                if ((*node)->exDataList.isEmpty())
                {
                    delete(*node);
                    *node = NULL;
                    return true;
                }
                return false;
            }
            else
            {
                // not a free node
                    // 1. could be a joint
                    // 2. could be part of a limb

                // set eof as false, remove uiq
                (*node)->removeExData(uiq);
                (*node)->isEOW = false;
                return false;
            }
        }

        // coming from center subtree
        bool deletionRequired = deleteWord(&((*node)->eq), word + 1, uiq);
        if (deletionRequired)
        {
            // is part of a word
            if (isEndOfWord(*node))
            {
                // do not delete any further
                return false;
            }

            // is a free node
            else if(isFreeNode(*node))
            {
                delete(*node);
                *node = NULL;
                return true;
            }

            // is a joint
            else
            {
                //  if LST exists
                if((*node)->left != NULL)
                {
                    // 1. find right most parent in left sub-tree
                    TernarySearchTreeNode **RMP = &(*node)->left;

                    // if first left node has no right subtree
                    if((*RMP)->right == NULL)
                    {
                        TernarySearchTreeNode *temp = *node;
                        *node = (*node)->left;
                        (*node)->right = temp->right;
                    }
                    else
                    {
                        while((*RMP)->right != NULL)
                            RMP = &(*RMP)->right;

                        TernarySearchTreeNode *tempRMP = *RMP;
                        // 2. check if RMP has left sub-tree

                        // 2.1 if RMP has left sub-tree
                        // attach parent of RMP's right ptr to left sub-tree
                        // of RMP
                        if ((*RMP)->left != NULL)
                        {
                            *RMP = (*RMP)->left;
                        }
                        // 2.2 else RMP parent's right ptr = NULL
                        else
                        {
                            *RMP = NULL;
                        }

                        // 3. delete center node to be deleted
                        TernarySearchTreeNode *tempNode = *node;

                        // 4. replace deleted node with RMP
                        // RMP takes left and right children of deleted node
                        // and retains it's center child
                        *node = tempRMP;
                        (*node)->right = tempNode->right;
                        (*node)->left = tempNode->left;
                    }
                }
                else
                {
                    TernarySearchTreeNode *temp = *node;
                    // first right parent
                    *node = (*node)->right;

                    delete(temp);
                }

            }

        }

    }

    return false;
}

/**
 * @brief CTernarySearchTree::isFreeNode : Checks if a node has any childern or not
 * @param node
 * @return boolean
 */
bool CTernarySearchTree::isFreeNode(TernarySearchTreeNode *node) const
{
    return (node->left == NULL &&
            node ->right == NULL &&
            node ->eq == NULL);
}

/**
 * @brief CTernarySearchTree::isEndOfWord : Returns if this node is End Of Word node
 * @param node
 * @return
 */
bool CTernarySearchTree::isEndOfWord(TernarySearchTreeNode *node) const
{
    return node->isEOW;
}

/**
 * @brief CTernarySearchTree::appendNodeData : Append all words in one node into auto complete list.
 * An EOW node can have different uiqs
 * @param list
 * @param node
 * @param str
 */
void CTernarySearchTree::appendNodeData(QList<LocateDataStruct> &list,
        TernarySearchTreeNode *node, QString str)
{
    // for different uiq with data, add as a different element in list
    for (auto iter = node->exDataList.begin(); iter != node->exDataList.end(); ++iter)
    {
        list.append(LocateDataStruct(str, iter->uiq, iter->desc));
    }
}

/**
 * @brief CTernarySearchTree::SearchInPairDictionary : Search in Pair Dictionary, and combine the results with main dictionary
 * @param word
 * @param parentAutoCompleteList
 * @param autoCompleteListLimit
 */
void CTernarySearchTree::searchInPairDictionary(
        QString word,
        QList<LocateDataStruct> *parentAutoCompleteList,
        int autoCompleteListLimit) const
{
    auto autoCompleteList = pairDictionary->search(word, autoCompleteListLimit);
    for (auto iter = autoCompleteList->begin(); iter != autoCompleteList->end(); ++iter )
    {
        // replace desc with data; TODO: remove later, quick hack
        LocateDataStruct data(iter->desc, iter->uiq, iter->data);

        // find iter in parent auto complete list
        if (!parentAutoCompleteList->contains(data))
        {
            parentAutoCompleteList->prepend(data);
        }
    }
}

/**
 * @brief CTernarySearchTree::clearDictionary : Clears entire dictionary
 */
void CTernarySearchTree::clearDictionary()
{
   deleteNode(&root);
   root = NULL;
   pairDictionary = NULL;
}
