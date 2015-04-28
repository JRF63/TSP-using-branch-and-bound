import sys

def parse(filename):
    num_cities = None
    cities = []
    with open(filename) as f:
        num_cities = int(f.readline().strip())
        for line in f:
            if line:
                x, y = map(float, line.strip().split())
                cities.append((x,y))
    assert len(cities) == num_cities
    return cities

def plot_data(filename):
    import matplotlib.pyplot as plt
    cities = parser(filename)
    xs = []
    ys = []
    x_max = 0.
    x_min = float('inf')
    y_max = 0.
    y_min = float('inf')
    for x, y in cities:
        if x > x_max:
            x_max = x
        if x < x_min:
            x_min = x
        if y > y_max:
            y_max = y
        if y < y_min:
            y_min = y
        xs.append(x)
        ys.append(y)

    plt.scatter(xs, ys)
    plt.xlim((x_min-1,x_max+1))
    plt.ylim((y_min-1,y_max+1))
    plt.show()

def tsp_comb(length, num_zeroes):
    # Gosper's hack
    s = (1 << num_zeroes) - 1
    limit = (1 << length)
    while (s < limit):
        if s & 1:
            yield s
        c = s & -s
        r = s + c
        s = (((r^s) >> 2) // c) | r
    
def tsp(cities):
    def idx(i, j):
        return i*num_cities + j
    num_cities = len(cities)
    
    table = {}
    table[idx(1, 0)] = 0
    new_table = {}
    
    costs = [0 for _ in range(num_cities**2)]
    for i in range(num_cities):
        x0, y0 = cities[i]
        for j in range(num_cities):
            x1, y1 = cities[j]
            cost = ((x0 - x1)**2 + (y0 - y1)**2)**0.5
            costs[idx(i,j)] = cost
            
    for m in range(2, num_cities + 1):
        print('Size: %s' % m)
        for s in tsp_comb(num_cities, m):
            tct = [j for j in range(num_cities) if ((s >> j) & 1)]
            for i in range(1, len(tct)):
                j = tct[i]
                x0, y0 = cities[j]
                
                minimum = float('inf')
                for z in range(len(tct)):
                    if z == i:
                        continue
                    
                    k = tct[z]
                    index = idx(s^(1<<j), k)
                    
                    if index not in table:
                        continue
                    
                    cost = table[index] + costs[idx(j,k)]
                    if cost < minimum:
                        minimum = cost
                
                new_table[idx(s,j)] = minimum
                    
        table = new_table
        new_table = {}
      
    p_set = 2**num_cities
    options = []
    x0, y0 = cities[0]
    for j in range(1, num_cities):
        x1, y1 = cities[j]
        index = idx(p_set - 1, j)
        if index not in table:
            continue
        options.append(table[index] + costs[idx(0,j)])
    ans = min(options)
    return ans
        
def assignment():
    filename = sys.argv[1]
    cities = parse(filename)
    print('Parsing complete')
    ans = tsp(cities)
    print(ans)
    #26442
    
assignment()