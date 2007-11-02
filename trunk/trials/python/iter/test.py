import py_iter

class TestIter(list):
	def __init__(self):
		self._cur = 0
	def __next__(self):
		if self._cur == len(self):
			return None
		else:
			v = self[self._cur]
			self._cur += 1
			return v
	

if __name__ == "__main__":
	obj = TestIter()
	for i in range(3):
		obj.append(i)
	x = py_iter.iter_me(obj.__next__)
	for i in x:
		print i
	
