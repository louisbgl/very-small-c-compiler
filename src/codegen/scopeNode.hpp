#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

enum class Type {
    Int,
};

struct VarInfo {
    std::string name;
    Type type;
    int offset;
    int size;
};

class ScopeNode {
public:
    explicit ScopeNode(ScopeNode* parent = nullptr);

    ScopeNode* pushChild();
    ScopeNode* getParent() const;

    void addVariable(const std::string& name, Type type, int size);

    std::optional<int> getOffset(const std::string& name) const;
    std::optional<Type> getType(const std::string& name) const;

    std::optional<int> getOffsetRecursive(const std::string& name) const;
    std::optional<Type> getTypeRecursive(const std::string& name) const;

    int getFrameSize() const;
    ScopeNode& getChild(int index);

    void dump(int depth = 0) const;

private:
    ScopeNode* parent;
    std::vector<std::unique_ptr<ScopeNode>> children;
    std::vector<VarInfo> variables;

    int currentOffset = 0;
};
