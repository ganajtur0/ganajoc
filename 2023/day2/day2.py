i = [l.split(': ')[1].strip() for l in open('i', 'r').readlines()]
sum = 0
for j,l in enumerate(i):
    good = True
    for p in l.split(';'):
        for balls in p.split(','):
            n = [int(c) for c in balls.split() if c.isdigit()][0]
            if "red" in balls and n > 12:
                good = False   
            elif "green" in balls and n > 13:
                good = False
            elif "blue" in balls and n > 14:
                good = False
    if good:
        sum+=(j+1)

print(sum)

sum = 0
for j,l in enumerate(i):
    nums = {"r" : [], "g" : [], "b" : []}
    for p in l.split(';'):
        for balls in p.split(','):
            n = [int(c) for c in balls.split() if c.isdigit()][0]
            if "red" in balls:
                nums["r"].append(n)
            elif "green" in balls:
                nums["g"].append(n)
            elif "blue" in balls:
                nums["b"].append(n)
    sum+=(max(nums["r"]) * max(nums["g"]) * max(nums["b"]))

print(sum)
