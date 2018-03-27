#include <stdio.h>
#include <stdlib.h>

struct tree {
    int key;
    tree *lr[2];
    //tree *left;
    //tree *right;
    unsigned int height;
};

unsigned int height (tree *avl) {
    if (avl!=NULL)
        return avl->height;
    else
        return 0;
}

tree * addNode (tree* avl, int elem) {
    avl = (tree *) malloc (sizeof(tree));
    avl->key = elem;
    avl->lr[0] = NULL;
    avl->lr[1] = NULL;
    avl->height = 1;
    return avl;
}

void setHeight (tree *avl) {
    unsigned int leftHeight = height (avl->lr[0]);
    unsigned int rightHeight = height (avl->lr[1]);
    if (leftHeight > rightHeight)
        avl->height = leftHeight + 1;
    else
        avl->height = rightHeight + 1;
}

int balanceNumber (tree *avl) {
    return height (avl->lr[1]) - height (avl->lr[0]);
}

int avlCheck (tree *avl) {
    if (avl == NULL) return 0;
    if (balanceNumber (avl) <= 1 && balanceNumber(avl) >= -1) {
        return 0 + avlCheck(avl->lr[0]) + avlCheck(avl->lr[1]);
    }
    else return 1;
}
tree *smallRotation (tree *avl, int where) {
    tree *b = avl->lr[1-where];
    avl->lr[1-where] = b->lr[where];
    b->lr[where] = avl;
    setHeight(avl);
    setHeight(b);
    return b;
}
/*tree *smallLeftRotation (tree *avl) {
    tree *b = avl->right;
    avl->right = b->left;
    b->left = avl;
    setHeight(avl);
    setHeight(b);
    return b;
}*/

/*tree *smallRightRotation (tree *avl) {
    tree *b = avl->left;
    avl->left = b->right;
    b->right = avl;
    setHeight(avl);
    setHeight(b);
    return b;
}*/

tree *balance (tree *avl) {
    setHeight (avl);
    if (balanceNumber(avl) == 2) {
        if (balanceNumber(avl->lr[1]) < 0) {
            avl->lr[1] = smallRotation(avl->lr[1], 1);
        }
        return smallRotation(avl, 0);
    }
    if (balanceNumber(avl) == -2) {
        if (balanceNumber(avl->lr[0]) > 0) {
            avl->lr[0] = smallRotation(avl->lr[0], 0);
        }
        return smallRotation(avl, 1);
    }
    else
        return avl;
}
tree *addElement (tree *avl, int elem) {
    if (avl == NULL) {
        avl = addNode(avl, elem);
    }
    else {
        if (elem < avl->key)
            avl->lr[0] = addElement(avl->lr[0], elem);
        else
            avl->lr[1] = addElement(avl->lr[1], elem);
    }
    return balance (avl);
}

int main() {
    int n, elem;
    int i;
    struct tree *avl = NULL;
    scanf ("%d", &n);
    for (i = 0; i < n; i++){
        scanf ("%d", &elem);
        avl = addElement(avl, elem);
    }
    printf ("%d %d", avlCheck(avl), height(avl));
    return 0;
}