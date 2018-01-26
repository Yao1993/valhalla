import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from config import bench_config


def get_label(library, system, backend):
    if system in library:
        label = library
    else:
        label = '_'.join([library, system])
    label = '_'.join([label, backend])
    return label


def load_hdf(hdf_filename, key):
    return pd.HDFStore(hdf_filename).select(key)


def calc_bandwidth(data):
    def f(x):
        if x['value_type'] == 'float32':
            n_bytes = 4
        elif x['value_type'] == 'float64':
            n_bytes = 8
        else:
            n_bytes = None
        return [n_bytes * n / t / (1000000000 / 1000000) for n, t in zip(x['sequence'], x['time'])]

    data['bandwidth'] = pd.Series({index: f(row) for index, row in data.iterrows()})
    return data


def visualize(target, data):
    plt.figure(dpi=300)
    for _, row in data.iterrows():
        plt.loglog(row['sequence'], row['bandwidth'],
                   '.-',
                   label=get_label(row['library'], row['system'], row['backend']))
    plt.xlabel('Vector Size (Number of elements)')
    plt.ylabel('Bandwidth (GBPS)')
    plt.title('{} performance'.format(target))
    plt.legend()
    plt.savefig("{}.eps".format(target))


if __name__ == '__main__':
    for target in bench_config.targets:
        bench_df = load_hdf(bench_config.hdf_filename, target)
        bench_df = calc_bandwidth(bench_df)
        visualize(target, bench_df)
