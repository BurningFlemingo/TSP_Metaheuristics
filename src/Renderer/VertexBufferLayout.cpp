#include "VertexBufferLayout.h"

void addVertexBufferElement(VertexBufferLayout& layout, uint type, uint count, bool normalized) {
    VertexBufferElement element;
    layout.stride += count * VertexBufferElement::getSizeOfType(type);
    element.count = count;
    element.type = type;
    element.normalized = normalized;
    layout.elements.emplace_back(element);
}
