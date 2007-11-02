from pprint import pprint
from timeit import Timer


def _printf(*args):
	print args[0] % args[1:]

class _Slot(object):

	__slots__ = ("key", "key_hash", "value", "next")

	def __init__(self, key, key_hash, value, next):
		self.key = key
		self.key_hash = key_hash
		self.value = value
		self.next = next

	def __str__(self):
		return "%s._Slot(%r, %r)" % (__name__, self.key, self.value)
	
	__repr__ = __str__

class _SlotTable(list):

	primes = (
		11, 19, 37, 73, 109, 163, 251, 367, 557, 823, 1237,
        	1861, 2777, 4177, 6247, 9371, 14057, 21089, 31627,
        	47431, 71143, 106721, 160073, 240101, 360163,
        	540217, 810343, 1215497, 1823231, 2734867, 4102283,
        	6153409, 9230113, 13845163
	)
	
	@staticmethod
	def next_prime(x):
		for i in _SlotTable.primes:
			if i > x:
				return i
		#FIXME
		return _SlotTable.primes[len(_SlotTable.primes) - 1]

	def __init__(self):
		self.extend([None] * 8)
		#self.grow()

	def grow(self):
		#grow_by = _SlotTable.next_prime(len(self)) - len(self)
		self.extend([None] * len(self) * 2)
		_printf("grow(%d)", len(self))

	def __setitem__(self, k, v):
		super(_SlotTable, self).__setitem__(k, v)		

class Hashtable(object):

	__slots__ = ("_table", "_in_use")

	def __init__(self):
		self._table = _SlotTable()
		self._in_use = 0
	
	def __setitem__(self, key, value):
		hashcode = hash(key)
		index = hashcode % len(self._table)	
		#_printf("hashcode = %d", hashcode)
		s = self._table[index]
		while s is not None:
			#if s.key_hash == hashcode:
			#	s.value = value
			#	return
			if s.key == key:
				s.value = value
				return
			s = s.next
		s = _Slot(key, hashcode, value, self._table[index])
		self._table[index] = s
		self._in_use += 1	
		self._resize()

	def _resize(self):
		if len(self._table) <= self._in_use:
			_printf("_resize(%d, %d)", len(self._table), self._in_use)
			self._table.grow()		
			self._rehash()

	def _rehash(self):
		size = len(self._table)
		slot, next = None, None
		for i in xrange(size):
			slot = self._table[i]
			while slot is not None:
				next = slot.next
				index = hash(slot.key) % size
				slot.next = self._table[index]
				self._table[index] = slot
				print slot
				slot = next

	def __getitem__(self, key):
		hashcode = hash(key)
		index = hashcode % len(self._table)
		s = self._table[index]
		while s is not None:
			#if s.key_hash == hashcode:
			#	return s.value
			if s.key == key:	
				return s.value
			s = s.next
		raise KeyError
	
	def __contains__(self, key):
		try:
			self.__getitem__(key)
			return True
		except KeyError:
			return False

	has_key = __contains__

	def __len__(self):
		return self._in_use

	def iterkeys(self):
		for slot in self._table:
			s = slot
			while s is not None:
				yield s.key
				s = s.next
	def keys():
		return [key for key in self.iterkeys()]	

	def itervalues(self):
		for slot in self._table:
			s = slot
			while s is not None:
				yield s.value
				s = s.next
	def values():
		return [value for value in self.itervalues()]

	def iteritems(self):
		for slot in self._table:
			s = slot
			while s is not None:
				yield (s.key, s.value)
				s = s.next
	def items():
		return [item for item in self.iteritems()]

def _test1():
	s = Hashtable()
	for i in range(10000):
		s[str(i)] = i
	for i in range(10000):
		assert s[str(i)] == i
			
def _test2():
	correct_res = set([i for i in range(200)])
	res = set()
	d = Hashtable()
	for i in range(200):
		d[str(i)] = i
	for k in d.iterkeys():
		res.add(d[k])
	assert correct_res == res

if __name__ == "__main__":
	_test2()
	#t = Timer("_test1()", "from __main__ import _test1")
	#print t.timeit(5)


