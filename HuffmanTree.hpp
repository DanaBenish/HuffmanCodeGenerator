#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "HuffmanBase.hpp"
#include "HeapQueue.hpp"
#include <iostream>
#include <map>
#include <stack>

class HuffmanTree : HuffmanTreeBase {

    private:
        size_t n;
        HuffmanNode* root;

    public:
        std::string compress(const std::string inputStr) override;
        std::string serializeTree() const override;
        std::string decompress(const std::string inputCode, const std::string serializedTree) override;

    protected:
        void findCode(const HuffmanNode* node, std::map<char, std::string>& codeMap, std::string code);
        void serialize(const HuffmanNode* node, std::string& outString) const;
};

#endif