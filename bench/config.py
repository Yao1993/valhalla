import collections

BenchConfig = collections.namedtuple('BenchConfig', ['hdf_filename', 'targets', 'options', 'bin_dir', 'config'])

bench_config = BenchConfig(hdf_filename='E5_2686V3_GTX_1080.hdf',
                           targets=['reduce', 'saxpy'],
                           options=['f', '1000', '100000000', 'x10'],
                           bin_dir='../bin',
                           config='Release')
