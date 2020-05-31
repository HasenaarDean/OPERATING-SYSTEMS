//
// Created by imri on 5/22/2019.
//

#include "util.h"

uint64_t getPage(uint64_t address) {
    return address >> OFFSET_WIDTH;
}

typedef struct {
    word_t parent;
    uint64_t parentIndex;
    uint64_t page;
    word_t content;
    uint64_t address;
} Node;


void populateChildren(Node parent, Node *children, uint64_t virtualPage) {
    for (uint64_t i = 0; i < PAGE_SIZE; i++) {
        children[i].parent = parent.content;
        children[i].parentIndex = i;
        children[i].address = parent.content * PAGE_SIZE + i;
        children[i].page = (virtualPage << OFFSET_WIDTH) + i;
        PMread(children[i].address, &children[i].content);
    }
}

void populateChildren(Node parent, Node *children) {
    populateChildren(parent, children, 0);
}

uint64_t getOffset(uint64_t address) {
    uint64_t offsetMask = 0;
    for (int i = 0; i < OFFSET_WIDTH; i++) {
        offsetMask += 1LL << i;
    }
    return address & offsetMask;
}

uint64_t getAddress(uint64_t address, uint64_t layer) {
    uint64_t res = address >> (TABLES_DEPTH - layer) * OFFSET_WIDTH;
    return getOffset(res);
}

Node maxFrame(Node x, Node y) {
    return x.content > y.content ? x : y;
}

Node unusedFrameHelper(Node node, Node maxNode, uint64_t layer) {
    if (layer == TABLES_DEPTH || maxNode.content == NUM_FRAMES) {
        return maxNode;
    } else {
        Node children[PAGE_SIZE];
        populateChildren(node, children);
        for (int i = 0; i < PAGE_SIZE; i++) {
            if (children[i].content != 0) {
                children[i] = unusedFrameHelper(children[i], maxFrame(children[i], maxNode), layer + 1);
            } else {
                children[i] = maxNode;
            }
        }
        word_t childrenContent[PAGE_SIZE];
        for (int i = 0; i < PAGE_SIZE; i++) {
            childrenContent[i] = children[i].content;
        }
        word_t *max_child = std::max_element(childrenContent, childrenContent + PAGE_SIZE);
        for (auto &i : children) {
            if (i.content == *max_child) return i;
        }
        assert(false);
    }
}

Node unusedFrame() {
    Node root = {0, 0, 0, 0, 0};
    Node result = unusedFrameHelper(root, root, 0);
    result.content += 1;
    return result.content == NUM_FRAMES ? root : result;
}

uint64_t distance(uint64_t x, uint64_t y) {
    return x > y ? x - y : y - x;
}

typedef struct {
    Node node;
    uint64_t distance;
} CyclicDistanceData;

CyclicDistanceData
cyclicDistance(Node node, uint64_t layer, uint64_t currentPage, uint64_t targetPage) {
    if (layer == TABLES_DEPTH) {
        uint64_t distanceFromCurrentToTarget = distance(targetPage, currentPage);
        return CyclicDistanceData{
                node,
                std::min<uint64_t>(NUM_PAGES - distanceFromCurrentToTarget, distanceFromCurrentToTarget)
        };
    } else {
        Node children[PAGE_SIZE];
        uint64_t childrenValues[PAGE_SIZE];
        populateChildren(node, children, currentPage);
        for (word_t i = 0; i < PAGE_SIZE; i++) {
            if (children[i].content != 0) {
                CyclicDistanceData res = cyclicDistance(children[i], layer + 1, children[i].page, targetPage);
                childrenValues[i] = res.distance;
                children[i] = res.node;
            } else {
                childrenValues[i] = 0;
            }
        }
        uint64_t *maxChildPtr = std::max_element(childrenValues, childrenValues + PAGE_SIZE);
        Node maxChild = children[maxChildPtr - childrenValues];
        return CyclicDistanceData{maxChild, *maxChildPtr};
    }
}

void evictPage(Node node, word_t layer, word_t toEvict, uint64_t virtualPage) {
    if (layer == TABLES_DEPTH) return;
    Node children[PAGE_SIZE];
    populateChildren(node, children, virtualPage);
    for (uint64_t i = 0; i < PAGE_SIZE; i++) {
        if (toEvict && children[i].content == toEvict) {
            assert(layer == TABLES_DEPTH - 1);
            PMevict(toEvict, children[i].page);
            PRINT("freed VA %lu\n", children[i].address);
            PMwrite(children[i].address, 0);
            return;
        } else if (children[i].content != 0) {
            evictPage(children[i], layer + 1, toEvict, children[i].page);
        }
    }
}

Node evictedFrame(uint64_t targetPage) {
    Node root = {0, 0, 0, 0, 0};
    CyclicDistanceData result = cyclicDistance(root, 0, 0, targetPage);
    evictPage(root, 0, result.node.content, 0);
    return result.node;
}

Node emptyTableHelper(Node node, word_t layer, uint64_t virtualAddress, word_t currentFrame) {
    Node root = {0, 0, 0, 0, 0};
    if (layer == TABLES_DEPTH) {
        return root;
    }

    Node children[PAGE_SIZE];
    populateChildren(node, children);
    bool emptyTable = true;
    for (word_t i = 0; i < PAGE_SIZE; i++) {
        if (children[i].content != 0) {
            emptyTable = false;
            auto child = emptyTableHelper(children[i], layer + 1, children[i].address, currentFrame);
            if (child.content) return child;
        }
    }

    if (emptyTable && node.content != currentFrame && node.content != 0) {
        PRINT("freed VA %lu\n", virtualAddress);
        PMwrite(virtualAddress, 0);
        return node;
    } else {
        return root;
    }
}

Node emptyTable(word_t currentFrame) {
    Node root = {0, 0, 0, 0, 0};
    return emptyTableHelper(root, 0, 0, currentFrame);
}

word_t findFrame(uint64_t targetPage, word_t currentFrame) {
    Node next = {0, 0, 0, 0, 0};
    if (!next.content) next = emptyTable(currentFrame);
    if (next.content) PRINT("empty table: %d\n", next.content);
    if (!next.content) {
        next = unusedFrame();
        if (next.content) PRINT("unused frame: %d\n", next.content);
    }
    if (!next.content) {
        next = evictedFrame(targetPage);
        PRINT("evicted: %d\n", next.content);
    }
    assert(next.content != 0);
    assert(next.content < NUM_FRAMES);
    return next.content;
}

uint64_t translateAddress(uint64_t virtualAddress) {
    uint64_t tableIndex = 0;
    uint64_t currentLayer = 0;
    word_t nextFrame = 0;
    uint64_t virtualPage = getPage(virtualAddress);
    while (currentLayer < TABLES_DEPTH) {
        tableIndex = getAddress(virtualAddress, currentLayer);
        uint64_t nextAddress = nextFrame * PAGE_SIZE + tableIndex;
        word_t currentFrame = nextFrame;
        PMread(nextAddress, &nextFrame);
        if (nextFrame == 0) {
            nextFrame = findFrame(virtualPage, currentFrame);
            PRINT("writing frame to VA %lu\n", nextAddress);
            PMwrite(nextAddress, nextFrame);
            clearTable(nextFrame);
            if (currentLayer + 1 == TABLES_DEPTH) {
                PMrestore(nextFrame, virtualPage);
            }
        }
        currentLayer++;
    }
    uint64_t offset = getOffset(virtualAddress);
    PRINT_2("translated V:%lu to P:%lu\n", virtualAddress, nextFrame * PAGE_SIZE + offset);
    return nextFrame * PAGE_SIZE + offset;
}

void clearTable(uint64_t frameIndex) {
    for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}