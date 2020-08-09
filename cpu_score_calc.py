cores = int(input('# of Cores: '))
threads = int(input('# of Threads: '))
baseFreq = float(input('Base frequency: '))
maxFreq = float(input('Max frequency: '))
cache = float(input('Cache: '))

# get average
average = cores + threads + baseFreq + maxFreq + cache
average /= 5
average = round(average, 2)
print(average)
