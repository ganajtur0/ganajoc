s=[sum([int(z)for z in y.split('\n')])for y in[x for x in open("i","r").read().strip().split("\n\n")]]
s.sort(reverse=True)
print(s[0],sum(s[:3]))
