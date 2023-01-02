#include <iostream>
using namespace std;

// Node class
class Node
{
    // min is the low of interval
    // max is the high of interval
    // is the maximum high between node's subtree
    int min, max, m;
public:
    // left is the left child of any node
    // right is the right child of any node
    // parent is the parent of any node
    Node *left, *right, *parent;

    // Parameterized constructor
    Node(int min = NULL, int max = NULL)
    {
        // Set values
        this->min = min;
        this->max = max;
        this->m = max;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    // return lower value of interval
    int getMin() const { return min; }

    // return higher value of interval
    int getMax() const { return max; }

    // return maximum higher in node's subtree
    int getM() const{ return m; }

    // set lower
    void setMin(int x) { min = x; }

    // set higher
    void setMax(int x) { max = x;}

    // set maximum higher
    void setM(int maximum) { m = maximum; }
};

// merge left and right arrays which contain overlaps intervals from left and right subtrees in one array
void mergeIntervals(Node intervals[], Node *left, Node *right, int size)
{
    int counter = 0;
    // put left array in intervals array
    for (int i = 0; i < size; ++i, ++counter)
    {
        // if node is empty
        if (left[i].getMin() == NULL) break;

            // else add this node to intervals array
        else intervals[counter] = left[i];
    }

    // put right array in intervals array
    for (int i = 0; i < size; ++i, ++counter)
    {
        // if node is empty
        if (right[i].getMin() == NULL) break;

            // else add this node to intervals array
        else intervals[counter] = right[i];
    }
}

class tree
{
    // root of the tree
    Node *root;

    // display function prints all nodes in my tree using in-order traverse
    void display(Node *node)
    {
        // if node is empty then return
        if (node == nullptr) return;
        else
        {
            // go to left child
            display(node->left);
            // print
            cout << node->getMin() << " " << node->getMax() << " " << node->getM() << endl;
            // go to right child
            display(node->right);
        }
    }


    void search(Node *node, int min, int max, int size, Node intervals[])
    {
        // if node is not empty
        if (node != nullptr)
        {
            // left array contains intervals that overlap with query in left subtree
            Node left[size];
            // right array contains intervals that overlap with query in right subtree
            Node right[size];

            // search in left subtree
            search(node->left, min, max, size, left);

            // search in right subtree
            search(node->right, min, max, size, right);

            // merge left and right arrays that we fill when searching for an overlapping interval
            mergeIntervals(intervals, left, right, size);

            // if interval overlaps with current node
            if (node->getMin() <= max && node->getMax() >= min)
            {
                for (int i = 0; i < size; ++i)
                {
                    // if node is empty
                    if (intervals[i].getMin() == NULL)
                    {
                        // add node contains interval that overlap to intervals array
                        intervals[i] = *node;

                        // delete this node
                        this->deletionCall(node->getMin(), node->getMax());
                        break;
                    }
                }
            }
        }
    }

    // delete node from the tree
    Node *deletion(Node *r, int min, int max)
    {

        Node *temp;
        // if the entered interval is not exist
        if (r == nullptr) return nullptr;
            // if the entered interval's lower < current node's lower
        else if (min < r->getMin()) r->left = deletion(r->left, min, max);

            // if the entered interval's lower > current node's lower
        else if (min > r->getMin() || (min == r->getMin() && max != r->getMax())) r->right = deletion(r->right, min, max);

            // if the entered interval founded in the current node
        else if (min == r->getMin() && max == r->getMax())
        {
            // if current node has not any children ( case 0 )
            if (r->left == nullptr && r->right == nullptr)
            {
                // if node is leaf and it is not the root
                if (r != root)
                {
                    temp = r->parent;
                    if (temp->right == r) r->parent->right = nullptr;
                    else if (temp->left == r) r->parent->left = nullptr;
                    r = nullptr;
                    resetTree(temp);
                }

                    // if node to be deleted is the root
                else
                {
                    root = nullptr;
                    return r;
                }
            }
                // if node to be deleted has no right child
            else if (r->right == nullptr)
            {
                temp = r->left;
                // clone the left child data to current node
                r->setMax(temp->getMax());
                r->setMin(temp->getMin());
                r->setM(temp->getM());
                r->left = temp->left;
                r->right = temp->right;
                delete temp;
                resetTree(r);
            }
                // if node to be deleted has no left child
            else if (r->left == nullptr)
            {
                temp = r->right;
                // clone the right child data to current node
                r->setMax(temp->getMax());
                r->setMin(temp->getMin());
                r->setM(temp->getM());
                r->right = temp->right;
                r->left = temp->left;
                delete temp;
                resetTree(r);
            }
                // if node has two children, then get successor and delete it
            else
            {
                Node *Successor = findMin(r->right);
                r->setMax(Successor->getMax());
                r->setMin(Successor->getMin());
                r->right = deletion(r->right, Successor->getMin(), Successor->getMax());
            }
        }
        return r;
    }

    // return minimum node's low in the subtree rooted by r
    Node *findMin(Node *r)
    {
        if (r == nullptr) return nullptr;
        else if (r->left == nullptr) return r;
        else return findMin(r->left);
    }

    // return subtree size rooted by r
    int getSize(Node *r)
    {
        int size = 1;
        if (r == nullptr) return 0;
        else return size + getSize(r->left) + getSize(r->right);
    }

    // update maximum high for a specific node
    static void resetParent(Node *node)
    {
        // if node is leaf
        if (node->left == nullptr && node->right == nullptr)
            node->setM(node->getMax());

            // if node has not left child
        else if (node->left == nullptr)
        {
            if (node->getMax() > node->right->getM())
                node->setM(node->getMax());
            else
                node->setM(node->right->getM());
        }
            // if node has not right child
        else if (node->right == nullptr)
        {
            if (node->getMax() > node->left->getM())
                node->setM(node->getMax());
            else
                node->setM(node->left->getM());
        }
            // if node has both children
        else
        {
            if (node->getMax() >= node->left->getM() && node->getMax() >= node->right->getM())
                node->setM(node->getMax());
            else if (node->left->getM() >= node->getMax() && node->left->getM() >= node->right->getM())
                node->setM(node->left->getM());
            else
                node->setM(node->right->getM());
        }
    }

    // traverse tree from deleted node to the root and update maximum higher in each node on the path
    void resetTree(Node *temp)
    {
        while (true)
        {
            resetParent(temp);
            if (temp == root)
                return;
            temp = temp->parent;
        }
    };
public:
    // parameterized constructor
    tree(int min, int max)
    {
        root = new Node(min, max);
    }

    // insert interval
    void insert(int min, int max)
    {
        Node *x = new Node(min, max);
        Node *temp = root;

        // find right place to insert interval
        while (temp != nullptr)
        {
            if (x->getM() > temp->getM())
                temp->setM(x->getM());
            // check if interval lower is less than current node's lower
            if (x->getMin() < temp->getMin())
            {
                if (temp->left == nullptr)
                {
                    temp->left = x;
                    x->parent = temp;
                    return;
                }
                temp = temp->left;
            }
                // interval node's lower is more than or equal to current node's lower
            else
            {
                if (temp->right == nullptr)
                {
                    temp->right = x;
                    x->parent = temp;
                    return;
                }
                temp = temp->right;
            }
        }
    }

    // find function calls search functions only
    void find(int min, int max, Node intervals[])
    {
        search(root, min, max, this->size(), intervals);
    }

    // print functions calls display function only
    void print()
    {
        display(root);
    }

    // deletionCall calls display function only
    Node *deletionCall(int min, int max)
    {
        if (root == nullptr)
            return nullptr;
        return deletion(root, min, max);
    }

    // size function calls getSize function only
    int size()
    {
        return getSize(root);
    }

    // print all overlaps intervals
    void printInterval(Node overLab[], int size, int low, int high)
    {
        if (overLab[0].getMin() != NULL) {
            cout << "The node that`s overlap with interval " << low <<  " to " << high << " are :\n";
        } else
            cout << "No node overlap with the interval 0 to 3\n";
        for (int i = 0; i < size; ++i)
        {
            if (overLab[i].getMin() == NULL)
                break;
            cout << overLab[i].getMin() << " " << overLab[i].getMax() << " " << overLab[i].getM() << endl;
            this->insert(overLab[i].getMin(), overLab[i].getMax());
        }
        cout << "-------------------------------------------" << endl;

    }
};

// clear array
void clear(Node overlap[], int size)
{
    Node newNode;
    for (int i = 0; i < size; ++i)
    {
        overlap[i] = newNode;
    }
}
int main() {

    tree tree(17, 19);
    tree.insert(5, 11);
    tree.insert(22, 23);
    tree.insert(4, 8);
    tree.insert(15, 18);
    tree.insert(7, 10);
    tree.insert(16, 17);
    tree.insert(20, 30);
    tree.insert(6, 8);
    tree.insert(61, 800);

    tree.print();
    cout << "-------------------------------------------" << endl;

    int size = tree.size();
    Node overlab[size];
    tree.find(10, 18, overlab);
    tree.printInterval(overlab, size, 10, 18);
    clear(overlab, size);

    tree.find(0, 0, overlab);
    tree.printInterval(overlab, size, 0, 0);
    clear(overlab, size);

    tree.find(61, 800, overlab);
    tree.printInterval(overlab, size, 61, 800);
    clear(overlab, size);

    tree.find(10, 10, overlab);
    tree.printInterval(overlab, size, 10, 10);
    clear(overlab, size);

    tree.find(4, 17, overlab);
    tree.printInterval(overlab, size, 4, 17);
    clear(overlab, size);

    tree.find(3, 14, overlab);
    tree.printInterval(overlab, size, 3, 14);
    clear(overlab, size);

    tree.find(-2, 0, overlab);
    tree.printInterval(overlab, size, -2, 0);
    clear(overlab, size);

    tree.find(4, 4, overlab);
    tree.printInterval(overlab, size, 4, 4);
    clear(overlab, size);

    tree.find(1, 18, overlab);
    tree.printInterval(overlab, size, 1, 18);
    clear(overlab, size);

    tree.find(23, 50, overlab);
    tree.printInterval(overlab, size, 23, 50);
    clear(overlab, size);

    tree.deletionCall(5, 11);
    tree.deletionCall(16, 17);
    tree.deletionCall(17, 18);
    tree.deletionCall(23, 50);
    tree.deletionCall(7, 10);
    tree.deletionCall(22, 23);
    tree.deletionCall(61, 800);
    tree.deletionCall(4, 8);
    tree.deletionCall(20, 30);
    tree.deletionCall(4, 4);
    cout<<"the tree after deletion\n";
    tree.print();
}