import math

file = "i"
#file = "test"

t = int(''.join(open(file).readlines()[0].split()[1:]))
d = int(''.join(open(file).readlines()[1].split()[1:]))

x1 = ( (-1*t + math.sqrt(t**2 - 4*d))/(-2) )
x2 = ( (-1*t - math.sqrt(t**2 - 4*d))/(-2) )

print(int(abs(x2-x1)))
