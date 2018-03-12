import collections
import os

BenchConfig = collections.namedtuple('BenchConfig', ['hdf_filename', 'targets', 'options', 'bin_dir', 'config'])

bench_config = BenchConfig(hdf_filename='{}.hdf'.format(os.environ['bench_platform']),
                        #    targets=['reduce', 'saxpy', 'inner_product', 'sort', 'transform_reduce'],
                           targets=['sort'],                                                          
                           options=['f', '1024', '1048576', 'x2'],
                           bin_dir='../bin',
                           config='Release')
