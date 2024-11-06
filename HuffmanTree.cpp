#include "HuffmanTree.hpp"
#include <string>

// Compress the input string
std::string HuffmanTree::compress(const std::string inputStr) {
    std::map<char, size_t> charMap;
    std::map<char, size_t>::iterator i;

    size_t count[256] = {0};

    // Read characters
    for (int i = 0; inputStr[i] != '\0'; i++) {
        count[inputStr[i]]++; // Count frequency
    }

    for(int i = 0; i < 256; i++) {
        if(count[i] != 0){
            charMap.insert(std::pair<char,size_t>((char)i, count[i])); // Store in array               
        }
    }

    // Create Heap Queue
    HeapQueue<HuffmanNode*, HuffmanNode::Compare> queue;

    for (i = charMap.begin(); i != charMap.end(); ++i) {
        HuffmanNode* node = new HuffmanNode(i->first, i->second);
        queue.insert(node);
        n++;
    }

    // Build the Huffman Tree
    while (queue.size() != 1){

        HuffmanNode* leftnode = queue.min();
        queue.removeMin();

        HuffmanNode* rightnode = queue.min();
        queue.removeMin();

        size_t totalfreq = (leftnode->getFrequency() + rightnode->getFrequency());
        HuffmanNode* parent = new HuffmanNode('\0', totalfreq, nullptr, leftnode, rightnode);

        leftnode->parent = parent;
        rightnode->parent = parent;
        queue.insert(parent);
    }

    root = queue.min();

    // Encode characters of the string using map
    std::map<char, std::string> codeMap;
    std::string code = "";
    std::string output = "";

    for (i = charMap.begin(); i != charMap.end(); ++i) {
        findCode(queue.min(), codeMap, code);
    }
    
    for (auto i : inputStr) {
        output += codeMap[i];
    }

   return output;
}

// Serialize the tree - using serialize helper function
std::string HuffmanTree::serializeTree() const {

    // Make sure tree is not empty
    if (root == nullptr) {
        return "";
    }

    std::string output = "";
    serialize(root, output);
    return output;
}

// Decompress to the original string
std::string HuffmanTree::decompress(const std::string input, const std::string serializedT) {

    std::string output = "";
    std::stack<HuffmanNode*> tstack;
   
    // Using serialized tree to restruct 
    for(std::string::const_iterator i = serializedT.begin(); i != serializedT.end(); i++){

        if(*i == 'L'){
            i++;
            HuffmanNode *n = new HuffmanNode(*i, 0);
            tstack.push(n);
        } 
        
        else {
            HuffmanNode* right = tstack.top();
            tstack.pop();
            HuffmanNode* left = tstack.top();
            tstack.pop();
            HuffmanNode* branch = new HuffmanNode('\0', 0, nullptr, left, right);
            tstack.push(branch);
        }
    }

    // Copy the remains of stack to the root
    HuffmanNode* root = tstack.top();
    tstack.pop();

    // Reconstruct text
    HuffmanNode* curr = root;
    for (auto character : input) {
        if (character == '0') curr = curr->left;
        else curr = curr->right;

        // When a leaf is reached add a character to the output and restart
        if (curr->isLeaf()) {
            output += curr->getCharacter();
            curr = root;
        }
    }   
    return output;
}

// Recursively builds a map of Huffman codes
void HuffmanTree::findCode(const HuffmanNode* node, std::map<char, std::string>& codeMap, std::string c){

    // Make sure tree is not empty
    if (node == nullptr) {
        return;
    }

    if(node->isLeaf()){
        codeMap[node->getCharacter()] = c; 
    }

    findCode(node->left, codeMap, c + "0");
    findCode(node->right, codeMap, c + "1");
}

// Helper function that recursively serializes in postorder traversal
void HuffmanTree::serialize(const HuffmanNode* node, std::string& Ostring) const {

    // Make sure tree is not empty
    if (node == nullptr){
        return;
    }
            
    serialize(node->left, Ostring);
    serialize(node->right, Ostring);

    // If node is a leaf
    if (node->isLeaf()) {
        Ostring = Ostring + "L" + node->getCharacter();
    }

    // If node is a branch
    if (node->isBranch()) {
        Ostring += "B";
    }
}