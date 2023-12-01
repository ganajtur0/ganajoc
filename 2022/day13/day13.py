def compare_lists(first, second):
    #print('compare lists')
    while len(first) > 0 and len(second) > 0:
        left = first.pop(0)
        right = second.pop(0)
        #print(f"{left=}, {right=}")
        if type(left) == int and type(right) == int:
            if left < right:
                return 1
            elif left > right:
                return -1
        if type(left) == list and type(right) == list:
            sub_comparison = compare_lists(left, right)
            if sub_comparison != 0:
                return sub_comparison
        if type(left) == int and type(right) == list:
            sub_comparison = compare_lists(list([left]), right)
            if sub_comparison != 0:
                return sub_comparison
        if type(left) == list and type(right) == int:
            sub_comparison = compare_lists(left, list([right]))
            if sub_comparison != 0:
                return sub_comparison
    #print('compare lengths', f"{first=}, {second=}")
    if len(first) < len(second):
        return 1
    elif len(first) > len(second):
        return -1
    else:
         return 0

def solve1(file):
    with open(file, 'r') as f:
        lines = f.readlines()
        lines = [entry.strip() for entry in lines]

    index = 1
    indices = []
    while len(lines) > 0:
        list_a = eval(lines.pop(0))
        list_b = eval(lines.pop(0))
        if len(lines) > 0:
            lines.pop(0)

        comparison = compare_lists(list_a, list_b)
        if comparison == 1:
            indices.append(index)
        index += 1
    print(indices)
    print(sum(indices))

if __name__ == "__main__":
    solve1("i")
