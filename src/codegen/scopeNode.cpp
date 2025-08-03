#include <iostream>
#include <iomanip>
#include "scopeNode.hpp"

ScopeNode::ScopeNode(ScopeNode* parent)
    : parent(parent), currentOffset(0) {
        if (parent) {
            currentOffset = parent->getFrameSize();
        }
    }

ScopeNode* ScopeNode::pushChild() {
    children.push_back(std::make_unique<ScopeNode>(this));
    return children.back().get();
}

ScopeNode* ScopeNode::getParent() const {
    return parent;
}

void ScopeNode::addVariable(const std::string& name, Type type, int size) {
    if (getOffset(name)) {
        throw std::runtime_error("Variable " + name + " already exists in this scope");
    }
    currentOffset += size;  // Increment before storing
    variables.push_back({name, type, currentOffset, size});
}

std::optional<int> ScopeNode::getOffset(const std::string& name) const {
    for (const auto& var : variables) {
        if (var.name == name) {
            return var.offset;
        }
    }
    return std::nullopt;
}

std::optional<Type> ScopeNode::getType(const std::string& name) const {
    for (const auto& var : variables) {
        if (var.name == name) {
            return var.type;
        }
    }
    return std::nullopt;
}

std::optional<int> ScopeNode::getOffsetRecursive(const std::string& name) const {
    if (auto offset = getOffset(name)) {
        return offset;
    }
    if (parent) {
        return parent->getOffsetRecursive(name);
    }
    return std::nullopt;
}

std::optional<Type> ScopeNode::getTypeRecursive(const std::string& name) const {
    if (auto type = getType(name)) {
        return type;
    }
    if (parent) {
        return parent->getTypeRecursive(name);
    }
    return std::nullopt;
}

int ScopeNode::getFrameSize() const {
    return currentOffset;
}

bool ScopeNode::hasChildren() const {
    return !children.empty();
}

ScopeNode& ScopeNode::getChild(int index) {
    if (index < 0 || index >= static_cast<int>(children.size())) {
        throw std::out_of_range("[ScopeNode::getChild] Child index out of range (" + std::to_string(index) + ")");
    }
    return *children[index];
}


void ScopeNode::dump(int depth) const {
    std::string indent(depth * 2, ' ');  // Two spaces per level of depth

    std::cout << indent << "Scope (depth=" << depth << ", frameSize=" << currentOffset << "):\n";

    for (const auto& var : variables) {
        std::cout << indent << "  - " << var.name
                  << " : type=" << static_cast<int>(var.type)
                  << ", offset=" << var.offset
                  << ", size=" << var.size << '\n';
    }

    for (const auto& child : children) {
        child->dump(depth + 1);
    }
}