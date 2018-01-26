from pathlib import Path
import subprocess
import pandas as pd
import numpy as np
import collections

from config import bench_config

Record = collections.namedtuple('Record', ['library', 'args', 'output'])


def start_bench(binary_dir, config, bench_target, options):
    binary_dir = Path(binary_dir)
    library_dirs = [library_dir for library_dir in binary_dir.iterdir() if library_dir.is_dir()]
    bench_result = []
    for library_dir in library_dirs:
        target_executables = [executable for executable in (library_dir / config).iterdir()
                              if bench_target in executable.name]
        for target_executable in target_executables:
            args = [str(target_executable), *options]
            print(args)
            bench_result.append(Record(library=library_dir.name,
                                       args=args,
                                       output=subprocess.check_output(args, shell=True)))
    return bench_result


ArgContent = collections.namedtuple('ArgContent', ['executable', 'value_type', 'sequence'])


def parse_args(args):
    # Usage: %s <data_type> <start> [<end>  [<step>]]
    if len(args) < 3:
        return None

    # decide value type
    if args[1] == 'f':
        value_type = 'float32'
    elif args[1] == 'd':
        value_type = 'float64'
    else:
        return None

    start = int(args[2])
    end = int(args[3]) if len(args) > 3 else start

    is_multi = False
    step = start
    if len(args) > 4:
        is_multi = args[4].startswith('x') or args[4].startswith('X')
        step = int(args[4][1:])

    sequence = []
    n = start
    while n <= end:
        sequence.append(n)
        if is_multi:
            n *= step
        else:
            n += step

    return ArgContent(executable=Path(args[0]).stem, value_type=value_type, sequence=sequence)


def get_system_backend(stem):
    host_backends = ['cpp', 'cpu', 'tbb', 'omp']
    device_backends = ['opencl', 'cuda']
    backend = stem.split('_')[1]
    if backend in host_backends:
        return 'host', backend
    elif backend in device_backends:
        return 'device', backend
    else:
        return None


def dump_result(records, hdf_filename, key):
    bench_df = pd.DataFrame(index=np.arange(0, len(records)),
                            columns=['library', 'executable', 'value_type', 'sequence', 'time', 'system', 'backend'])
    for i, record in enumerate(records):
        arg_content = parse_args(record.args)
        time = []
        for line in record.output.splitlines()[1:]:
            time.append(float(line.split()[1].strip()))
        bench_df.loc[i] = pd.Series({'library': record.library, 'executable': arg_content.executable,
                                     'value_type': arg_content.value_type, 'sequence': arg_content.sequence,
                                     'time': time, 'system': get_system_backend(arg_content.executable)[0],
                                     'backend': get_system_backend(arg_content.executable)[1]})

        bench_df.to_hdf(hdf_filename, key)


if __name__ == '__main__':
    for target in bench_config.targets:
        records = start_bench(bench_config.bin_dir, bench_config.config, target, bench_config.options)
        dump_result(records, bench_config.hdf_filename, target)
