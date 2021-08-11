import collections
import pprint
import math
# file_input = input('q3.txt')
entropy = 0
with open('inputfile_problem3_28.txt', 'r') as info:
  count = collections.Counter(info.read().upper())
  value = pprint.pformat(count)
print(value)

prob = [520, 282, 112, 85, 1]
print(sum(prob))
# d = dict(value)
for val in prob:
	val = int(val)/1000
	entropy -= val * math.log(val, 2)

print(entropy)