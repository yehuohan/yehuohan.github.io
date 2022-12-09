
import os
import platform
import re

def NewWalk(top, suffixs, exdirs):
    """
    NotImplement new walk function with suffix and dir filter.
    """
    top = os.fspath(top)
    dirs = []
    nondirs = []

    # We have read permission to our project obviously.
    scandir_it = os.scandir(top)
    with scandir_it:
        while True:
            try:
                try:
                    entry = next(scandir_it)
                except StopIteration:
                    break
            except OSError as error:
                return

            try:
                is_dir = entry.is_dir()
            except OSError:
                # If is_dir() raises an OSError, consider that the entry is not
                # a directory, same behaviour than os.path.isdir().
                is_dir = False

            if is_dir:
                # 'entry is not is exdirs'
                if entry.name not in exdirs:
                    dirs.append(entry.name)
            else:
                # 'suffixs is empty' or 'entry is in suffix'
                if not suffixs or os.path.splitext(entry.name)[1] in suffixs:
                    nondirs.append(entry.name)

    yield top, dirs, nondirs

    # Recurse into sub-directories
    islink, join = os.path.islink, os.path.join
    for dirname in dirs:
        new_path = join(top, dirname)
        if not islink(new_path):
            yield from NewWalk(new_path, suffixs, exdirs)

def GetDirsRecursive(paths, suffixs=[], exdirs=[]):
    """
    Get dirs recursive with suffix and dir filter.
    """
    flags = []
    for path in paths:
        for root, dirs, files in NewWalk(path, suffixs, exdirs):
            # files or dirs is not empty
            if files or dirs:
                flags.append(root)
    return flags

LOC_DIR = os.path.dirname(os.path.abspath(__file__))

for item in (GetDirsRecursive(
                [os.path.join(LOC_DIR, ''),],
                ['.c', '.cpp', '.h', '.hpp' ],
                ['sample', ])):
    print(item)
