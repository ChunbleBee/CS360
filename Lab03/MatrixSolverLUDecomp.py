from copy import deepcopy
# [| || || |]
# [| || || |]
# [| || || |]

# algorithm,

def ludecomp(matrix):
    if (len(matrix) != len(matrix[0])):
        print("Not a Square matrix - can't decompose to L and U!")
        return None
    length = len(matrix)

    upper = deepcopy(matrix)
    lower = [[0 for i in range(len(matrix))] for j in range(len(matrix))]

    for i in range(0, len(lower)):
        lower[i][i] = 1
    print(lower)

    for i in range(0, length - 1):
        print(i, ",", matrix[i])
        for j in range(i + 1, length):
            iVal = upper[i][i]
            jVal = upper[j][i]
            kVal = jVal/iVal
            print("[", i, ",", j, "]:",jVal, "/", iVal," = ", kVal)

            # 0 = x - n * y
            # x = n * y
            # n = x / y

            for k in range(i, length):
                upper[j][k] -= kVal * upper[i][k]
            print("\t", upper[j])
            lower[j][i] = kVal
    print(lower, "X", upper, "=", dotproduct(lower, upper))
    return (lower, upper)

def dotproduct(matA, matB):
    aNumRows = len(matA)
    aNumCols = len(matA[0])
    bNumRows = len(matB)
    bNumCols = len(matB[0])

    if (aNumCols != bNumRows):
        print("Can't calculate the dot product of these matrices!!\n\t", matA, "\n\t", matB)
        return None

    dot = [[0 for _ in range(0, bNumCols)] for _ in range(0, aNumRows)]

    for row in range(0, aNumRows):
        for col in range(0, bNumCols):
            for i in range(0, aNumCols):
                dot[row][col] += matA[row][i]*matB[i][col]
    return dot