#!/usr/bin/env python
# coding: utf-8

# In[13]:


# A Simplified Version of PGM Index. The code assesses the index behaviour when the dataset is scaled up
#and when there is data drift introduced 

import pandas as pd

# binary search for fast lookup in sorted data
from bisect import bisect_left, bisect_right

# measure performance (timing queries)
from time import perf_counter

# plotting (may use later to visualize BMI distribution)
import matplotlib.pyplot as plt

#measure memory usage
from pympler import asizeof


# In[14]:


class Segment:
    # stores info for one segment (linear model)

    def __init__(self, start_pos, end_pos, min_key, max_key, slope, intercept):
        # stores the range and linear model for one segment
        self.start_pos = start_pos
        self.end_pos = end_pos
        self.min_key = min_key
        self.max_key = max_key
        self.slope = slope
        self.intercept = intercept

    def predict(self, value):
        # estimate where this value should be in the array
        return int(round(self.slope * value + self.intercept))


# In[15]:


class SimplifiedPGMIndex:
    def __init__(self, bmi_values, error_bound=8):
        # input data (must already be sorted)
        self.bmi_values = bmi_values

        # how much prediction error allowed
        self.error_bound = error_bound

        # store all segments after building the index
        self.segments = []

        # used to quickly find the right segment later
        self.segment_starts = []


    
    def build(self):
        # go through the data and create segments
        total_values = len(self.bmi_values)
        start_pos = 0

        while start_pos < total_values:
            end_pos = start_pos

            # keep extending the segment while it stays within error bound
            while end_pos + 1 < total_values and self.is_segment_valid(start_pos, end_pos + 1):
                end_pos += 1

            current_segment = self.create_segment(start_pos, end_pos)
            self.segments.append(current_segment)

            # move to next part of the data
            start_pos = end_pos + 1

        # store starting values for each segment fast lookup later
        self.segment_starts = [seg.min_key for seg in self.segments]




    

    def create_segment(self, start_pos, end_pos):
        # get the values at the start and end
        min_key = self.bmi_values[start_pos]
        max_key = self.bmi_values[end_pos]

        # if only one value (or duplicates), use flat model
        if start_pos == end_pos or min_key == max_key:
            slope = 0
            intercept = start_pos
        else:
            # build linear model
            slope = (end_pos - start_pos) / (max_key - min_key)
            intercept = start_pos - slope * min_key

        return Segment(
            start_pos=start_pos,
            end_pos=end_pos,
            min_key=min_key,
            max_key=max_key,
            slope=slope,
            intercept=intercept
        )

    
    def is_segment_valid(self, start_pos, end_pos):
        # check if one linear model can represent this range
        seg = self.create_segment(start_pos, end_pos)

        for i in range(start_pos, end_pos + 1):
            predicted_pos = seg.predict(self.bmi_values[i])

            # if prediction is too far off, not valid
            if abs(predicted_pos - i) > self.error_bound:
                return False

        return True

    

    def find_segment(self, value):
        # find which segment this value belongs to
        pos = bisect_right(self.segment_starts, value) - 1

        if pos < 0:
            return self.segments[0]

        if pos >= len(self.segments):
            return self.segments[-1]

        return self.segments[pos]


    

    def search(self, value):
        
        # find correct segment
        seg = self.find_segment(value)

        # predict approximate position
        predicted_pos = seg.predict(value)

        # search in small range around prediction
        left = max(seg.start_pos, predicted_pos - self.error_bound)
        right = min(seg.end_pos, predicted_pos + self.error_bound)

        found_pos = bisect_left(self.bmi_values, value, left, right + 1)

        if found_pos <= right and found_pos < len(self.bmi_values) and self.bmi_values[found_pos] == value:
            return found_pos

        return None


# In[16]:


# load BMI values for baseline dataset
df = pd.read_csv("diabetes_70692_BMIsorted.csv", header=None)

# convert column to list
bmi_values = df[0].tolist()

# check file loaded correctly
print(len(bmi_values))
print(bmi_values[:10])
print(bmi_values[-10:])


# In[17]:


# create indexes for baseline dataset
pgm_index = SimplifiedPGMIndex(bmi_values, error_bound=8)

# build the index (create segments)
pgm_index.build()

# quick check
print("Number of segments:", len(pgm_index.segments))


# In[18]:


# test a few lookups
print("First value index:", pgm_index.search(bmi_values[0]))

print("Middle value index:", 
      pgm_index.search(bmi_values[len(bmi_values)//2]))

print("Last value index:", pgm_index.search(bmi_values[-1]))
pgm_index.search(98)


# In[19]:


# create lookup values using each distinct BMI value
queries = sorted(set(bmi_values))

print("Number of lookup values:", len(queries))
print(queries)


# In[20]:


# run lookups multiple times and measure time
repeats = 100
found = 0

start_time = perf_counter()

for i in range(repeats):
    for value in queries:
        result = pgm_index.search(value)
        if result is not None:
            found += 1

end_time = perf_counter()

total_time = end_time - start_time
total_searches = len(queries) * repeats

print("Total searches:", total_searches)
print("Values found:", found)
print("Total time (seconds):", total_time)
print("Average time per lookup (nanoseconds):", (total_time / total_searches) * 1000000000)
# index size in segments
print("Index size (segments):", len(pgm_index.segments))
print("Memory usage (bytes): ", asizeof.asizeof(pgm_index))


# In[21]:


# will be used later for plotting
lookuptime_70k = (total_time / total_searches) * 1000000000 #nanosecond
Nbrofsegments_70k = len(pgm_index.segments) 
memorySize_70k = asizeof.asizeof(pgm_index)   #bytes


# In[22]:


##### load scaled dataset (file has 10 times more data but there is no  drift )
df = pd.read_csv("diabetes_706920_BMIsorted.csv", header=None)
bmi_values = df[0].tolist()

# quick check
print(len(bmi_values))

# build index
pgm_index = SimplifiedPGMIndex(bmi_values, error_bound=8)
pgm_index.build()

print("Segments:", len(pgm_index.segments))

# queries
queries = sorted(set(bmi_values))
print("Distinct values:", len(queries))


# In[23]:


# run lookups multiple times and measure time
repeats = 100
found = 0

start_time = perf_counter()

for i in range(repeats):
    for value in queries:
        result = pgm_index.search(value)
        if result is not None:
            found += 1

end_time = perf_counter()

total_time = end_time - start_time
total_searches = len(queries) * repeats

print("Total searches:", total_searches)
print("Values found:", found)
print("Total time (seconds):", total_time)
print("Average time per lookup (nanoseconds):", (total_time / total_searches) * 1000000000)
# index size in segments
print("Index size (segments):", len(pgm_index.segments))
print("Memory usage (bytes): ", asizeof.asizeof(pgm_index))


# In[24]:


# will be used later for plotting
lookuptime_700k = (total_time / total_searches ) *1000000000  # converted to nanoseconds
Nbrofsegments_700k = len(pgm_index.segments) 
memorySize_700k = asizeof.asizeof(pgm_index)   #bytes


# In[25]:


#load data for drifted dataset
df = pd.read_csv("diabetes_800000_BMIsorted_drift.csv", header=None)
bmi_values = df[0].tolist()

# check
print(len(bmi_values))

# build index
pgm_index = SimplifiedPGMIndex(bmi_values, error_bound=8)
pgm_index.build()

print("Segments:", len(pgm_index.segments))

#queries
queries = sorted(set(bmi_values))
print("Distinct values:", len(queries))


# In[26]:


# run lookups multiple times and measure time
repeats = 100
found = 0

start_time = perf_counter()

for i in range(repeats):
    for value in queries:
        result = pgm_index.search(value)
        if result is not None:
            found += 1

end_time = perf_counter()

total_time = end_time - start_time
total_searches = len(queries) * repeats

print("Total searches:", total_searches)
print("Values found:", found)
print("Total time (seconds):", total_time)
print("Average time per lookup (nanoseconds):", (total_time / total_searches) * 1000000000)
# index size in segments
print("Index size (segments):", len(pgm_index.segments))
print("Memory usage (bytes): ", asizeof.asizeof(pgm_index))


# In[27]:


# will be used later for plotting
lookuptime_800k = (total_time / total_searches ) * 1000000000 #nanosecond
Nbrofsegments_800k = len(pgm_index.segments)
memorySize_800k = asizeof.asizeof(pgm_index)   #bytes


# In[28]:


datasets = ["70k Baseline", "700k Scaled", "800k Drifted"]

Avg_lookup_time = [lookuptime_70k, lookuptime_700k, lookuptime_800k]
Segments = [Nbrofsegments_70k, Nbrofsegments_700k, Nbrofsegments_800k]
Memory_in_mb = [memorySize_70k, memorySize_700k, memorySize_800k]



# In[29]:


# plot comparison of outcomes for datasets
# lookup time
plt.figure()
plt.bar(datasets, Avg_lookup_time)
plt.title("Lookup Time Comparison")
plt.ylabel("Time (nanoseconds)")
plt.xlabel("Datasets")
plt.show()


# segment
plt.figure()
plt.bar(datasets, Segments)
plt.title("Number of Segments")
plt.ylabel("Segments")
plt.xlabel("Datasets")
plt.show()


# memory
plt.figure()
plt.bar(datasets, Memory_in_mb)
plt.title("Memory Usage")
plt.ylabel("Memory (bytes)")
plt.xlabel("Datasets")
plt.show()


# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:




