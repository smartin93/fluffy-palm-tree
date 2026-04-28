import numpy as np
import pandas as pd
import time
import sys
import matplotlib.pyplot as plt
from BTrees.OOBTree import OOBTree

# ==============================
# HARDCODED DATASETS
# ==============================

DATASETS = [
    ("baseline", "baseline.csv"),
    ("scaled", "scaled.csv"),
    ("drift", "drift.csv"),
]

# ==============================
# LOAD DATASET
# ==============================

def load_dataset(filename):
    data = pd.read_csv(filename).iloc[:, 0].values
    return np.sort(data.astype(int))


# ==============================
# B-TREE
# ==============================

def build_btree(data):
    tree = OOBTree()
    start = time.perf_counter()
    for i, key in enumerate(data):
        tree[key] = i
    build_time = time.perf_counter() - start
    return tree, build_time


def btree_lookup(tree, key):
    start = time.perf_counter()
    _ = tree.get(key, None)
    return time.perf_counter() - start


def btree_range(tree, low, high):
    start = time.perf_counter()
    _ = list(tree.items(min=low, max=high))
    return time.perf_counter() - start


def estimate_memory(tree):
    return len(tree) * 64  # rough bytes per entry


# ==============================
# EXPERIMENT
# ==============================

def run_experiment(dataset_type, filename):
    data = load_dataset(filename)
    size = len(data)

    tree, build_time = build_btree(data)

    keys = np.random.choice(data, size=500)
    ranges = [(k, k + 50) for k in keys]

    lookup_times = [btree_lookup(tree, k) for k in keys]
    range_times = [btree_range(tree, low, high) for low, high in ranges]

    return {
        "dataset_type": dataset_type,
        "size": size,
        "lookup_avg": to_ns(np.mean(lookup_times)),
        "range_avg": to_ns(np.mean(range_times)),
        "build_time": to_ms(build_time),
        "memory_est": estimate_memory(tree)
    }


def to_ms(seconds):
    return seconds * 1e3


def to_ns(seconds):
    return seconds * 1e9


# ==============================
# PLOTTING
# ==============================

def format_table(df):
    return df.rename(columns={
        "lookup_avg": "lookup_avg (ns)",
        "range_avg": "range_avg (ns)",
        "build_time": "build_time (ms)",
        "memory_est": "memory est (bytes)"
    })


def plot_results(df):
    metrics = {
        "lookup_avg": "Lookup Latency (ns)",
        "range_avg": "Range Query Latency (ns)",
        "build_time": "Build Time (ms)",
        "memory_est": "Memory Usage (bytes)"
    }

    # Assign colors per dataset type
    color_map = {
        "baseline": "tab:blue",
        "scaled": "tab:green",
        "drift": "tab:orange"
    }

    # Create labels like "baseline (10000)"
    df["label"] = df.apply(
        lambda row: f"{row['dataset_type']} ({row['size']})", axis=1
    )

    for metric, ylabel in metrics.items():
        plt.figure()

        x = np.arange(len(df))
        values = df[metric]

        # Map colors to each bar
        colors = [color_map.get(dtype, "gray") for dtype in df["dataset_type"]]

        plt.bar(x, values, color=colors)

        plt.xticks(x, df["label"], rotation=30)
        plt.xlabel("Dataset Type (Index Size)")
        plt.ylabel(ylabel)
        plt.title(f"{ylabel} by Dataset Type")

        plt.grid(axis='y')

        # Create legend manually
        handles = [
            plt.Rectangle((0,0),1,1, color=color_map[k])
            for k in color_map
        ]
        plt.legend(handles, color_map.keys())

        plt.savefig(f"{metric}_bar.png")
        plt.show()


# ==============================
# MAIN
# ==============================

def main():
    results = []

    for dtype, fname in DATASETS:
        print(f"Running: {dtype} - {fname}")
        res = run_experiment(dtype, fname)
        results.append(res)

    df = pd.DataFrame(results)

    print("\nResults table:")
    # print(df)
    formatted_df = format_table(df)
    print(formatted_df.to_string(index=False))

    df.to_csv("results.csv", index=False)

    plot_results(df)


if __name__ == "__main__":
    main()