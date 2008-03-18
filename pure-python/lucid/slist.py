# Simple slingly-linked list implementation

__all__ = ("prepend", "remove")

class Node(object):
    __slots__ = ("data", "next")


def prepend(node, data):
    new_node = Node()
    new_node.data = data
    new_node.next = node
    return new_node

def remove(node, data):
    tmp, prev = node, None
    while tmp is not None:
        if tmp.data == data:
            if prev is not None:
                prev.next = tmp.next
            else:
                node = tmp.next
            #del tmp
            break
        prev = tmp
        tmp = prev.next
    return node
    
