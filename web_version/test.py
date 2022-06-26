import traceback


n = 4
dist = [[0, 10, 15, 20], [10, 0, 25, 25], [15, 25, 0, 30], [20, 25, 30, 0]]

memo = [[-1] * (1 << (n)) for _ in range(n)]
trace = [[-1] * (1 << (n)) for _ in range(n)]



def fun(i, mask):
    if mask == ((1 << i) | 1):
        memo[i][mask] = dist[0][i]
        trace[i][mask] = 0
        return dist[0][i]

    if memo[i][mask] != -1:
        return memo[i][mask]

    res = 10**9
    node = 0
    for j in range(0, n):
        if (mask & (1 << j)) != 0 and j != i and j != 0:
            tmp = fun(j, mask & (~(1 << i))) + dist[j][i]
            if tmp < res:
              res = tmp
              node = j
    memo[i][mask] = res
    trace[i][mask & (~(1<<i))] = node
    return res


ans = 10**9
mask = (1 << n) - 1
for i in range(0, n):
    tmp = fun(i, mask) + dist[i][0]
    if tmp < ans:
      ans = tmp
      memo[0][mask] = ans
      trace[0][mask] = i

mask = (1 << n) - 1
node = 0
trace_back = [0]
while mask != 1:
  nxt = trace[node][mask]
  trace_back.append(nxt)
  mask = mask & (~(1 << nxt))
  node = nxt
print(trace_back)

print("The cost of most efficient tour = " + str(ans))
