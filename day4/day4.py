def p(x):
    return ([int(v)for v in x[0].split('-')],[int(v)for v in x[1].split('-')])
def c(x,y):
    return x[0]<=y[0]and x[1]>=y[1]
def o(x,y):
    return min(x[-1],y[-1])-max(x[0],y[0])>=0
print(sum([1 if c(x,y)or c(y,x) else 0 for(x,y) in[p(line.strip().split(','))for line in open("i", "r").readlines()]]))
print(sum([1 if o(x,y) else 0 for(x,y) in[p(line.strip().split(','))for line in open("i", "r").readlines()]]))
