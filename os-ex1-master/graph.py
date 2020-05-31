import matplotlib.pyplot as plt
import pandas as pd

results = {
    "BARE METAL": {
        "operation": 1.82,
        "function call": 1.58,
        "system call": 349.86
    },
    "CONTAINER": {
        "operation": 1.83,
        "function call": 1.93,
        "system call": 349.74
    },
    "VM": {
        "operation": 32.2,
        "function call": 19.5,
        "system call": 569.6}
}


def main():
    df = pd.DataFrame.from_records(results)
    df.plot.bar()
    plt.yscale('log')
    plt.title('Results For OSM Time Measurements')
    plt.ylabel('Time in ns (log scale)')
    plt.savefig('results.png', bbox_inches='tight', pad_inches=0.2)


if __name__ == '__main__':
    main()
