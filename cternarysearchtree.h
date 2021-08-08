#ifndef CTERNARYSEARCHTREE_H
#define CTERNARYSEARCHTREE_H

#include <QStringList>
#include "ternarysearchtreenode.h"
#include "locatedatastruct.h"

/**
 * @brief The CTernarySearchTree class : Implementation of Ternary Search Tree data structure
 * @author Anurag
 */
class CTernarySearchTree
{

public:
    CTernarySearchTree();
    ~CTernarySearchTree();

    void insert(const QString word, const QString uiq, const QString desc);
    void deleteWord(const QString &word, const QString &uiq);
    QList<LocateDataStruct>* search(QString word, int autoCompleteListLimit  = defaultAutoCompleteListLimit) const;
    QList<LocateDataStruct>* listEntireDictionary(int autoCompleteListlimit = defaultAutoCompleteListLimit) const;
    void AddPairDictionary(CTernarySearchTree * pairDictionary);
    void clearDictionary();

private:
    TernarySearchTreeNode *root;
    CTernarySearchTree *pairDictionary;
    static const int defaultAutoCompleteListLimit = 20;

    void insert(TernarySearchTreeNode **node, const char *word, const char *uiq, const char *desc);
    TernarySearchTreeNode* search(TernarySearchTreeNode *node, const char *word) const;
    QList<LocateDataStruct> *traverseTree(TernarySearchTreeNode *node, QString phrase, int autoCompleteListLimit) const;
    void traverseTree(TernarySearchTreeNode *node, QList<LocateDataStruct> &autoCompleteList,
                      QString str, int autoCompleteListLimit) const;
    void deleteNode(TernarySearchTreeNode **node);
    bool deleteWord(TernarySearchTreeNode **node, const char *word, const QString &uiq);
    bool isFreeNode(TernarySearchTreeNode *node) const;
    bool isEndOfWord(TernarySearchTreeNode *node) const;
    void appendNodeData(QList<LocateDataStruct> &list, TernarySearchTreeNode* node, QString str);
    void searchInPairDictionary(QString word, QList<LocateDataStruct>* parentAutoCompleteList, int autoCompleteListLimit) const;
};

#endif // CTERNARYSEARCHTREE_H
