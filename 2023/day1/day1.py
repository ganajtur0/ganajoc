x = open('i','r').readlines()
sum = 0
for r in x:
    y = []
    for c in r:
        try:
            y.append(int(c))
        except:
            pass
    sum += (y[0] * 10 + y[-1])
print(sum)
sum = 0
nums = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]
for r in x:
    y = []
    for i in range(len(r)):
        try:
            y.append(int(r[i]))
        except:
            for j in range(len(nums)):
                if r[i:(i+len(nums[j]))] == nums[j]:
                    y.append(j+1)
    print(y[0] * 10 + y[-1])
    sum += (y[0] * 10 + y[-1])
print(sum)

