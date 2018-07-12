#!/usr/bin/env python3.5

import subprocess
import os
import argparse
import sys

from compilation_parser import *

def locationOfThisScript():
    return '/' + '/'.join(os.path.realpath(__file__).split('/')[1:-1])

def getRootDirectory():
    return locationOfThisScript() + '/../../'

def getBuildDirectory():
    return getRootDirectory() + 'build/'

def assemble_full_build_command():
    changeDirectoryCommand = 'cd ' + getBuildDirectory()
    makeCommand = 'make CBRMain'
    commands = [
        changeDirectoryCommand,
        makeCommand
    ]

    return ';'.join(commands)

def run_default_build():
    subprocess.call(assemble_full_build_command(), shell=True)

def clean_build_directory():
    cleanCommand = 'rm -rf ' + getBuildDirectory() + '*'
    print(cleanCommand)
    subprocess.call(cleanCommand, shell=True)

def update_cmake():
    updateCommand = "cd {}; cmake -G \'Unix Makefiles\' -D CMAKE_CXX_COMPILER=/usr/bin/clang++-6.0 ../".format(getBuildDirectory())
    print(updateCommand)
    subprocess.call(updateCommand, shell=True)


def run_build_with_parsing():
    compilationOutput = ""
    try:
        compilationOutput = subprocess.check_output(assemble_full_build_command(),
            stderr=subprocess.STDOUT,
            shell=True).decode("utf-8")
    except subprocess.CalledProcessError as compilationException:
        compilationOutput = compilationException.output.decode("utf-8")

    print(compilationOutput)

    errorLines = parse_error_lines(compilationOutput.split('\n'))
    for line in errorLines:
        print(line)

def run_cbr():
    runCommand = "cd {}projects/CBRMain; ./CBRMain {}config/config.json".format(getBuildDirectory(), getRootDirectory())
    print(runCommand)
    subprocess.call(runCommand, shell=True)


parser = argparse.ArgumentParser(description='Build script for CBR.')
parser.add_argument('--fancy', help='Fancy, parsed output of build.', action='store_true')
parser.add_argument('--clean', help='Deletes build directory.', action='store_true')
parser.add_argument('--nb', help='Abbreviation for \'No build\'.', action='store_true')
parser.add_argument('--run', help='Run without cmake update and build.', action='store_true')

args = parser.parse_args()

if not args.run:
    print('===== CBR compilation manager called with the following parameters ======')
    for arg in vars(args):
        print('{}: {}'.format(arg, getattr(args, arg)))
    print('=========================================================================')

if args.clean:
    clean_build_directory()
    print('Clean completed. Quitting.')
    sys.exit()

if not args.run:
    update_cmake()

if not args.nb or args.run:
    if args.fancy:
        run_build_with_parsing()
    else:
        run_default_build()

if args.run:
    run_cbr()




