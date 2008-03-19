
cdef class Node:
    cdef Node _next
    cdef public object data

    property next:
        def __get__(self):
            return self._next


def prepend(Node node, data):
    cdef Node new_node 
    new_node = Node()
    new_node.data = data
    new_node._next = node
    return new_node

def remove(Node node, data):
    cdef Node tmp, prev
    prev = None
    tmp = node
    while tmp is not None:
        if tmp.data == data:
            if prev is not None:
                prev._next = tmp._next
            else:
                node = tmp._next
            #del tmp
            break
        prev = tmp
        tmp = prev._next
    return node
    
