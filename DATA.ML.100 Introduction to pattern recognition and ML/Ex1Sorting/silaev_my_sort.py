
digits = list(map(int, input("Give a list of integers separated by space: ").split()))
sort_digits = sorted (digits)

print("Given numbers sorted: " + ", ".join([ str(i)  for i in sort_digits ]))