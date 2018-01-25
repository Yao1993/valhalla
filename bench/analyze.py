import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def get_label(library, system, backend):
    if system in library:
        label = library
    else:
        label = '_'.join([library, system])
    label = '_'.join([label, backend])
    return label


def calc_bandwidth(x):
    if x['value_type'] == 'float32':
        n_bytes = 4
    elif x['value_type'] == 'float64':
        n_bytes = 8
    else:
        n_bytes = None
    return [n_bytes * n / t / (1000000000/1000000) for n, t in zip(x['sequence'], x['time'])]


if __name__ == '__main__':
    bench_df = pd.HDFStore('test.hdf').select('test')
    bench_df['bandwidth'] = pd.Series({index: calc_bandwidth(row)for index, row in bench_df.iterrows()})

    plt.figure(dpi=300)
    for _, row in bench_df.iterrows():
        print(row['time'])
        print(row['bandwidth'])
        plt.loglog(row['sequence'], row['bandwidth'],
                   '.-',
                   label=get_label(row['library'], row['system'], row['backend']))
    plt.xlabel('Vector Size (Number of elements)')
    plt.ylabel('Bandwidth (GBPS)')
    plt.title('Reduce Performance')
    plt.legend()
    plt.show()
